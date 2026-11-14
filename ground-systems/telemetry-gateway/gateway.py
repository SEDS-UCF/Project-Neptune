#This gateway script accepts the data from the sending script,
#then immediately writes that data to a file, 
#converts it from bytes into readable data to store,
#finally, it creates a receiver checksum to compare with the sent checksum

#Written by Dillon Del Rosario for Project Neptune - Ground Systems
#Checksum function modeled after Riya Kumar's code from GeeksforGeeks.org

import socket #Library for network communication

ip = socket.gethostname() 
port = 12345 #Defines the port for sending via socket
#Will need to update for sending from rocket antennas, or through the boats

packet_size = 40 #These numbers are just what I found through experimenting
id_size = 4 
data_size = 4
checksum_size = 32
#Packets are split in 4 bytes id, 4 bytes data, 32 bytes checksum

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((ip, port))           
server.listen()
print("CONNECTION LINE OPEN\n")
#Same basic connection setup

con, addr = server.accept()
#Accepts the connection

rawdata = con.recv(packet_size) #This is what receives each packet from sending script
#No decoding yet, receives as bytes

with open("raw_data.txt", "wb") as rawdatafile:
    rawdatafile.write(rawdata)
print("raw data is received and written to file\n")
#Now the data has been saved to the raw data file

#This is where the raw data is translated from the packet sizes defined earlier
id_bytes = rawdata[0:id_size]
message_bytes = rawdata[id_size:id_size + data_size]
checksum_bytes = rawdata[id_size + data_size:]

id = id_bytes.decode(encoding = 'utf-8')
message = message_bytes.decode(encoding = 'utf-8')
checksum = checksum_bytes.decode(encoding = 'utf-8')
#Now the decoding process has happened between all the components of the packet

with open("readable_data.txt", "w") as datafile:
    datafile.write(message)
print("readable data is written to file\n")
#This only saves the readable data to be looked at later

#Now this is the function to calculate receiver checksum, mirrored from the sender script
def find_receiverchecksum(message, k):
  binary_message = ''.join(format(ord(char), '08b') for char in message)
  receiver_sum = bin(int(binary_message, 2))[2:]
  #Converts the message into a sum in binary

  #Here are the overflow bit precautions
  if (len(receiver_sum) > k):
    x = len(receiver_sum) - k
    receiver_sum = bin(int(receiver_sum[0:x],2) + int(receiver_sum[x:], 2))[2:]
    #If the data is longer than the size defined from the data, it will split them and then add them together

  if (len(receiver_sum) < k):
    receiver_sum = '0' * (k - len(receiver_sum)) + receiver_sum
    #If it is shorter, it will add 0's to the front till it reaches the size

  receiver_checksum = ''
  for i in receiver_sum:
    if i == '1':
      receiver_checksum += '0'
    else:
      receiver_checksum += '1'
    #This calculates the checksum 

  return receiver_checksum

receiver_checksum = find_receiverchecksum(message, data_size) #Finds the receiver checksum of the translated message

#The checksums are now compared through subtracting the two values
finalsum = bin(int(checksum, 2) - int(receiver_checksum, 2))[2:]
#If the final sum is 0, the two checksums are the same, meaning the data has been stored safely

print("Checksum from Sender: " + checksum)
print("Checksum from Receiver: " + receiver_checksum + "\n")

if (int(finalsum, 2) == 0):
   print("NO ERRORS DETECTED")
else:
   print("MESSAGE CORRUPTED")
#Checksum verification

#Now that the script can receive data,
#the next step is to have a stream of incoming packets be stored and verified