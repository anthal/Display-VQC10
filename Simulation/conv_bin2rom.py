# Python 3
# Konvertierung von Bin-Files in ROM Files für SimulIDE
# Script wurde durch Perplexity generiert
# Quellformat (Bin-File):
# Zielformat:
#  62,  18, 211,   1,  62,  52, 211,   1,  24, 246,  11,  12,  12,  14,  15,  22
#   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0

import sys

if len(sys.argv) < 2:
    print("Usage: python script.py inputfile [outputfile]")
    sys.exit(1)

file = sys.argv[1]
#output_file = sys.argv[2] if len(sys.argv) > 2 else "output.txt"

# Dateinamen anpassen
input_file = file + ".bin"
output_file = file + ".data"

with open(input_file, "rb") as fin, open(output_file, "w", encoding="utf-8") as fout:
    byte_data = fin.read()
    for i in range(0, len(byte_data), 16):
        #print("i: ", i)
        # Hole 16 Bytes (oder weniger am Ende)
        chunk = byte_data[i:i+16]
        #print("chunk", chunk)
        # Wandle jedes Byte in Dezimalzahl um
        decimals = [str(b) for b in chunk]
        print(decimals)
        # Verbinde mit ", "
        line = ", ".join(decimals)
        fout.write(line + "\n")
