import socket
import struct
from tabnanny import check

ip = socket.gethostname() #New feature is adding ip and port at the top to easily change in the future
port = 12345

packet_size = 32
#packets are split in 4 bytes header, 4 bytes data, 8 bytes checksum

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((ip, port))           
server.listen()
#Same basic connection setup
print("CONNECTION LINE OPEN\n")

con, addr = server.accept()

rawdata = con.recv(packet_size)#No decoding yet, receives as bytes

with open("telemetry_data.txt", "wb") as rawdatafile:
    rawdatafile.write(rawdata)

print("raw data is received and written to file\n")

id = rawdata[0:4]
message_bytes = rawdata[4:8]
checksum_bytes = rawdata[8:]

id = struct.unpack('!I', id)[0]
message = message_bytes.decode(encoding = 'utf-8')
checksum = checksum_bytes.decode(encoding = 'utf-8')


with open("readable_data.txt", "w") as datafile:
    datafile.write(message)

print("readable data is written to file\n")

def find_receiverchecksum(message, k):
  m1 = message[0:k]

  binary_m1 = ''.join(format(ord(char), '08b') for char in m1)
#converts each data segment to binary
  receiver_sum = bin(int(binary_m1, 2))[2:]

  if (len(receiver_sum) > k):
    x = len(receiver_sum) - k
    receiver_sum = bin(int(receiver_sum[0:x],2) + int(receiver_sum[x:], 2))[2:]

  if (len(receiver_sum) < k):
    receiver_sum = '0' * (k - len(receiver_sum)) + receiver_sum

  receiver_checksum = ''
  for i in receiver_sum:
    if i == '1':
      receiver_checksum += '1'
    else:
      receiver_checksum += '0'

  return receiver_checksum

receiver_checksum = find_receiverchecksum(message, 16)

finalsum = bin(int(checksum, 2) + int(receiver_checksum, 2))[2:]

print(checksum)
print(receiver_checksum)

finalcomp = ''
for i in finalsum:
    if(i == '0'):
        finalcomp += '0'
    else:
        finalcomp += '1'

if (int(finalcomp, 2) == 0):
   print("NO ERRORS DETECTED")
else:
   print("MESSAGE CORRUPTED")