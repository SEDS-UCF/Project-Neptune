import socket

ip = socket.gethostname()
port = 12345

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((ip, port))
                   
server.listen()

print("CONNECTION LINE OPEN")

con, addr = server.accept()

data = con.recv(9999).decode(encoding='utf-8')

print(data)

