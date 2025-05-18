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


sbit RESERVE_BUTTON at RA4_bit;
sbit RESERVE_BUTTON_Direction at TRISA4_bit;

 
unsigned char buttonPressed = 0;


void interrupt() {

    if (INTCON.T0IF) {
        INTCON.T0IF = 0;
        TMR0 = 254;


        if (buttonPressed == 1 && PORTA.RA4 == 1) {
            Delay_ms(100);


                Lcd_Cmd(_LCD_CLEAR);
                Lcd_Out(1, 1, "Reservation");
                Lcd_Out(2, 1, "Confirmee");

                buttonPressed = 0;

        }
    }
}

void main() {

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);


    TRISA = 0x10;
    PORTA = 0x00;


    INTCON = 0xA0;
    OPTION_REG.T0CS = 1;
    OPTION_REG.T0SE = 1;
    TMR0 = 254;


        if (buttonPressed == 0 && PORTA.RA4 == 1) {


                Lcd_Cmd(_LCD_CLEAR);
                Lcd_Out(1, 1, "Confirmer ");
                Lcd_Out(2, 1, "reservation ");

                buttonPressed = 1;

        }

    while (1) {


    }
}
