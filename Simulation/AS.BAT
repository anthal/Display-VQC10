@echo off
set file=%1
set bin="d:\Projekte\DDR\LC80ex\ASM\Arnold-Assembler\aswcurr\bin\"
REM %bin%\as.exe -cpu Z80 -L %file%.asm
%bin%\asl.exe -cpu Z80 -L %file%.asm
REM Weiter wenn kein Fehler:
if errorlevel 1 goto Ende
echo Assemblieren erfolgreich!
%bin%\p2bin.exe -r $-$ %file%.p
del %file%.p
rem bdiff epromm~1.bin %file%.bin
python3 conv_bin2rom.py %file%
copy %file%.data "D:\Projekte\Simulation\simulide"
:Ende

