// PLEASE READ - PLEASE READ - PLEASE READ - PLEASE READ - PLEASE READ - PLEASE READ - PLEASE READ - PLEASE READ

// YOU MUST ACTIVATE PSRAM:  TOOLS -> PSRAM -> OPI PSRAM
// If you do not, the file size wil be 0KB!

// THIS CODE WAS GENERATED WITH CLAUDE AND HAS NOT BEEN COMPLETELY REVIEWED!!

/*
  XIAO ESP32S3 Sense - Record VIDEO + AUDIO into ONE .avi file on the SD card

  Arduino IDE settings:
    Board:  XIAO_ESP32S3
    PSRAM:  OPI PSRAM   (must be enabled)
    Core:   esp32 by Espressif 3.x

  Output: /video.avi  (MJPEG video + 16 kHz mono PCM audio). Plays in VLC.
*/

#include "esp_camera.h"
#include "ESP_I2S.h"
#include "FS.h"
#include "SD.h"
#include "freertos/stream_buffer.h"

// ===================== SETTINGS YOU CAN CHANGE =====================
#define RECORD_SECONDS 20
#define FRAME_SIZE     FRAMESIZE_VGA   // 640x480. Try FRAMESIZE_QVGA for smoother video
#define JPEG_QUALITY   12              // 10-63. Lower = better picture, bigger file
#define MIC_GAIN       4.0f            // audio volume boost (2.0 - 8.0)
#define FILE_NAME      "/video.avi"
// ===================================================================

#define SAMPLE_RATE    16000
#define SD_CS_PIN      21
#define MIC_CLK_PIN    42
#define MIC_DATA_PIN   41
#define AUDIO_BUF_SIZE (128 * 1024)
#define MAX_INDEX      40000
#define HEADER_SIZE    324

// XIAO ESP32S3 Sense camera pins
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  10
#define SIOD_GPIO_NUM  40
#define SIOC_GPIO_NUM  39
#define Y9_GPIO_NUM    48
#define Y8_GPIO_NUM    11
#define Y7_GPIO_NUM    12
#define Y6_GPIO_NUM    14
#define Y5_GPIO_NUM    16
#define Y4_GPIO_NUM    18
#define Y3_GPIO_NUM    17
#define Y2_GPIO_NUM    15
#define VSYNC_GPIO_NUM 38
#define HREF_GPIO_NUM  47
#define PCLK_GPIO_NUM  13

struct IndexEntry { uint32_t id, offset, size; };

I2SClass i2s;
File aviFile;
StreamBufferHandle_t audioStream;
StaticStreamBuffer_t audioStreamStruct;
IndexEntry *indexTable;

volatile bool recording = false;
volatile bool audioTaskDone = false;

uint32_t idxCount = 0;
uint32_t moviBytes = 0;     // bytes written inside the 'movi' list
uint32_t frameCount = 0;
uint32_t audioBytes = 0;
uint32_t maxChunk = 0;
uint16_t vidW = 640, vidH = 480;

static uint8_t audioChunk[8192];

// ---------- little helpers ---------- (explain)
static uint32_t fourcc(const char *s) {
  return (uint32_t)s[0] | ((uint32_t)s[1] << 8) | ((uint32_t)s[2] << 16) | ((uint32_t)s[3] << 24);
}

static uint8_t *hp;
static void p32(uint32_t v) { memcpy(hp, &v, 4); hp += 4; }
static void p16(uint16_t v) { memcpy(hp, &v, 2); hp += 2; }
static void pfc(const char *s) { memcpy(hp, s, 4); hp += 4; }

