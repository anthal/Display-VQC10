# Speichern und Durchschalten der Daten pro Stelle mit der L/H Flanke des Stellentakts
#

# VQC 10 einstellig
# Mit 74HCT138 als Zeilendecoder!
# Mit 74HCT154 als Spaltendecoder!

import time
from machine import Pin
from machine import Timer
from bitmap5x7 import getBitmap

# Zeilen Pins:
pin_z0 = 15
pin_z1 = 14
pin_z2 = 12
pin_z3 = 8  # Zur (De-)Aktivierung des Zeilendecoders 

# Spalten Pins:
pin_d1 = 28
pin_d2 = 27
pin_d3 = 26
pin_d4 = 22
pin_d5 = 21

# Takt Pins (Stellen):
pin_cp1 = 20
pin_cp2 = 19
pin_cp3 = 18
pin_cp4 = 17
pin_cp5 = 16
pin_cp6 = 10
# pin_cp7 = 9

# Zeilen:
#z = [0,0,0,0,0,0,0]
z = [1,1,1,1]
z[0] = Pin(pin_z0, Pin.OUT, value=0)    # create output pin 
z[1] = Pin(pin_z1, Pin.OUT, value=0)    # create output pin
z[2] = Pin(pin_z2, Pin.OUT, value=0)    # create output pin
z[3] = Pin(pin_z3, Pin.OUT, value=0)    # create output pin


# Spaltendaten:
d = [0,0,0,0,0]
d[0] = Pin(pin_d1, Pin.OUT, value=0)    # create output pin 
d[1] = Pin(pin_d2, Pin.OUT, value=0)    # create output pin
d[2] = Pin(pin_d3, Pin.OUT, value=0)    # create output pin
d[3] = Pin(pin_d4, Pin.OUT, value=0)    # create output pin
d[4] = Pin(pin_d5, Pin.OUT, value=0)    # create output pin

# Takte (Stellen):
cp = [0,0,0,0,0,0,0]
cp[0] = Pin(pin_cp1, Pin.OUT)    # create output pin 
cp[1] = Pin(pin_cp2, Pin.OUT)    # create output pin
cp[2] = Pin(pin_cp3, Pin.OUT)    # create output pin
cp[3] = Pin(pin_cp4, Pin.OUT)    # create output pin
cp[4] = Pin(pin_cp5, Pin.OUT)    # create output pin
cp[5] = Pin(pin_cp6, Pin.OUT)    # create output pin
#cp[6] = Pin(pin_cp7, Pin.OUT)    # create output pin


#text1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ"
#text2 = "abcdefghijklmnopqrstuvwxyzäöüß"
#text3 = "1234567890!§$%&/()=?[],.-;:_<>|@"
#text_in = "x+x+x+x+x+x+"
#text_in = text1 + text2 + text3
text = "ABCD"
#text = "abcd"
#print("Auf VQC 10:", text)
zeile_alt = 0
i = 0
zeile = 0

def zeilen_decoder(i):
    # Deaktivierung der Ausgänge ueber 74HCT138 (E2):
    z[3].value(0)
    #z[0].value(1)
    #z[1].value(1)
    #z[2].value(1)
    
    a = i & 1
    b = (i & 2) >> 1
    c = (i & 4) >> 2
    
    z[0].value(a)
    z[1].value(b)
    z[2].value(c)
    
    # Aktivierung der Ausgänge:
    z[3].value(1)
    #print(i,a,b,c)
    
    
def digit_decoder(i):
    cp[5].value(0)
    #cp[6].value(0)

    a = i & 1
    b = (i & 2) >> 1
    c = (i & 4) >> 2
    d = (i & 8) >> 3
    #e = (i & 16) >> 4
    cp[0].value(a)
    cp[1].value(b)
    cp[2].value(c)
    cp[3].value(d)
    # Aktivierung der Ausgänge:
    cp[4].value(0)
    # Deaktivierung der Ausgänge:
    cp[4].value(1)
    #print(i,a,b,c,d)
    
    
def timer_tick(text):
    global zeile
    
    # Alte Zeile deaktivieren (setze Dekoder auf Zeile 8):
    #z[zeile].value(1)
    #z[0].value(1)
    #z[1].value(1)
    #z[2].value(1)
    # Zeilendecoder dunkel schalten:
    z[3].value(0)
    
    zeile += 1
    if zeile == 7:
        zeile = 0
    #print(zeile)
        
    # Pro Stelle/Digit:
    for digit in range(len(text)):
        bitmap = getBitmap(ord(text[digit]))
        #print(digit, bitmap)
        
        # Pro Spalte:
        for spalte in range(5):
            bit = bitmap[spalte] & (1 << zeile)
            d[spalte].value(bit)

        # Daten fuer aktuelle Stelle uebernehmen:
        digit_decoder(digit)
        
    # Aktuelle Zeile aktivieren:
    #z[zeile].value(0)
    zeilen_decoder(zeile)
    

#tim = Timer(period=2, mode=Timer.PERIODIC, callback=lambda t:timer_tick(text))
tim = Timer(freq=200, mode=Timer.PERIODIC, callback=lambda t:timer_tick(text))
# freq=500
counter = 0
while True:
    zeit = time.localtime(time.time())
    #print(zeit)
    #text = "{0:02d}.{1:02d}.{2:02d} {3:02d}:{4:02d}".format(zeit[2],zeit[1],zeit[0],zeit[3],zeit[4],zeit[5])
    text = "{0:02d}.{1:02d}.  {3:02d}:{4:02d}:{5:02d}".format(zeit[2],zeit[1],zeit[0],zeit[3],zeit[4],zeit[5])    
    
    #time.sleep(1)
    #text = "S:{1:02d}".format(zeit[4], zeit[5])
    #print(text)
    #time.sleep(1)
    #text = "23°C"
    #text = "ABCDEFGHIJKLMNOP"
    #text = "Guten Abend Rosi"
    #text = "* Hallo Holger *"
    #text = "1234"
    #print(ord("°"))
    #text = "{0:016d}".format(counter)
    #print(text)
    #counter += 1
    #if counter > 9999:
    #    counter = 0
    #time.sleep(0.1)
    time.sleep(1)
    
    