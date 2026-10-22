import socket

ip = socket.gethostname() #New feature is adding ip and port at the top to easily change in the future
port = 12345
size = 99999999 #Just to be safe on the size of data
#Also excluded loopback ip that was giving issues

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((ip, port))           
server.listen()
#Same basic connection setup
print("CONNECTION LINE OPEN")

con, addr = server.accept()

rawdata = con.recv(size)#No decoding yet, receives as bytes

with open("telemetry_data.txt", "wb") as rawdatafile:
    rawdatafile.write(rawdata)

print("raw data is received and written to file\n")

#Now need to add in the packets so that they are stored in determined sizes

data = rawdata.decode(encoding = 'utf-8')#Decodes

with open("readable_data.txt", "w") as datafile:
    datafile.write(data)

print("readable data is received and written to file\n")
