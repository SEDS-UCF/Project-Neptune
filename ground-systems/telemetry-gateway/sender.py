import socket
import struct

ip = socket.gethostname()
port = 12345
sender = socket.socket()
sender.connect((ip, port))

message = "ABCD"
message_bytes = message.encode('utf-8')

id = 1

def find_checksum(message, k):
  m1 = message[0:k]

  binary_m1 = ''.join(format(ord(char), '08b') for char in m1)
#converts each data segment to binary
  sum = bin(int(binary_m1, 2))[2:]

  if (len(sum) > k):
    x = len(sum) - k
    sum = bin(int(sum[0:x],2) + int(sum[x:], 2))[2:]

  if (len(sum) < k):
    sum = '0' * (k - len(sum)) + sum

  checksum = ''
  for i in sum:
    if i == '1':
      checksum += '0'
    else:
      checksum += '1'

  return checksum

checksum = find_checksum(message, 16)
checksum_bytes = checksum.encode('utf-8')

packet = struct.pack('!I', id) + message_bytes + checksum_bytes

print(packet)

sender.send(packet)
sender.close()


