import socket


ip = socket.gethostname()
port = 12345
sender = socket.socket()
sender.connect((ip, port))

data = "Telemetry Data Telemetry Data Telemetry Data Telemetry Data "

k = 15

def findChecksum(data, k):#Double check how to define checksum
  c1 = data[0:k]
  c2 = data[k:2*k]
  c3 = data[2*k:3*k]#Packet overlap?
  c4 = data[3*k:4*k]

  sum = bin(int(sum[0:x], 2) + int(c3, 2) + int(c4, 2))[2:]

  if (len(sum) > k):
    x = len(sum) - k
    sum = bin(int(sum[0:x], 2) + int(sum[x:], 2))[2:]
  if (len(sum) < k):
    sum = '0'*(k - len(sum)) + sum

  checksum = ''
  for i in sum:
    if (i == '1'):
      checksum += '0'
    else:
      checksum += '1'
    return checksum


sender.send(data.encode('utf-8'))

sender.close()

