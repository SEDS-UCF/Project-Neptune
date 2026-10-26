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
#start of checksum
k = 15

#function returns the checksum


def findReceiverchecksum(data, k, checksum):

    #splices the data into 
    c1 = data[0:k]
    c2 = data[k:2*k]
    c3 = data[2*k:3*k]
    c4 = data[3*k:4*k]

    receiversum = bin(int(c1, base=2) + int(c2, base=2) + int(checksum, base=2) + int(c3, base=2) + int(c4, base=2) + int(checksum, base=2))
    print(receiverchecksum)
    if (len(receiversum) > k):
         x = len(receiversum) - k
         receiversum = bin(int(receiversum[0:x], base=2) + int(receiversum[x:], base=2))[2:]

    if (len(receiversum) < k):
        sum = '0'*(k - len(receiversum)) + receiversum

    receiverchecksum = ''
    for i in receiversum:
        if (i =='1'):
            receiverchecksum += '0'
        else:
            receiverchecksum += '1'
        return receiverchecksum
#End checksum



rawdata = con.recv(size)#No decoding yet, receives as bytes

with open("telemetry_data.txt", "wb") as rawdatafile:
    rawdatafile.write(rawdata)

print("raw data is received and written to file\n")

#Now need to add in the packets so that they are stored in determined sizes

data = rawdata.decode(encoding = 'utf-8')#Decodes

with open("readable_data.txt", "w") as datafile:
    datafile.write(data)

print("readable data is received and written to file\n")

receiverchecksum = findReceiverchecksum(rawdata, k, checksum)

print(receiverchecksum)


