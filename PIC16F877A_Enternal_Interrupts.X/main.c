/*
 * File:   main.c
 * Author: HASAN
 *
 * Created on December 15, 2020, 6:02 PM
 */


#include <xc.h>
#include "16F_external_int_xc8_header.h"
#include "lcd.h"

int number;
char ch1, ch2, ch3, ch4;

/****Interrupt function ****/
void __interrupt() ISR_example()
{
        if (INTF==1) //External Interrupt detected
        { 
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Print_String("Enter Interrupt");
            Lcd_Set_Cursor(2,1);
            Lcd_Print_String("Service Routine");
            INTF = 0;          // clear the interrupt flag after done with it
           __delay_ms(5000);
            Lcd_Clear();
        }
}

void main(void) {
    TRISD = 0x00; //PORTD declared as output for interfacing LCD
    TRISBbits.TRISB0 = 1; //Define the RB0 pin as input to use as interrupt pin
    
    OPTION_REG = 0b00000000;  // Enables PULL UPs at PORTB
    INTCONbits.GIE = 1; // Enable Global Interrupt
    INTCONbits.PEIE = 1; // Enable the Peripheral Interrupt
    INTCONbits.INTE = 1; // Enable RB0 as external Interrupt pin
    
    Lcd_Start();
    
    while(1){
        ch1 = (number/1000)%10;
        ch2 = (number/100)%10;
        ch3 = (number/10)%10;
        ch4 = (number/1)%10;
        
        Lcd_Set_Cursor(2,1);
        Lcd_Print_String("Inside Main Loop");
        Lcd_Set_Cursor(1,1);
        Lcd_Print_String("Number: ");
        Lcd_Print_Char(ch1+'0');
        Lcd_Print_Char(ch2+'0');
        Lcd_Print_Char(ch3+'0');
        Lcd_Print_Char(ch4+'0');
        
        __delay_ms(500);
        number++;
    }
    return;
}
