import serial

# Abre a conexão com a porta serial
serial_port = serial.Serial("COM3", 9600)  # substitua 'COM3' pela porta serial do seu dispositivo


# Aguarda receber uma mensagem pela porta serial
while True:
    # Envia uma mensagem pela porta serial
    mensagem_enviada = input("Comandos : ")
    serial_port.write(mensagem_enviada.encode())
    mensagem_recebida = serial_port.readline().decode().strip()
    if mensagem_recebida:
        print("Mensagem recebida: " + mensagem_recebida)
