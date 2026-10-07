import struct#for packing binary data
import socket#for sending data over TCP

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # creates a TCP socket
socket.bind(('127.0.0.1', 12345))  # binds the socket to localhost and port
socket.listen(1)  # listens for incoming connections
data, addr = socket.accept()  # accepts a connection from a client
telemetry_data = data.recv(1024)  # receives telemetry data from the client
print(telemetry_data, type(telemetry_data), list(telemetry_data))  # confirms byte data

chunk_size = 5
telemetry_packets = {}

for i in range(0, len(telemetry_data), chunk_size):
    chunk = telemetry_data[i:i + chunk_size]#slices the data into chunks of 8 bytes
    header = struct.pack('<II', len(chunk), i // chunk_size)  # packs the length of the chunk and the index into binary format
    telemetry_packets[i // chunk_size] = header + chunk  # stores the header and chunk

print(telemetry_packets)#confirm chunk 

#makes the actual packets

with open('telemetry_packets.bin', 'wb') as telemetry_file:
    for chunk in telemetry_packets:
        telemetry_file.write(telemetry_packets[chunk])  # writes the chunk data

#writes packets to file

with open('telemetry_packets.bin', 'rb') as telemetry_file:
    while True:
        header = telemetry_file.read(8)  # reads the header (length + index)
        if not header:
            break  # end of file
        length, index = struct.unpack('<II', header)  # unpacks the header to get length and index
        chunk = telemetry_file.read(length)  # reads the chunk data based on the length


        #DONT REALLY UNDERSTAND, WRITTEN BY VS CODE

packet_original = b''.join(telemetry_packets[i] for i in sorted(telemetry_packets)) #recombines packets
original = packet_original.decode('utf-8') #decodes the recombined packets
print(original)  #verifies everything is correct
