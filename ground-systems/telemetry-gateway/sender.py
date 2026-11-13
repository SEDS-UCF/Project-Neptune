#This sender script creates the message and id and converts both into bytes to send across network, 
#then it calculates a checksum of the message to also send,
#finally, it creates a packet of the id, message, and checksum (all in bytes) and sends it

#Written by Dillon Del Rosario for Project Neptune - Ground Systems
#Checksum function modeled after Riya Kumar's code from GeeksforGeeks.org


import socket #Library for network communication


ip = socket.gethostname()
port = 12345
sender = socket.socket()
sender.connect((ip, port))

message = "ABCD" #Defines the message, 4 bytes to be simple
message_bytes = message.encode('utf-8') #Converts the message to bytes

id = '0001' #Same thing with ID
id_bytes = id.encode('utf-8') #Converts the ID to bytes
#The id for now can only be a string because ints can't have leading 0's


def find_checksum(message, k):
  binary_message = ''.join(format(ord(char), '08b') for char in message)
  #converts each data segment to binary
  sum = bin(int(binary_message, 2))[2:]
  #Converts the binary message to a sum in binary

  #Here are the overflow bit precautions
  if (len(sum) > k):
    x = len(sum) - k
    sum = bin(int(sum[0:x],2) + int(sum[x:], 2))[2:]
    #If the data is longer than the size defined from the data, it will split them and then add them together

  if (len(sum) < k):
    sum = '0' * (k - len(sum)) + sum
    #If it is shorter, it will add 0's to the front till it reaches the size

  checksum = ''
  for i in sum:
    if i == '1':
      checksum += '0'
    else:
      checksum += '1'
    #This calculates the checksum

  return checksum

checksum = find_checksum(message, 4) 
checksum_bytes = checksum.encode('utf-8')
#This calculates the checksum from the message and the size of the message, and converts checksum to bytes

packet = id_bytes + message_bytes + checksum_bytes
#Creates the packet with the ID, message, and checksum (all in bytes)

print(packet) #Displays the packet to compare to the raw_data file

sender.send(packet) #Sends the packet to the gateway
sender.close()
#Ends the connection with the gateway


