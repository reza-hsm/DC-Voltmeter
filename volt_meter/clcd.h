/*
 * clcd.h
 *
 * Created: 12/23/2021 8:49:53 PM
 * Author : Reza Hesami
 */ 

#include <stdarg.h>
#define F_CPU	1000000
#include <util/delay.h>
#define setRS	PORTD|=1<<0;	//PORTD0 : 1 
#define setRW	PORTD|=1<<1;	//PORTD1 : 1 
#define setEN	PORTD|=1<<2;	//PORTD2 : 1 
#define clearRS PORTD&=~(1<<0);	//PORTD0 : 0 
#define clearRW PORTD&=~(1<<1);	//PORTD1 : 0 
#define clearEN PORTD&=~(1<<2);	//PORTD2 : 0 

void sendLCDCommand(uint8_t comm);
void sendLCDChar(uint8_t chr);
void numberToString(int number,char number_str[]);
void addString(char str1[],char str2[]);

void lcdInit();
void lcdSetCursor(int row,int col);	//Set position 
void printString(char* str);
void printInteger(int number);
void printFloat(float number);		
void printfLcd(char *input,...);	//printf for lcd
void lcdClear();					//Clear hole lcd

void sendLCDCommand(uint8_t comm){
	_delay_ms(1);
	PORTD = (PORTD & 0x0f) | (comm & 0xf0);	//4 Upper bits of comm
	clearRS		//RS : 0
	clearRW		//RW : 0
	
	setEN		//EN : 1
	_delay_ms(1);
	clearEN		//EN : 0
	_delay_ms(5);
	
	PORTD =(PORTD & 0x0f) | ((comm<<4) & 0xf0); //4 Lower bits of comm
	
	setEN		//EN : 1
	_delay_ms(1);
	clearEN		//EN : 0
	_delay_ms(5);
}

void sendLCDCharacter(uint8_t chr){
	PORTD = (PORTD & 0x0f) | (chr & 0xf0);	//4 Upper bits of chr
	setRS		//RS : 1
	clearRW		//RW : 0
	setEN		//EN : 1
	_delay_ms(1);
	clearEN		//EN : 0
	_delay_ms(5);
	
	PORTD =(PORTD & 0x0f) | ((chr<<4) & 0xf0);	//4 Lower bits of chr
	
	setEN		//EN : 1
	_delay_ms(1);
	clearEN		//EN : 0 
	_delay_ms(5);
}

void printString(char* str){
	for(int i=0;*(str+i)!=0;i++)
		sendLCDCharacter(*(str+i));
}

void lcdSetCursor(int row,int col){
	if(row == 0)
		sendLCDCommand(0x80 | (0x0f & col));	//4 Upper bit : 1000 (row 0) , 4 Lower bits : column number
	else
		sendLCDCommand(0xc0  | (0x0f & col));	//4 Lower bit : 1100 (row 1) , 4 Lower bits : column number

		
}

void numberToString(int number,char number_str[]){
	if(number == 0){
		number_str[0] ='0';
		number_str[1] =0;
		return;
	}
	int len_num=0;
	int tmp =number;
	for(len_num=0;tmp!=0;len_num++)
		tmp/=10;

	for(int i=0;number != 0;i++){
		number_str[len_num-1-i] = number%10+'0';
		number/=10;
	}
	number_str[len_num] = 0;
}

void printInteger(int number){
	char number_str[10];
	numberToString(number,number_str);
	printString(number_str);
}

void addString(char str1[],char str2[]){		//str1 : str1 + str2
	int len_str1 =0;
	for(int i=0;str1[i]!=0;i++)
		len_str1++;
	for(int i=0;str2[i]!=0;i++)
		str1[len_str1+i] = str2[i];
}

void printFloat(float number){
	int integer_number = number;
	int decimal_number = (int)(number*100)%100;
	char float_str[10]={0};
	char integer_str[10];
	char decimal_str[10];
	numberToString(integer_number,integer_str);
	numberToString(decimal_number,decimal_str);
	addString(float_str,integer_str);
	addString(float_str,".");
	if(decimal_number < 10)
		addString(float_str,"0");
	addString(float_str,decimal_str);
	printString(float_str);
}

void lcdInit(){
	DDRD = 0xff;
	
	sendLCDCommand(0x02);	//Return home
	sendLCDCommand(0x28);	//2x16 LCD
	sendLCDCommand(0x01);	//Clear LCD
	sendLCDCommand(0x0c);	//Display on , Cursor off
	sendLCDCommand(0x06);	//Increment cursor
	
}

void printfLcd(char *input,...){
	
	va_list args;
	va_start(args,input);
	
	for(int i=0;input[i]!=0;i++){
		if(input[i] == '%'){
			if(input[i+1] == 'd')	//Detect %d
				printInteger(va_arg(args,int));		
			if(input[i+1] == 'f')	//Detect %f
				printFloat(va_arg(args,double));	
			if(input[i+1] == 's')	//Detect %s
				printString(va_arg(args,char*));
			i+=2;
		}
		
		if(input[i] == 10){			// Detect '\n' 
			printString("                "); // Clear 
			lcdSetCursor(1,0);
			i++;
		}
		if(input[i] == 0)
			break;	
		sendLCDCharacter(input[i]);
	}
	printString("                ");
}

void lcdClear(){
	sendLCDCommand(0x01);
}