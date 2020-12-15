/*
 * File:   main.c
 * Author: HASAN
 *
 * Created on December 14, 2020, 6:07 PM
 */


#include <xc.h>
#include "adc_pwm_xc8_header.h"
#include "stdint.h"

#define TMR2PRESCALE 4

int PWM_freq = 5000;

/*
    Fdesired = Target Frequency in Hz
    Tdesired = (1/Fdesired)
    Finput = (Fosc/4)
    Tinput = 1/(Finput)
    PR = Tdesired/(Tinput * Prescale * Postscale)

    So say for example you want a 1KHz timer from a 20MHz Fosc:
    Fdesired = 1000Hz
    Tdesired = (1/1000Hz) = 0.001s
    Finput  = (20000000Hz/4) = 5000000Hz
    Tinput = (1/5000000Hz) = 0.0000002s
    Prescale = 4
    Postscale = 6
 */

void PWM_Initialize()
{
    PR2 = 0.001/(0.0000002 * 4 * 6); //PR2 value must be 8-bit (1-256)
    CCP1M3 = 1; CCP1M2 = 1;  //Configure the CCP1CON register. Datasheet page no 66
    T2CON = 0b00101101; //Ref: PIC16F877A datasheet page no 63
    TRISCbits.TRISC2 = 0; // make port C pin 2 as output
}

void PWM2_LoadDutyValue(uint16_t dutyValue)
{
   // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR1L = ((dutyValue & 0x03FC)>>2);
    
   // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP1CON = ((uint8_t)(CCP1CON & 0xCF) | ((dutyValue & 0x0003)<<4));
}

void adc_init(){
    ADCON0 = 0b01000001;
    ADCON1 = 0b11000000;
}

unsigned int read_adc(unsigned char ch){
    ADCON0 &= 0b11000101;
    ADCON0 |= ch<<3;
    __delay_ms(2);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH<<8)+ADRESL);
}

void main(void) {
    int adc_val;
    
    PWM_Initialize();
    adc_init();
    
    while(1){
        adc_val = read_adc(3);
        PWM2_LoadDutyValue(adc_val);
    }
    
    return;
}
