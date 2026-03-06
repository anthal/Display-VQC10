# VQC10
DDR LED Punkt-Matrix-Display VQC10 (Leiterplatte und Code)

![Leiterplatte](Images/4xVQC10_on.jpg)
![Leiterplatte](Images/8xVQC10_on.jpg)

## Leiterplatte
* KiCAD 9 Projekt
* Die Platinen sind nebeneinander anreihbar und es können 4 Stück kaskadiert werden.
* Ab der 2. Platine geht es mit Flachbandkabel zur nächsten Platine. 
* Jede bekommt eine eigene Adresse über die 2 Jumper.

**=== ACHTUNG ===**
* Das Layout wird noch überarbeitet, da Widerstände für die Kaskadierung fehlen! 
* Außerdem stimmen die Abstände bei dem VQC10 Footprint nicht!

### Halbleiterbauelemente
* 4 x VQC 10 (4-Digit-Punkt-Matrix-LED-Displays)
* 1 x 74LS138 (1-aus-8 Dekoder für Zeilen)
* 1 x 74LS154 (1-aus-16 Dekoder für Digits)
* 1 x 74LS123 (Schutzschaltung)
* 7 x BD140 (Zeilentreiber)

### Anschlüsse Leiterplatte Version 1
**An 20 pol. Steckerleiste:**
* SD0 - SD6 (7): Stellendecoder (74HCT154)
* ZD0 - ZD2 (3): Zeilendecoder (74HCT138)
* D1-D5 (5): Daten (Zeilen)

**Belegung der Anschlüsse:**
| Pin | Belegung | Raspberry Pi Pico | LC80ex | Pin LC80-Userport |
|-----|----------|-------------------|--------|-------------------|
| 1   | +5 V     | +5 V              | +5 V   | 26                |
| 2   | +5 V     | +5 V              | +5 V   | 25                |
| 3   | SD0      | GP20              | PB0    | 16                |
| 4   | ZD0      | GP15              | PA0    | 21                |
| 5   | SD1      | GP19              | PB1    | 18                |
| 6   | ZD1      | GP14              | PA1    | 19                |
| 7   | SD2      | GP18              | PB2    | 20                |
| 8   | ZD2      | GP12              | PA2    | 17                |
| 9   | SD3      | GP17              | PB3    | 22                |
| 10  | D1       | GP28              | PA3    | 7                 |
| 11  | SD4      | GP16              | -      | -                 |
| 12  | D2       | GP27              | PA4    | 9                 |
| 13  | SD5      | GP10              | -      | -                 |
| 14  | D3       | GP26              | PA5    | 11                |
| 15  | SD6      | GP9               | -      | -                 |
| 16  | D4       | GP22              | PA6    | 13                |
| 17  | Masse    | -                 | -      | -                 |
| 18  | D5       | GP21              | PA7    | 15                |
| 19  | Masse    | Masse             | Masse  | 1                 |
| 20  | Masse    | Masse             | Masse  | 2                 |

### Selektion der 4 Leiterplatten
| Bit (Digit-Dekoder)     | Pin (J2) | Belegung | Port-Pin (Raspberry Pi Pico) |
|----------|-------|-----|-----|
| 5 | 11 | SD4 | GP16 |
| 6 | 13 | SD5 | GP10 |

Leiterplatte 1: 
* Bit 5: direkt zum Controller
* Bit 6: direkt zum Controller

Leiterplatte 2:
* Bit 5: zum Controller, Widerstand gegen +5V 
* Bit 6: direkt zum Controller

Leiterplatte 3:
* Bit 5: direkt zum Controller
* Bit 6: zum Controller, Widerstand gegen +5V 

Leiterplatte 4:
* Bit 5: zum Controller, Widerstand gegen +5V 
* Bit 6: zum Controller, Widerstand gegen +5V


## Literatur
* Datenblatt
* Schaltungen aus der rfe

## Simulation
* Simulation von 1x VQC10 mit Z80 unter SimulIDE
* https://simulide.com/
![Leiterplatte](Images/simulide.png)

## Software
### Arduino Code
* Für den Raspberry Pi Pico
* auf Basis von https://github.com/versioduo/VQC10
* Vor Compilierung Example Datei und Files aus scr Verzeichnis in ein gemeinsames Verzeichnis verschieben

### Micropython Code
* Für den Raspberry Pi Pico

### Z80-Assembler
#### aZ80/RC2016-Computer
![Leiterplatte](Images/aZ80_mit_4x_VQC10.jpg)
4x VQC10 am aZ80 Computer

Benutzte Entwicklungssoftware:
* "Small Computer-Monitor"
* https://smallcomputercentral.com/small-computer-monitor/

#### LC80ex
![Leiterplatte](Images/4xVQC10_am_LC80ex_1200.jpg)
Test mit dem Nachbau des DDR-Platinen Computers LC80 


#### Z1013
ToDo

