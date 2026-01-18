import serial
import datetime
import time

# Serielle Schnittstelle öffnen (Port, Baudrate anpassen!)
ser = serial.Serial('COM10', 115200, timeout=1)  # Windows: 'COM1', Linux: '/dev/ttyUSB0'

text = "Hallo ueber serielle Schnittstelle!\n"
#ser.write(text.encode('utf-8'))  # Text als Bytes senden
ser.write(text.encode('utf-8'))
time.sleep(5)  # Kurze Pause

while(True):
    now = datetime.datetime.now()
    #zeit_text = now.strftime("%H:%M:%S %d.%m.%Y\n")  # Format: 2026-01-11 17:51:00
    text = now.strftime("%H:%M:%S  %d.%m.\n")  # Format: 2026-01-11 17:51:00
    ser.write(text.encode('utf-8'))
    print("Gesendet:", text.strip())
    time.sleep(1)  # Kurze Pause für stabile Übertragung
    
ser.close()  # Port schließen
