#TARGET     Simulated_Z80       ; Determines hardware support included
; ToDo:
; - Test RAM/Stack 
        ORG 8000h
; Text zur Anzeige auf Display: 
TEXT:       
;         DB "Rosi und Andi !!",0
;         DB "Z80 mit 4x VQC10"
;          DB " 4444           "
;          DB "1234567890&#$&()"
          DB "ABCDEFGHIJKLMNOP"
;          DB "QRSTUVWXYZabcdef"
;          DB "ghijklmnopqrstux"
; ------------------------------------------------------------
;        CPU Z80
        ORG 8100h
; SimulIDE:
; 1024 Byte ROM: 0000h-03FFh
;  512 Byte RAM: 8000h-84FFh
;        ld sp,8400h ; Init Stackpointer
; LC80:?
; 1024 Byte ROM: 000h-3FFh
; 1024 Byte RAM: 400h-7FFh
        ; ld sp,0600h ; Init Stackpointer

; z80 out adr. 0 und 1
    

port1:  EQU  000h          ;LED port address
port2:  EQU  001h          ;LED port address
init:   
        ld a,7
        out (port1),a           ; Deaktivierung Zeilendekoder:
        ld a,16                 ; ein Digit weiter als vorhanden
        out (port2),a           ; Digitdecoder aus

START1
        ; alle Portpins auf 0 setzen:
        ld b,0                  ; Zeilenzaehler / 16 bit Tmp.
        ld c,0                  ; Zeilenzaehler
;        ld d,0                 ; Daten aller 5 spalten einer Zeile (5 Bit)
START2:
        ld e,0                  ; Digit-Zaehler/-Adresse
        
digits: ; fuer alle Stellen:
        ; hole Daten zur Anzeige:
        ; mul hl,e   ; Positioniere Digit im Zeichengenerator
        ld hl,TEXT
        ld d,0
        add hl,de               ; Zeichencode von aktuellem Digit holen
        ld a,(hl)
        sub a,20h               ; Offset wegen fehlender Fonts am Anfang des Zeichengenerators
        PUSH BC
        PUSH DE
        ; E * D -> HL (16 Bit)
        LD D,7 
        LD E,A
        CALL MULTIP 
        LD BC,FONT
        ADD HL,BC
        POP DE
        POP BC
        ld b,0
        ld d,0
        add hl,bc               ; Offset zu aktueller Zeile im Zeichengenerator
        ld a,(hl)               ; Hole Daten aus Zeichengenerator
        sla a                   ; bits 3 stellen nach links
        sla a                   ; bits 3 stellen nach links
        sla a                   ; bits 3 stellen nach links
        or 7                    ; alle Zeilen aus
        out (port1),a           ; Daten Zeile ausgeben
        ld d,a                  ; Zeilendaten sichern
        
        ; Adr. vom Digit ausgeben:
        ld a,e
        out (port2),a
        ; Deaktivierung Stellendekoder zur Datenbernahme ueber L/H-Flanke
        ld a,16
        out (port2),a

        inc e
        ld a,e
        cp 16                ; Anzahl der Digits (Ende Digit-Schleife)
        jr nz,digits

        ; Aktivierung Zeilendekoder:
        ld a,d
        and 11111000b
        or c                ; Bit 0-2 HIGH
        out (port1),a
        ; Kurz Warten zur Darstellung der aktuellen Zeile:
        call DELAY
        ; Deaktivierung Zeilendekoder:
        ld a,7
        out (port1),a

        inc c
        ld a,c
        cp 7                ; Teste Zeilenzaehler
        jr nz,START2
        jp START1
; ------------------------------------------------------------
; Unterprogramm DELAY
; Eingabeparameter in Register B - Verzögerungszeit 0FFh ca. 1 Sekunde
DELAY0: 
        PUSH HL             ; Preserve HL
        PUSH DE             ; Preserve DE
        PUSH BC             ; Preserve BC
        LD   DE,1           ; Delay time = X ms
        LD   C,0Ah          ; API 0x0A
        RST  30h            ;  = Delay by DE ms
        POP  BC             ; Restore BC
        POP  DE             ; Restore DE
        POP  HL             ; Restore HL
        RET
; ------------------------------------------------------------
DELAY: 
        LD      A,0FFH
LOOP3:  NOP
        DEC     A
        JR      NZ,LOOP3
        RET           
