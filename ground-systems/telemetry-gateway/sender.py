import socket

ip = socket.gethostname()
port = 12345
sender = socket.socket()
sender.connect((ip, port))

data = "Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data Telemetry data  "

sender.send(data.encode('utf-8'))

sender.close()