// ---------- AVI header (always exactly HEADER_SIZE bytes) ----------
void writeHeader(uint32_t usPerFrame, uint32_t riffSize, uint32_t maxBps) {
  uint8_t hdr[HEADER_SIZE];
  hp = hdr;

  pfc("RIFF"); p32(riffSize); pfc("AVI ");

  pfc("LIST"); p32(292); pfc("hdrl");
    pfc("avih"); p32(56);
      p32(usPerFrame); p32(maxBps); p32(0); p32(0x10);      // 0x10 = has index
      p32(frameCount); p32(0); p32(2); p32(maxChunk);
      p32(vidW); p32(vidH); p32(0); p32(0); p32(0); p32(0);

    // video stream
    pfc("LIST"); p32(116); pfc("strl");
      pfc("strh"); p32(56);
        pfc("vids"); pfc("MJPG"); p32(0); p16(0); p16(0); p32(0);
        p32(usPerFrame); p32(1000000); p32(0); p32(frameCount);
        p32(maxChunk); p32(0xFFFFFFFF); p32(0);
        p16(0); p16(0); p16(vidW); p16(vidH);
      pfc("strf"); p32(40);
        p32(40); p32(vidW); p32(vidH); p16(1); p16(24); pfc("MJPG");
        p32((uint32_t)vidW * vidH * 3); p32(0); p32(0); p32(0); p32(0);

    // audio stream
    pfc("LIST"); p32(92); pfc("strl");
      pfc("strh"); p32(56);
        pfc("auds"); p32(0); p32(0); p16(0); p16(0); p32(0);
        p32(2); p32(SAMPLE_RATE * 2); p32(0); p32(audioBytes / 2);
        p32(sizeof(audioChunk)); p32(0xFFFFFFFF); p32(2);
        p16(0); p16(0); p16(0); p16(0);
      pfc("strf"); p32(16);
        p16(1); p16(1); p32(SAMPLE_RATE); p32(SAMPLE_RATE * 2); p16(2); p16(16);

  pfc("LIST"); p32(4 + moviBytes); pfc("movi");

  aviFile.write(hdr, HEADER_SIZE);
}

// ---------- write one video or audio chunk ----------
bool writeChunk(const char *id, const uint8_t *data, uint32_t len) {
  if (idxCount >= MAX_INDEX) return false;

  indexTable[idxCount++] = { fourcc(id), 4 + moviBytes, len };

  uint8_t head[8];
  memcpy(head, id, 4);
  memcpy(head + 4, &len, 4);
  aviFile.write(head, 8);
  aviFile.write(data, len);
  if (len & 1) aviFile.write((uint8_t)0);   // AVI chunks must be even length

  moviBytes += 8 + len + (len & 1);
  if (len > maxChunk) maxChunk = len;
  return true;
}

// ---------- move captured audio from buffer into the file ----------
void drainAudio() {
  size_t n;
  while ((n = xStreamBufferReceive(audioStream, audioChunk, sizeof(audioChunk), 0)) > 0) {
    writeChunk("01wb", audioChunk, n);
    audioBytes += n;
  }
}

// ---------- audio task: runs in parallel, reads mic nonstop ----------
void audioTask(void *) {
  int16_t buf[512];
  while (recording) {
    size_t n = i2s.readBytes((char *)buf, sizeof(buf));
    size_t samples = n / 2;
    for (size_t i = 0; i < samples; i++) {
      int32_t v = (int32_t)(buf[i] * MIC_GAIN);
      if (v > 32767) v = 32767;
      if (v < -32768) v = -32768;
      buf[i] = (int16_t)v;
    }
    if (n) xStreamBufferSend(audioStream, buf, samples * 2, 0);
  }
  audioTaskDone = true;
  vTaskDelete(NULL);
}

// ---------- setup helpers ----------
bool initCamera() {
  camera_config_t c = {};
  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer   = LEDC_TIMER_0;
  c.pin_d0 = Y2_GPIO_NUM; c.pin_d1 = Y3_GPIO_NUM; c.pin_d2 = Y4_GPIO_NUM; c.pin_d3 = Y5_GPIO_NUM;
  c.pin_d4 = Y6_GPIO_NUM; c.pin_d5 = Y7_GPIO_NUM; c.pin_d6 = Y8_GPIO_NUM; c.pin_d7 = Y9_GPIO_NUM;
  c.pin_xclk = XCLK_GPIO_NUM;
  c.pin_pclk = PCLK_GPIO_NUM;
  c.pin_vsync = VSYNC_GPIO_NUM;
  c.pin_href = HREF_GPIO_NUM;
  c.pin_sccb_sda = SIOD_GPIO_NUM;
  c.pin_sccb_scl = SIOC_GPIO_NUM;
  c.pin_pwdn = PWDN_GPIO_NUM;
  c.pin_reset = RESET_GPIO_NUM;
  c.xclk_freq_hz = 20000000;
  c.pixel_format = PIXFORMAT_JPEG;
  c.frame_size = FRAME_SIZE;
  c.jpeg_quality = JPEG_QUALITY;
  c.fb_count = 2;
  c.fb_location = CAMERA_FB_IN_PSRAM;
  c.grab_mode = CAMERA_GRAB_LATEST;
  return esp_camera_init(&c) == ESP_OK;
}