; ------------------------------------------------------------
MULTIP: 
        ; E * D -> HL (16 Bit)
        LD HL, 0     ; Ergebnis initialisieren
        LD A, D      ; A = Multiplikator kopieren
        OR A         ; Ist A == 0?
        RET Z        ; Ja -> Ergebnis 0
        LD B, A      ; B = Schleifenzähler (Multiplikator)
        LD A, E      ; DE = Multiplikand (E in DE erweitern)
        LD D, 0
MUL_LOOP:
        ADD HL, DE   ; HL += DE
        DJNZ MUL_LOOP ; B-- und wiederholen
        RET          ; Fertig, HL = E * D
; ------------------------------------------------------------
; Text zur Anzeige auf Display: 
TEXT2:       
;         DB "Rosi und Andi !!",0
         DB "Z80 mit 4x VQC10"
;          DB " 4444           "
;          DB "1234567890&#$&()"
;          DB "ABCDEFGHIJKLMNOP"
;          DB "QRSTUVWXYZabcdef"
;          DB "ghijklmnopqrstux"
; ------------------------------------------------------------
; Tabelle für ASCII-Zeichencodes: 
; Definiere Zeichencodes:
FONT:
    DB 00h,00h,00h,00h,00h,00h,00h ; 20h: Space
    DB 04h,04h,04h,04h,04h,00h,04h ; !
    DB 0Ah,0Ah,0Ah,00h,00h,00h,00h ; "
    DB 0Ah,0Ah,1Fh,0Ah,1Fh,0Ah,0Ah ; #
    DB 04h,0Fh,14h,0Eh,05h,1Eh,04h ; $
    DB 18h,19h,02h,04h,08h,13h,03h ; %
    DB 0Ch,12h,14h,08h,15h,12h,0Dh ; &
    DB 0Ch,04h,08h,00h,00h,00h,00h ; '
    DB 02h,04h,08h,08h,08h,04h,02h ; (
    DB 08h,04h,02h,02h,02h,04h,08h ; )
    DB 00h,0Ah,04h,1Fh,04h,0Ah,00h ; *
    DB 00h,04h,04h,1Fh,04h,04h,00h ; +
    DB 00h,00h,00h,00h,0Ch,04h,08h ; ,
    DB 00h,00h,00h,1Fh,00h,00h,00h ; -
    DB 00h,00h,00h,00h,00h,0Ch,0Ch ; .
    DB 00h,01h,02h,04h,08h,10h,00h ; /
    DB 0Eh,11h,13h,15h,19h,11h,0Eh ; 0
    DB 04h,0Ch,04h,04h,04h,04h,0Eh ; 1
    DB 0Eh,11h,01h,02h,04h,08h,1Fh ; 2
    DB 1Fh,02h,04h,02h,01h,11h,0Eh ; 3
    DB 02h,06h,0Ah,12h,1Fh,02h,02h ; 4
    DB 1Fh,10h,1Eh,01h,01h,11h,0Eh ; 5
    DB 06h,08h,10h,1Eh,11h,11h,0Eh ; 6
    DB 1Fh,01h,02h,04h,08h,08h,08h ; 7
    DB 0Eh,11h,11h,0Eh,11h,11h,0Eh ; 8
    DB 0Eh,11h,11h,0Fh,01h,02h,0Ch ; 9
    DB 00h,0Ch,0Ch,00h,0Ch,0Ch,00h ; :
    DB 00h,0Ch,0Ch,00h,0Ch,04h,08h ; ;
    DB 01h,02h,04h,08h,04h,02h,01h ; <
    DB 00h,00h,1Fh,00h,1Fh,00h,00h ; =
    DB 10h,08h,04h,02h,04h,08h,10h ; >
    DB 0Eh,11h,01h,02h,04h,00h,04h ; ?
    DB 0Eh,11h,01h,0Dh,15h,15h,0Eh ; @
    DB 0Eh,11h,11h,11h,1Fh,11h,11h ; A
    DB 1Eh,11h,11h,1Eh,11h,11h,1Eh ; B
    DB 0Eh,11h,10h,10h,10h,11h,0Eh ; C
    DB 1Ch,12h,11h,11h,11h,12h,1Ch ; D
    DB 1Fh,10h,10h,1Eh,10h,10h,1Fh ; E
    DB 1Fh,10h,10h,1Ch,10h,10h,10h ; F
    DB 0Eh,11h,10h,10h,13h,11h,0Eh ; G
    DB 11h,11h,11h,1Fh,11h,11h,11h ; H
    DB 0Eh,04h,04h,04h,04h,04h,0Eh ; I
    DB 07h,02h,02h,02h,02h,12h,0Ch ; J
    DB 11h,12h,14h,18h,14h,12h,11h ; K
    DB 10h,10h,10h,10h,10h,10h,1Fh ; L
    DB 11h,1Bh,15h,11h,11h,11h,11h ; M
    DB 11h,11h,19h,15h,13h,11h,11h ; N
    DB 0Eh,11h,11h,11h,11h,11h,0Eh ; O
    DB 1Eh,11h,11h,1Eh,10h,10h,10h ; P
    DB 0Eh,11h,11h,11h,15h,12h,0Dh ; Q
    DB 1Eh,11h,11h,1Eh,14h,12h,11h ; R
    DB 0Fh,10h,10h,0Eh,01h,01h,1Eh ; S
    DB 1Fh,04h,04h,04h,04h,04h,04h ; T
    DB 11h,11h,11h,11h,11h,11h,0Eh ; U
    DB 11h,11h,11h,11h,11h,0Ah,04h ; V
    DB 11h,11h,11h,15h,15h,1Bh,11h ; W
    DB 11h,11h,0Ah,04h,0Ah,11h,11h ; X
    DB 11h,11h,0Ah,04h,04h,04h,04h ; Y
    DB 1Fh,01h,02h,04h,08h,10h,1Fh ; Z
    DB 07h,04h,04h,04h,04h,04h,07h ; [
    DB 00h,10h,08h,04h,02h,01h,00h ; \
    DB 1Ch,04h,04h,04h,04h,04h,1Ch ; ]
    DB 04h,0Ah,11h,00h,00h,00h,00h ; ^
    DB 00h,00h,00h,00h,00h,00h,1Fh ; _
    DB 08h,04h,02h,00h,00h,00h,00h ; `
    DB 00h,00h,0Eh,01h,0Fh,11h,0Fh ; a
    DB 10h,10h,16h,19h,11h,11h,1Eh ; b
    DB 00h,00h,0Eh,10h,10h,11h,0Eh ; c
    DB 01h,01h,0Dh,13h,11h,11h,0Fh ; d
    DB 00h,00h,0Eh,11h,1Fh,10h,0Eh ; e
    DB 06h,09h,08h,1Ch,08h,08h,08h ; f
    DB 00h,00h,0Fh,11h,0Fh,01h,06h ; g
    DB 10h,10h,16h,19h,11h,11h,11h ; h
    DB 04h,00h,0Ch,04h,04h,04h,0Eh ; i
    DB 02h,00h,06h,02h,02h,12h,0Ch ; j
    DB 08h,08h,09h,0Ah,0Ch,0Ah,09h ; k
    DB 0Ch,04h,04h,04h,04h,04h,0Eh ; l
    DB 00h,00h,1Ah,15h,15h,11h,11h ; m
    DB 00h,00h,16h,19h,11h,11h,11h ; n
    DB 00h,00h,0Eh,11h,11h,11h,0Eh ; o
    DB 00h,00h,1Eh,11h,1Eh,10h,10h ; p
    DB 00h,00h,0Dh,13h,0Fh,01h,01h ; q
    DB 00h,00h,16h,19h,10h,10h,10h ; r
    DB 00h,00h,0Eh,10h,0Eh,01h,1Eh ; s
    DB 08h,08h,1Ch,08h,08h,09h,06h ; t
    DB 00h,00h,11h,11h,11h,13h,0Dh ; u
    DB 00h,00h,11h,11h,11h,0Ah,04h ; v
    DB 00h,00h,11h,11h,15h,15h,0Ah ; w
    DB 00h,00h,11h,0Ah,04h,0Ah,11h ; x
    DB 00h,00h,11h,11h,0Fh,01h,0Eh ; y
    DB 00h,00h,1Fh,02h,04h,08h,1Fh ; z
    DB 02h,04h,04h,08h,04h,04h,02h ; {
    DB 04h,04h,04h,04h,04h,04h,04h ; |
    DB 08h,04h,04h,02h,04h,04h,08h ; }









