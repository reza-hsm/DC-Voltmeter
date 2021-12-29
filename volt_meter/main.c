/*
 * volt_meter.c
 *
 * Created: 12/22/2021 2:49:53 AM
 * Author : rezah
 */ 

#include <avr/io.h>
#include "clcd.h"
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

int main(void)
{		
	lcdInit();	//Initialize lcd.
	ADCInit();	//Initialize ADC.
	
	lcdSetCursor(1,0);	//Move cursor to second line and first column.
	printfLcd("  DC Voltmeter");	//Write "  DC Voltmeter" on lcd.
	float voltage_value;
	while (1) 
    {
		voltage_value = readVoltage()*9.1818; //Use 33Kohm and 270 Kohm to divide voltage.( 9.1818 = (270+33)/33 )
		lcdSetCursor(0,0);	//Move cursor to first line and first column.
		printfLcd("Voltage : %f",voltage_value,readADC());
		_delay_ms(100);		//Refresh lcd every 100 ms.
    }
}