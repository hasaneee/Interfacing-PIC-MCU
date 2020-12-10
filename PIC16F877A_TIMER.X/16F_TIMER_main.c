/*
 * File:   16F_TIMER_main.c
 * Author: HASAN
 *
 * Created on December 7, 2020, 5:19 PM
 */

/* Timer0 Delay Calculation:
 * Delay = ((256-REG_val)*(Prescal*4))/Fosc
 * Here, REG_Val = 100 (this value can be 0-256), 
 * Prescal = 64, Fosc = 20000000MHz
 * Delay =  0.0019968s = ~2ms
*/
#include <xc.h>
#include "timer_xc8_header.h"
#include "lcd.h"
#include "stdio.h"

char count_delay = 0;
int timer_delay_count = 0;
int get_input_for_delay =0;
int time;
char buffer[20];
char flag =0;
char i=0;

void __interrupt() timer_isr()
{  
    if(TMR0IF==1) // Timer flag has been triggered due to timer overflow
    {
        TMR0 = 100;     //Load the timer Value
        TMR0IF=0;       // Clear timer interrupt flag
        timer_delay_count++;
    } 
    
    if (timer_delay_count == 250)
    {
        count_delay+=1;   // hscnd will get incremented for every half second
        timer_delay_count=0;
    }
}

void main(void) {
    TRISD = 0x00;
    Lcd_Start();
    // Port Configuration for Timer
    OPTION_REG = 0b00000101;  // Timer0 with external freq and 64 as prescalar // Also Enables PULL UPs
    TMR0=100;       // Load the time value for 0.0019968s; delayValue can be between 0-256 only
    INTCONbits.TMR0IE = 1;       //Enable timer interrupt bit in PIE1 register
    INTCONbits.GIE = 1;          //Enable Global Interrupt
    INTCONbits.PEIE = 1;         //Enable the Peripheral Interrupt
    
    // Port Configuration for I/O
    TRISB = 0x00;
    PORTB = 0x00;
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    
    while(1){
        timer_delay_count = 0;
        if(PORTCbits.RC6==0 && flag==0){
            get_input_for_delay+=1;
            flag = 1;
        }
        time = 500*get_input_for_delay;
        sprintf(buffer, "%d", time);
        Lcd_Set_Cursor(1, 1);
        Lcd_Print_String("Delay: ");
        Lcd_Set_Cursor(2, 1);
        Lcd_Print_String(buffer);
        Lcd_Set_Cursor(2, 6);
        Lcd_Print_String("MiliSeconds");
        if(PORTCbits.RC6==1) flag=0;
        
        while(PORTCbits.RC7==0){
            PORTB = 0b00000001<<i;
            if(get_input_for_delay == count_delay){
                i++;
                count_delay = 0;
            }
            flag = 2;
        }
        
        if(flag==2 && PORTCbits.RC7==1){
            count_delay = 0;
            get_input_for_delay = 0;
            //PORTB = 0x00;
            time = 0;
            buffer[0] = '\0';
            Lcd_Clear();
        }
    }
    
    return;
}
