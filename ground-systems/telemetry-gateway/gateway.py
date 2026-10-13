import socket

ip = socket.gethostname() #New feature is adding ip and port at the top to easily change in the future
port = 12345
#Also excluded loopback ip that was giving issues

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((ip, port))           
server.listen()
#Same basic connection setup
print("CONNECTION LINE OPEN")

con, addr = server.accept()

data = con.recv(9999).decode(encoding='utf-8')

print(data)#Here is where changes need to be made to write to files and decode at different times

