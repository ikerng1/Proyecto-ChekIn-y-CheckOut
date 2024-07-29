#Código en Python para la Raspberry Pi para recibir el dato a través del socket.
#-------------------------------------------------------------------------------
import socket

# Creamos un socket para recibir el dato
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Conectamos al socket con la IP y puerto que utilizamos en el código anterior
sock.bind(("192.168.0.12", 8080))

# Escuchamos conexiones
sock.listen(1)

print("Esperando conexiones...")

while True:
    # Aceptamos la conexión
    conn, addr = sock.accept()
    print("Conexión establecida con", addr)

    # Recibimos el dato
    data = conn.recv(1024)
    print("Dato recibido:", data.decode())

    # Cerramos la conexión
    conn.close()