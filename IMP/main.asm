;**************************************************************
;*                                                            *
;* IMP - Project n. 6 - Cyclic Redundancy Check (CRC)         *
;* Author: Petr Zemek, xzemek02@stud.fit.vutbr.cz             *
;* Description: This program can secure received data by      *
;*              computing it's CRC and appending it           *
;*              to the original data.                         *
;*              Generating polynom is: x^3 + x + 1 (1011)b    *
;*                                                            *
;**************************************************************

; Export symbols
                  XDEF Entry        ; Export 'Entry' symbol
                  ABSENTRY Entry    ; Application entry point

; Include derivative specific macros
                  Include 'lj12_registers.inc'

; Interrupts vectors start
VectorStart       EQU  $FFDA

; Variable/data section
                  ORG  RAMStart
; Variables (names) from the assignment
LENGTH_RECEIVED   DS   1         ; Number of received bytes
DATA_RECEIVED		  DS   3         ; Unsecured data
DATA_TOSEND			  DS   6         ; Secured data
; My own variables						 
PROCESSED_BYTES   DS   1         ; Number of secured bytes
PROCESSED_NIBBLES DS   1         ; Number of secured nibbles

; Code section
                ORG ROMStart

;**************************************************************
;* Entry - Application entry point                            *
;**************************************************************
Entry:
        MOV   #$01,CONFIG1    ; Disable COP
        CLI                   ; Enable interrupts

        ; Example start
        ; Data
        MOV   #%00001111,DATA_RECEIVED
        MOV   #%10010001,DATA_RECEIVED+1
        MOV   #%01111110,DATA_RECEIVED+2
        ; Data length
        MOV   #3,LENGTH_RECEIVED
        ; Example end
        
main_loop:
        RSP				              ; SP,A,H,X initialization
        CLRA                 
        CLRX
        CLRH
        MOV   0,PROCESSED_BYTES   ; We haven't processed anything yet
        MOV   0,PROCESSED_NIBBLES

crc_loop:
        LDA   LENGTH_RECEIVED   ; Load number of received bytes
        CMP   PROCESSED_BYTES   ; Have we processed everything?
        BEQ   crc_loop          ; Yes -> loop until there are more data to process
        
        ; First nibble
        LDX   PROCESSED_BYTES   ; Load next byte into A
        LDA   DATA_RECEIVED,X 
        AND   #%11110000        ; Get the first nibble of the byte
        PSHA                    ; Backup it onto stack for later usage
        BSR   compute_crc
        ORA   1,SP						  ; Append the computed CRC to A
        LDX   PROCESSED_NIBBLES ; Load the offset where we will store the result
        STA   DATA_TOSEND,X     ; Store the result into memory
        PULA                    ; Cleanup

        BSR   increase_nibbles
				
				; Second nibble
				LDX   PROCESSED_BYTES   ; Load the current byte into A (again)
				LDA   DATA_RECEIVED,X 
        LSLA                    ; Get the second nibble of the byte...
        LSLA                    ; ...by shifting A four times
        LSLA
        LSLA
        PSHA                    ; Backup it onto stack for later usage
        BSR   compute_crc                        
			  ORA   1,SP					  	; "Append" the computed CRC to A
			  LDX   PROCESSED_NIBBLES ; Load the offset where we will store the result
        STA   DATA_TOSEND,X     ; Store the result into memory
        PULA                    ; Cleanup
        
        BSR   increase_nibbles
        BSR   increase_bytes
                   
        BRA   crc_loop       
        BRA   main_loop				  ; Program should never get there...

; Increases number of processed bytes
increase_bytes:
        PSHX                    ; Backup of X
        LDX		PROCESSED_BYTES   ; Increase number of processed bytes
        INCX
        STX   PROCESSED_BYTES
        PULX										; Restoration of X
        RTS

; Increases number of processed nibbles
increase_nibbles:
        PSHX                    ; Backup of X
        LDX		PROCESSED_NIBBLES ; Increase number of processed bytes
        INCX
        STX   PROCESSED_NIBBLES
        PULX										; Restoration of X
        RTS

; Computes CRC from the byte stored in A.
; Result will be stored in A.
compute_crc:                  ; bit 1       
        PSHA                  ; Backup A
        AND   #%10000000			; Test if the 1st bit is 1
        PULA                  ; Restore A
        BEQ		compute_crc_2   ; It's not, skip this bit
        EOR   #%10110000  		; Perform A xor (shifted) GP
        
compute_crc_2:                ; bit 2        
        PSHA                  ; Backup A
        AND   #%01000000			; Test if the 2nd bit is 1
        PULA									; Restore A
        BEQ		compute_crc_3   ; It's not, skip this bit
        EOR   #%01011000  		; Perform A xor (shifted) GP
        
compute_crc_3:                ; bit 3        
        PSHA                  ; Backup A
        AND   #%00100000			; Test if the 3rd bit is 1
        PULA									; Restore A
        BEQ		compute_crc_4   ; It's not, skip this bit
        EOR   #%00101100  		; Perform A xor (shifted) GP

compute_crc_4:                ; bit 4        
        PSHA                  ; Backup A
        AND   #%00010000			; Test if the 4th bit is 1
        PULA									; Restore A
        BEQ		compute_crc_end ; It's not, skip this bit
        EOR   #%00010110  		; Perform A xor (shifted) GP
        
compute_crc_end       
        RTS

;**************************************************************
;* spurious - Spurious Interrupt Service Routine.             *
;*             (unwanted interrupt)                           *
;**************************************************************
spurious:
        NOP
        RTI

;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
        ORG  VectorStart
        ; Routine        ; Description                ;Address
        fdb  spurious    ; Real-time Clock Vector     ;$FFDA
        fdb  spurious    ; ADC Conversion Complete    ;$FFDC
        fdb  spurious    ; Keyboard Vector            ;$FFDE
        fdb  spurious    ; SCI Transmit Vector        ;$FFE0
        fdb  spurious    ; SCI Receive Vector         ;$FFE2
        fdb  spurious    ; SCI Error Vector           ;$FFE4
        fdb  spurious    ; SPI Receive Vector         ;$FFE6
        fdb  spurious    ; SPI Transmit Vector        ;$FFE8
        fdb  spurious    ; TIM2 Overflow Vector       ;$FFEA
        fdb  spurious    ; TIM2 Channel 1 Vector      ;$FFEC
        fdb  spurious    ; TIM2 Channel 0 Vector      ;$FFEE
        fdb  spurious    ; TIM1 Overflow Vector       ;$FFF0
        fdb  spurious    ; TIM1 Channel 1 Vector      ;$FFF2
        fdb  spurious    ; TIM1 Channel 0 Vector      ;$FFF4
        fdb  spurious    ; PLL Vector                 ;$FFF6
        fdb  spurious    ; LVI Vector                 ;$FFF8
        fdb  spurious    ; ~IRQ1 Vector               ;$FFFA
        fdb  spurious    ; SWI Vector                 ;$FFFC
        fdb  Entry       ; Reset Vector               ;$FFFE
