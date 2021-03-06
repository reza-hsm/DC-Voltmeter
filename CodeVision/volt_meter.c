/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
? Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 12/24/2021
Author  : 
Company : 
Comments: 


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 1.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32.h>
#include <stdlib.h>
#include <stdio.h>
#include <delay.h>
#include <alcd.h>   // Alphanumeric LCD functions

#define ADCChannel 0	//Read voltage from PORTA0(ADC0)

void ADCInit(){
	ADCSRA = 0xe0;				//Enable ADC, Prescaler : 2 ,Free running mode
	ADMUX = 0x40 | ADCChannel;	//Select ADC channel
}

int readADC(){
	int ADC_value = ADCL;	//Read 8 lower bit.
	ADC_value += (ADCH & 0x03)<<8;	//Read 2 upper bit and add it to ADC_value.
	return ADC_value;
}

float readVoltage(){
	int ADC_value = readADC(); 
	float voltage_value = ADC_value/201.37; //ADC_value is between 0 and 1023.voltage_value is between 0 and 5;
	return voltage_value;
}

void main(void)
{
    // Alphanumeric LCD initialization
    // Connections are specified in the
    // Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
    // RS - PORTD Bit 0
    // RD - PORTD Bit 1
    // EN - PORTD Bit 2
    // D4 - PORTD Bit 4
    // D5 - PORTD Bit 5
    // D6 - PORTD Bit 6
    // D7 - PORTD Bit 7
    float voltage_value;
    char voltage_string[10];
    char lcd_string[16];
    lcd_init(16); 
    ADCInit();
    lcd_gotoxy(0,1);    //Move cursor to second line and first column.
    lcd_puts("  DC Voltmeter"); //Write "  DC Voltmeter" on lcd.
    while (1)
          {
          lcd_gotoxy(0,0);  //Move cursor to first line and first column.
          voltage_value = readVoltage()*9.1818;    //Use 33Kohm and 270Kohm to divide voltage.( 9.1818 = (270+33)/33 ).            
          ftoa(voltage_value,2,voltage_string);    //convert float to char array.
          sprintf(lcd_string,"Voltage : %s",voltage_string);
          lcd_puts(lcd_string);       
          delay_ms(500);    ////Refresh lcd every 100 ms.
          }
}
