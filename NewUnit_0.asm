
_interrupt:
	MOVWF      R15+0
	SWAPF      STATUS+0, 0
	CLRF       STATUS+0
	MOVWF      ___saveSTATUS+0
	MOVF       PCLATH+0, 0
	MOVWF      ___savePCLATH+0
	CLRF       PCLATH+0

;NewUnit_0.c,5 :: 		void interrupt() {
;NewUnit_0.c,6 :: 		if (INTCON.RBIF)
	BTFSS      INTCON+0, 0
	GOTO       L_interrupt0
;NewUnit_0.c,8 :: 		INTCON.RBIF = 0;
	BCF        INTCON+0, 0
;NewUnit_0.c,9 :: 		if (PORTB.RB3){
	BTFSS      PORTB+0, 3
	GOTO       L_interrupt1
;NewUnit_0.c,10 :: 		clicksaat=1;
	MOVLW      1
	MOVWF      _clicksaat+0
	MOVLW      0
	MOVWF      _clicksaat+1
;NewUnit_0.c,11 :: 		}
L_interrupt1:
;NewUnit_0.c,12 :: 		}
L_interrupt0:
;NewUnit_0.c,13 :: 		}
L_end_interrupt:
L__interrupt7:
	MOVF       ___savePCLATH+0, 0
	MOVWF      PCLATH+0
	SWAPF      ___saveSTATUS+0, 0
	MOVWF      STATUS+0
	SWAPF      R15+0, 1
	SWAPF      R15+0, 0
	RETFIE
; end of _interrupt

_main:

;NewUnit_0.c,15 :: 		void main (){
;NewUnit_0.c,16 :: 		Lcd_Init();
	CALL       _Lcd_Init+0
;NewUnit_0.c,17 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOVLW      1
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;NewUnit_0.c,18 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);
	MOVLW      12
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;NewUnit_0.c,19 :: 		while (1) {
L_main2:
;NewUnit_0.c,20 :: 		if (clicksaat == 1){
	MOVLW      0
	XORWF      _clicksaat+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main9
	MOVLW      1
	XORWF      _clicksaat+0, 0
L__main9:
	BTFSS      STATUS+0, 2
	GOTO       L_main4
;NewUnit_0.c,21 :: 		clicksaat=0;
	CLRF       _clicksaat+0
	CLRF       _clicksaat+1
;NewUnit_0.c,22 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOVLW      1
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;NewUnit_0.c,23 :: 		EEPROM_Write(0x50,stocka);
	MOVLW      80
	MOVWF      FARG_EEPROM_Write_Address+0
	MOVF       _stocka+0, 0
	MOVWF      FARG_EEPROM_Write_data_+0
	CALL       _EEPROM_Write+0
;NewUnit_0.c,24 :: 		nbcara = EEPROM_Read(0x50);
	MOVLW      80
	MOVWF      FARG_EEPROM_Read_Address+0
	CALL       _EEPROM_Read+0
	MOVF       R0+0, 0
	MOVWF      _nbcara+0
	CLRF       _nbcara+1
;NewUnit_0.c,25 :: 		IntToStr(nbcara, txta);
	MOVF       _nbcara+0, 0
	MOVWF      FARG_IntToStr_input+0
	MOVF       _nbcara+1, 0
	MOVWF      FARG_IntToStr_input+1
	MOVLW      _txta+0
	MOVWF      FARG_IntToStr_output+0
	CALL       _IntToStr+0
;NewUnit_0.c,27 :: 		Lcd_Out(1, 1, "nbr:");
	MOVLW      1
	MOVWF      FARG_Lcd_Out_row+0
	MOVLW      1
	MOVWF      FARG_Lcd_Out_column+0
	MOVLW      ?lstr1_NewUnit_0+0
	MOVWF      FARG_Lcd_Out_text+0
	CALL       _Lcd_Out+0
;NewUnit_0.c,28 :: 		Lcd_Out(2, 4, txta);
	MOVLW      2
	MOVWF      FARG_Lcd_Out_row+0
	MOVLW      4
	MOVWF      FARG_Lcd_Out_column+0
	MOVLW      _txta+0
	MOVWF      FARG_Lcd_Out_text+0
	CALL       _Lcd_Out+0
;NewUnit_0.c,29 :: 		delay_ms(100);
	MOVLW      2
	MOVWF      R11+0
	MOVLW      4
	MOVWF      R12+0
	MOVLW      186
	MOVWF      R13+0
L_main5:
	DECFSZ     R13+0, 1
	GOTO       L_main5
	DECFSZ     R12+0, 1
	GOTO       L_main5
	DECFSZ     R11+0, 1
	GOTO       L_main5
	NOP
;NewUnit_0.c,30 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOVLW      1
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;NewUnit_0.c,31 :: 		}
L_main4:
;NewUnit_0.c,32 :: 		}
	GOTO       L_main2
;NewUnit_0.c,33 :: 		}
L_end_main:
	GOTO       $+0
; end of _main
