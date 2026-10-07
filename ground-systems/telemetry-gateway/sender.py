import socket

data1 = "First telemetry data!"
data2 = "GroundSystemGroundSystemGroundSystemGroundSystemGroundSystemGroundSystemGroundSystem" #It breaks after a certain amount of characters
string = (data1 + data2)

telemetry_data = bytes(string, 'utf-8')
print(telemetry_data, type(telemetry_data), list(telemetry_data))  #confirms byte data

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # creates a TCP socket
sock.connect(('127.0.0.1', 12345))  # connects to the server
sock.sendall(telemetry_data)  # sends the telemetry data to the server
sock.close()  
#sender 
