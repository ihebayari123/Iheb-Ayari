#line 1 "C:/Users/User/Desktop/micro/systéme de gestion des vehicules éléctriques/systéme de gestion des vehicules éléctriques.c"

sbit LCD_RS at RD0_bit;
sbit LCD_EN at RD1_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISD0_bit;
sbit LCD_EN_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;


sbit LED_JAUNE at RC0_bit;
sbit LED_BLEUE at RC1_bit;
sbit LED_ROUGE at RC2_bit;
sbit LED_VERTE at RC3_bit;

sbit LED_JAUNE_Direction at TRISC0_bit;
sbit LED_BLEUE_Direction at TRISC1_bit;
sbit LED_ROUGE_Direction at TRISC2_bit;
sbit LED_VERTE_Direction at TRISC3_bit;


sbit MOTOR1 at RA1_bit;
sbit MOTOR2 at RA2_bit;
sbit BUZZER at RA0_bit;

sbit MOTOR1_Direction at TRISA1_bit;
sbit MOTOR2_Direction at TRISA2_bit;
sbit BUZZER_Direction at TRISA0_bit;


sbit RESERVE_BUTTON at RB0_bit;
sbit RESERVE_BUTTON_Direction at TRISB0_bit;

sbit ENTREZ_BUTTON at RB4_bit;
sbit ENTREZ_BUTTON_Direction at TRISB4_bit;

sbit SORTIE_BUTTON at RB5_bit;
sbit SORTIE_BUTTON_Direction at TRISB5_bit;

sbit interne_BUTTON at RA4_bit;
sbit interne_BUTTON_Direction at TRISA4_bit;
 unsigned int adc_value;
 int charging_rate;
 char txt[16];
 int reserve=0;
 int BORN1=0;
 int BORN2=0;
 int BORN3=0;
 int test =0;
 int wes=0;
 int ii=0;



void interrupt() {
 Lcd_Init();

 if (INTCON.INTF == 1) {
 INTCON.INTF = 0;


 if (reserve> 3)
 {

 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "NON Disponible ");
 Delay_ms(100);
 Lcd_Cmd(_LCD_CLEAR);

 }
 else if (reserve <=3)
 {
 reserve++;

 INTCON.RBIE=1;
 if (reserve == 1){
 BORN1=1;
 IntToStr(BORN1, txt);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Borne");
 Lcd_Out(2, 1, txt);
 Lcd_Out(2, 8, "dispo");
 Delay_ms(100);
 Lcd_Cmd(_LCD_CLEAR);
 }
 else if (reserve == 2){
 BORN2=2;
 IntToStr(BORN2, txt);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Borne");
 Lcd_Out(2, 1, txt);
 Lcd_Out(2, 8, "dispo");
 Delay_ms(100);
 Lcd_Cmd(_LCD_CLEAR);
 }

 else if (reserve == 3){
 BORN3=3;
 IntToStr(BORN3, txt);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Borne");
 Lcd_Out(2, 1, txt);
 Lcd_Out(2, 8, "dispo");
 Delay_ms(100);
 Lcd_Cmd(_LCD_CLEAR);
 }

 }

 }

 if (INTCON.RBIF)
 {
 INTCON.RBIF = 0;
 if (PORTB.RB4 && test<3 && (BORN1==1 || BORN2==2 || BORN3==3)){
 test++;
 MOTOR1=1 ;
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "ENTREZ ");

 for (ii=0;ii<3;ii++){
 LED_VERTE = 1;
 Delay_ms(50);
 LED_VERTE = 0;
 Delay_ms(50);
 }
 MOTOR1=0;


 Lcd_Cmd(_LCD_CLEAR);
 }

 if (PORTB.RB5 && 0<test ){
 test--;
 RESERVE--;
 if (BORN1 == 1) {
 RESERVE--;
 IntToStr(BORN1, txt);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "bye ");
 Lcd_Out(2, 1, "Borne :");
 Lcd_Out(2, 7, txt);
 MOTOR2=1;
 LED_VERTE = 1;
 Delay_ms(300);
 LED_VERTE = 0;
 MOTOR2=0;
 Delay_ms(50);
 Lcd_Cmd(_LCD_CLEAR);
 BORN1 =0;
 }

 else if (BORN2 == 2) {

 IntToStr(BORN2, txt);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "bye ");
 Lcd_Out(2, 1, "Borne :");
 Lcd_Out(2, 7, txt);
 MOTOR2=1;
 LED_VERTE = 1;
 Delay_ms(300);
 LED_VERTE = 0;
 MOTOR2=0;
 Delay_ms(50);
 Lcd_Cmd(_LCD_CLEAR);
 BORN2 =0;
 }

 else if (BORN3 == 3) {

 IntToStr(BORN3, txt);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "bye ");
 Lcd_Out(2, 1, "Borne :");
 Lcd_Out(2, 7, txt);
 MOTOR2=1;
 LED_VERTE = 1;
 Delay_ms(300);
 LED_VERTE = 0;
 MOTOR2=0;
 Delay_ms(50);
 Lcd_Cmd(_LCD_CLEAR);
 BORN3 =0;
 }
 }
 }

 if(intcon.t0if) {
 if (PORTA.RA4=1) {
 wes++;
 if (wes==1){

 }
 else if(wes==2){
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "reservation ");
 Lcd_Out(2, 4, "confirmee ");
 }
 }

 porta.ra0=0;

 intcon.t0if=0;

 }
}

void main() {







 INTCON.GIE = 1;
 INTCON.RBIE = 1;
 OPTION_REG.INTEDG =1;
 INTCON.INTE = 1;
 intcon.t0ie=1;
 option_reg.t0cs=1;
 option_reg.t0se=1;
 TRISA.RA4=1;




 tmr0=254;


 LED_JAUNE_Direction = 0;
 LED_BLEUE_Direction = 0;
 LED_ROUGE_Direction = 0;
 LED_VERTE_Direction = 0;

 MOTOR1_Direction = 0;
 MOTOR2_Direction = 0;
 BUZZER_Direction = 0;

 PORTA.RA4 = 0;
 LED_BLEUE = 0;
 LED_ROUGE = 0;
 LED_VERTE = 0;
 LED_JAUNE = 0;
 MOTOR1 = 0;
 MOTOR2 = 0;
 BUZZER = 0;







 if (PORTA.RA4 == 1){
 wes=1;
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Confirmer ");
 Lcd_Out(2, 4, "reservation ");
 }


 while (1) {
#line 281 "C:/Users/User/Desktop/micro/systéme de gestion des vehicules éléctriques/systéme de gestion des vehicules éléctriques.c"
 }
}
