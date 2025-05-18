// Configuration des broches pour l'?cran LCD
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

// Configuration des LEDs

sbit LED_JAUNE at RC0_bit;
sbit LED_BLEUE at RC1_bit;
sbit LED_ROUGE at RC2_bit;
sbit LED_VERTE at RC3_bit;

sbit LED_JAUNE_Direction at TRISC0_bit;
sbit LED_BLEUE_Direction at TRISC1_bit;
sbit LED_ROUGE_Direction at TRISC2_bit;
sbit LED_VERTE_Direction at TRISC3_bit;

// Configuration du moteur et du buzzer
sbit MOTOR1 at RA1_bit;
sbit MOTOR2 at RA2_bit;
sbit BUZZER at RA0_bit;

sbit MOTOR1_Direction at TRISA1_bit;
sbit MOTOR2_Direction at TRISA2_bit;
sbit BUZZER_Direction at TRISA0_bit;

// Configuration du bouton de d?marrage
sbit RESERVE_BUTTON at RB0_bit;
sbit RESERVE_BUTTON_Direction at TRISB0_bit;

sbit BUTTON_RB6 at RB6_bit;
sbit BUTTON_RB6_Direction at TRISB6_bit;

sbit ENTREZ_BUTTON at RB4_bit;
sbit ENTREZ_BUTTON_Direction at TRISB4_bit;

sbit SORTIE_BUTTON at RB5_bit;
sbit SORTIE_BUTTON_Direction at TRISB5_bit;
// Configuration du bouton de r?servation
sbit RESERV_BUTTON at RA4_bit;
sbit RESERV_BUTTON_Direction at TRISA4_bit;

int reserv_confirm_step = 0; // Variable pour suivre l'?tat de la r?servation

     int a=0;
      int b=0;
      int cc=0;
      int dd=0;
      int ee=0;
      int ff=0;
      int click=0;
      int mm=0;
      int nbcar=0;
          unsigned int adc_value;
    int charging_rate;
    char txt[16];
    char tx[16];
        int reserve=0;
        int BORN1=0;
        int BORN2=0;
        int BORN3=0;
          int test =0;
        int stock = 0;
        char txtstock[10]  ;
                int ii=0;
                int sortie=0;


// Routine d'interruption
void interrupt() {
        Lcd_Init();

         // Gestion de l'interruption du Timer0


    if (INTCON.INTF == 1) {
        INTCON.INTF = 0;
        INTCON.T0IE=1;
        OPTION_REG.T0CS = 1;

        TMR0 = 254;
                b=1;



    }


     if (INTCON.T0IF) {
        INTCON.T0IF = 0;
        TMR0 = 254;

        // V?rifier si le bouton de r?servation est press?

               a=1;


            }



    if (INTCON.RBIF)
    {
    INTCON.RBIF = 0;

         if (PORTB.RB4 && (BORN1==1 || BORN2==2 || BORN3==3)){
            click=0;
          if (cc == 1)
           dd=1;
          }
        if (PORTB.RB5 ){
            click=0;
            if (ee==1){
            ee=0;
            ff=1;
            }


        }
        if (PORTB.RB6 ){
        click=1;


        }



    }


}

void main() {

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);




    // Configuration d
    RESERVE_BUTTON_Direction = 1;
    BUTTON_RB6_Direction=1;
    ENTREZ_BUTTON_Direction=1;
    SORTIE_BUTTON_Direction=1;

    // Initialisation du Timer0
    OPTION_REG.T0CS = 1;

    TMR0 = 254;
    // Configuration des interruptions
    INTCON.GIE = 1;

    INTCON.RBIE = 1;
    OPTION_REG.INTEDG =1;
    INTCON.INTE = 1;

    //INTCON.INTF = 0;






    LED_JAUNE_Direction = 0;
    LED_BLEUE_Direction = 0;
    LED_ROUGE_Direction = 0;
    LED_VERTE_Direction = 0;

    MOTOR1_Direction = 0;
    MOTOR2_Direction = 0;
    BUZZER_Direction = 0;

    LED_BLEUE = 0;
    LED_ROUGE = 0;
    LED_VERTE = 0;
    LED_JAUNE = 0;
    MOTOR1 = 0;
    MOTOR2 = 0;
    BUZZER = 0;
    BUTTON_RB6=0;

         //  Lcd_Out(1, 1, "Bienvenue au");
        //Lcd_Out(2, 1, "kiosque");


    //ADC_Init();

    while (1) {

           if (TMR0==255 ){

                    Lcd_Cmd(_LCD_CLEAR);
                Lcd_Out(1, 1, "confirmez");
                Lcd_Out(2, 1, "re");

                Delay_ms(100);
                Lcd_Cmd(_LCD_CLEAR);


           }
           if (a==1){

                       Lcd_Cmd(_LCD_CLEAR);
                Lcd_Out(1, 1, "Reservation");
                Lcd_Out(2, 1, "confirmee");

                Delay_ms(100);
                  Lcd_Cmd(_LCD_CLEAR);
                  a=0;
                  cc=1;

           }
           if (b==1){

                  b=0;
                  if (reserve> 3)
             {
                      reserve--;
                  Lcd_Cmd(_LCD_CLEAR);
                  Lcd_Out(1, 1, "NON Disponible ");
                  Delay_ms(100);
                  Lcd_Cmd(_LCD_CLEAR);

             }
          else  if (reserve <=3)
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

         if (dd==1){
         dd=0;
         cc=0;
         ee=1;
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
          click=0;
         }

         if (ff == 1){
          ff=0;
               sortie=1;
            stock++;
            reserve--;
          if (BORN1 == 1) {

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
          click=0;
         }
         
          if (click==1 ){
              click=0;
          Lcd_Cmd(_LCD_CLEAR);
            EEPROM_Write(0x15,stock);
                nbcar = EEPROM_Read(0x15);
                 IntToStr(nbcar, tx);

                     Lcd_Out(1, 2, "nbr:");
                      Lcd_Out(2, 4, tx);
                      delay_ms(100);
                         Lcd_Cmd(_LCD_CLEAR);
         }





       /* LED_JAUNE = 1;
        Delay_ms(100);
        LED_JAUNE = 0;
        Delay_ms(100);
          */



    }
}