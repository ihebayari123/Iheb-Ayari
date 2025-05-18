#line 1 "C:/Users/User/Desktop/micro/systéme de gestion des vehicules éléctriques/NewUnit_0.c"
int clicksaat=0;
char txta[50];
int nbcara=0;
int stocka=0;
void interrupt() {
 if (INTCON.RBIF)
 {
 INTCON.RBIF = 0;
 if (PORTB.RB3){
 clicksaat=1;
 }
 }
}

void main (){
 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 while (1) {
 if (clicksaat == 1){
 clicksaat=0;
 Lcd_Cmd(_LCD_CLEAR);
 EEPROM_Write(0x50,stocka);
 nbcara = EEPROM_Read(0x50);
 IntToStr(nbcara, txta);

 Lcd_Out(1, 1, "nbr:");
 Lcd_Out(2, 4, txta);
 delay_ms(100);
 Lcd_Cmd(_LCD_CLEAR);
 }
 }
}
