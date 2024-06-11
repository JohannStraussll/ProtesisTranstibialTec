import serial  # Asegúrate de importar la librería pySerial
import time

# Configurar la conexión serie con Arduino
arduino = serial.Serial('COM3', 9600)  # Reemplaza 'COM3' con el puerto correcto

time.sleep(2)  # Esperar a que la conexión se establezca

# Función para leer la respuesta del Arduino
def read_response():
    while arduino.in_waiting == 0:
        pass  # Esperar a que haya datos disponibles
    response = arduino.readline().decode().strip()
    return response

# Ejemplo de uso
while True:
    steps = read_response()  # Leer y mostrar el número de pasos
    print("Pasos totales:", steps)

arduino.close()