void halt(const char *msg) {
  Serial.println(msg);
  while (1) delay(1000);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  if (!psramFound()) halt("PSRAM not found! Set Tools > PSRAM > OPI PSRAM.");

  Serial.println("Starting camera...");
  if (!initCamera()) halt("Camera init failed!");

  Serial.println("Starting microphone...");
  i2s.setPinsPdmRx(MIC_CLK_PIN, MIC_DATA_PIN);
  if (!i2s.begin(I2S_MODE_PDM_RX, SAMPLE_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO))
    halt("Mic init failed!");

  Serial.println("Starting SD card...");
  if (!SD.begin(SD_CS_PIN)) halt("SD card mount failed!");

  indexTable = (IndexEntry *)ps_malloc(MAX_INDEX * sizeof(IndexEntry));
  uint8_t *sbStorage = (uint8_t *)ps_malloc(AUDIO_BUF_SIZE + 1);
  if (!indexTable || !sbStorage) halt("Out of memory!");
  audioStream = xStreamBufferCreateStatic(AUDIO_BUF_SIZE, 1, sbStorage, &audioStreamStruct);

  // Let the camera warm up and get the real frame size
  for (int i = 0; i < 5; i++) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) { vidW = fb->width; vidH = fb->height; esp_camera_fb_return(fb); }
  }

  SD.remove(FILE_NAME);
  aviFile = SD.open(FILE_NAME, FILE_WRITE);
  if (!aviFile) halt("Could not create file!");
  writeHeader(0, 0, 0);   // placeholder, fixed at the end

  // ---------- RECORD ----------
  Serial.printf("Recording %d seconds...\n", RECORD_SECONDS);
  recording = true;
  xTaskCreatePinnedToCore(audioTask, "audio", 8192, NULL, 5, NULL, 0);
  uint32_t startMs = millis();

  while (millis() - startMs < RECORD_SECONDS * 1000UL) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) continue;
    bool ok = writeChunk("00dc", fb->buf, fb->len);
    esp_camera_fb_return(fb);
    if (!ok) { Serial.println("Index full, stopping early."); break; }
    frameCount++;
    drainAudio();
  }

  recording = false;
  while (!audioTaskDone) delay(5);
  drainAudio();
  uint32_t durationMs = millis() - startMs;

  // ---------- FINISH FILE ----------
  Serial.println("Saving file...");

  // idx1 index (lets players seek)
  uint8_t head[8];
  uint32_t idxSize = idxCount * 16;
  memcpy(head, "idx1", 4);
  memcpy(head + 4, &idxSize, 4);
  aviFile.write(head, 8);
  for (uint32_t i = 0; i < idxCount; i++) {
    uint32_t e[4] = { indexTable[i].id, 0x10, indexTable[i].offset, indexTable[i].size };
    aviFile.write((uint8_t *)e, 16);
  }

  uint32_t fileSize   = HEADER_SIZE + moviBytes + 8 + idxSize;
  uint32_t usPerFrame = frameCount ? (uint32_t)((uint64_t)durationMs * 1000 / frameCount) : 100000;
  uint32_t maxBps     = durationMs ? (uint32_t)((uint64_t)moviBytes * 1000 / durationMs) : 0;

  aviFile.seek(0);
  writeHeader(usPerFrame, fileSize - 8, maxBps);
  aviFile.close();

  Serial.printf("Done! %lu frames (%.1f fps), %.1f s of audio -> %s\n",
                (unsigned long)frameCount, frameCount * 1000.0f / durationMs,
                audioBytes / (2.0f * SAMPLE_RATE), FILE_NAME);
}

void loop() {
  delay(1000);
  Serial.printf(".");
}
