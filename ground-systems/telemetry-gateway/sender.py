import socket


ip = socket.gethostname()
port = 12345
sender = socket.socket()
sender.connect((ip, port))

data = "Telemetry Data Telemetry Data Telemetry Data Telemetry Data "
#start of checksum
k = 15

def findChecksum(data, k):#Double check how to define checksum
  c1 = data[0:k]
  c2 = data[k:2*k]
  c3 = data[2*k:3*k]#Packet overlap?
  c4 = data[3*k:4*k]
  print(c1)
  print(c2)
  print(c3)
  print(c4)
  binary_c1 = ''.join(format(ord(letter), '08b') for letter in c1) # '08b' tells ord() to turn into 8-bit binary
  print(binary_c1) # worked because turned string into ASCII (integer i think?) and then into binary
  sum = bin(int(c1, base=2) + int(c2, base=2) + int(c3, base=2) + int(c4, base=2))[2:]

  if (len(sum) > k):
    x = len(sum) - k
    sum = bin(int(sum[0:x], base=2) + int(sum[x:], base=2))[2:]

  if (len(sum) < k):
    sum = '0'*(k - len(sum)) + sum

  checksum = ''
  for i in sum:
    if (i == '1'):
      checksum += '0'
    else:
      checksum += '1'
    return checksum

checksum = findChecksum(data, k)
#End checksum
print(checksum)

sender.send(data.encode('utf-8'))

sender.close()

