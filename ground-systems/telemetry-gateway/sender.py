import socket
def findChecksum(data, k)#Double check how to define checksum

ip = socket.gethostname()
port = 12345
sender = socket.socket()
sender.connect((ip, port))

data = "Telemetry Data Telemetry Data Telemetry Data"
c1 = data[0:k]
c2 = data[k:2*k]
c3 = data[2*k:3*k]#Packet overlap?

print(f''c1)
print(f'\n'c2)
print(f'\n'c3) #Potentially prints packets in defined size?? Not sure yet

sender.send(data.encode('utf-8'))

sender.close()

