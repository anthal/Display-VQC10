@echo off
set file=%1
set file="vqc10"
REM set file="vqc10a"
REM set file="vqc10_porttest"
REM set file="pio_led"
REM set file="pio_led1"
REM set file="led_counter"
REM if "%1"=="" goto hilfe
set bin=d:\Projekte\DDR\LC80ex\Software\aswcurr\bin\
echo 1.
%bin%\asw.exe -L "%file%.asm" -i d:\Projekte\DDR\LC80ex\Software\aswcurr\include
REM Weiter wenn kein Fehler:
if errorlevel 1 goto Ende
echo Assemblieren erfolgreich!
echo "> Erzeuge Intel-Hex-File:"
%bin%\p2hex.exe -a -F Intel -r $-$ "%file%.p"
echo "> Erzeuge Bin-File:"
%bin%\p2bin.exe -r $-$ "%file%.p"
del "%file%.p"
REM bdiff "%file%.rom" "%file%.bin"
echo 3.
REM Aufruf EPROM-Emulator ohne GUI:
REM usage: EPROM_EMU_NG_2.0rc10.py [-h] [-mem <type>] [-spi y/n] [-auto y/n] [-start <start>] [-map y/n] <file> <port>
python EPROM_EMU_NG_2.0rc10.py -mem 2716 -spi y -auto y %file%.bin com3

goto Ende
:hilfe
echo Aufruf: as.bat file (ohne Endung .asm)
echo erzeugt aus file.asm Datei file.bin
:Ende
REM pause

