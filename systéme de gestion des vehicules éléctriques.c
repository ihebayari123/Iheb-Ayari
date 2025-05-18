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

sbit ENTREZ_BUTTON at RB4_bit;
sbit ENTREZ_BUTTON_Direction at TRISB4_bit;

sbit SORTIE_BUTTON at RB5_bit;
sbit SORTIE_BUTTON_Direction at TRISB5_bit;
// Configuration du bouton de réservation
sbit RESERV_BUTTON at RA4_bit;
sbit RESERV_BUTTON_Direction at TRISA4_bit;

int reserv_confirm_step = 0; // Variable pour suivre l'état de la réservation


          unsigned int adc_value;
    int charging_rate;
    char txt[16];
        int reserve=0;
        int BORN1=0;
        int BORN2=0;
        int BORN3=0;
          int test =0;

                int ii=0;


// Routine d'interruption
void interrupt() {
        Lcd_Init();

         // Gestion de l'interruption du Timer0


    if (INTCON.INTF == 1) {
        INTCON.INTF = 0;


         if (reserve> 3)
             {

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


     if (INTCON.T0IF) {
        INTCON.T0IF = 0; // Réinitialiser le flag d'interruption de Timer0
        TMR0 = 252;      // Réinitialiser la valeur de Timer0

        // Vérifier si le bouton de réservation est pressé
        if (RESERV_BUTTON == 0) {
            if (reserv_confirm_step == 0) {
                // Étape 1 : Afficher le message de confirmation
                Lcd_Cmd(_LCD_CLEAR);
                Lcd_Out(1, 1, "Confirmer");
                Lcd_Out(2, 1, "reservation");
                reserv_confirm_step = 1;

            } else if (reserv_confirm_step == 1) {
                // Étape 2 : Afficher le message de réservation confirmée
                Lcd_Cmd(_LCD_CLEAR);
                Lcd_Out(1, 1, "Reservation");
                Lcd_Out(2, 1, "confirmee");
                reserv_confirm_step = 0; // Réinitialiser pour la prochaine réservation

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


}

void main() {
    // Initialisation de l'?cran LCD
   // Lcd_Init();
  //  Lcd_Cmd(_LCD_CLEAR);
   // Lcd_Cmd(_LCD_CURSOR_OFF);

        // Initialisation de l'écran LCD


    // Configuration des directions des broches
    RESERVE_BUTTON_Direction = 1; // Entrée pour le bouton de réservation

    // Initialisation du Timer0
    OPTION_REG.T0CS = 0; // Mode timer (horloge interne)
    OPTION_REG.PSA = 0;  // Prescaler activé
    OPTION_REG.PS2 = 1;  // Prescaler = 1:256
    OPTION_REG.PS1 = 1;
    OPTION_REG.PS0 = 1;
    TMR0 = 252;          // Valeur initiale pour obtenir un délai spécifique
    // Configuration des interruptions
    INTCON.GIE = 1;
     INTCON.T0IE = 1;     // Activer l'interruption du Timer0
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

         //  Lcd_Out(1, 1, "Bienvenue au");
        //Lcd_Out(2, 1, "kiosque");


    //ADC_Init();

    while (1) {


       /* LED_JAUNE = 1;
        Delay_ms(100);
        LED_JAUNE = 0;
        Delay_ms(100);
          */



    }
}