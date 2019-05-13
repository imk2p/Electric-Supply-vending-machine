#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"sbit.h"
#include<string.h>
#include<stdlib.h>
#include"lcd.h"

#define RELAY 			SBIT(PORTD,5)
#define RELAY_DIR		SBIT(DDRD,5)

#define IR 				SBIT(PINA,2)
#define IR_DIR			SBIT(DDRA,2)

#define RED_LED			SBIT(PORTC,1)
#define GREEN_LED		SBIT(PORTC,0)
#define RL_DIR			SBIT(DDRC,1)
#define GL_DIR			SBIT(DDRC,0)

int remaining_coin = 10;
int on_time = 0;
int timeout = 200;

enum{IR_DET,START_ELEC};

int main()
{
	RELAY_DIR = 1;
	IR_DIR = 0;
	RL_DIR = 1;
	GL_DIR = 1;

	RELAY = 0;
	RED_LED = 1;
	GREEN_LED = 1;

	lcd_init();
	lcd_clear();
	_delay_ms(1000);
	lcd_printf("COIN DETECTOR");
	_delay_ms(1000);
	lcd_clear();
	//lcd_printf("WAITING FOR COIN");
	int state = IR_DET;
	char ls,cs;
	char arr[4];
	while(1)
	{
		_delay_ms(100);
		switch(state)
		{
			case IR_DET:
				RED_LED = 0;
				GREEN_LED = 1;
				if(remaining_coin!=0)
				{
					cs = IR;
					if((cs)&&(!ls))
					{
						_delay_ms(20);
						if((PINA&(1<<2)))
						{
							RED_LED = 1;
							GREEN_LED = 0;
							state = START_ELEC;
							RELAY = 1;
							timeout = 200;
							remaining_coin--;
						}
					}
					ls=cs;
				}
			break;
			case START_ELEC:
				timeout--;
				if(timeout==0)
				{
					state = IR_DET;
					RELAY = 0;
				}
			break;
		}
		lcd_gotoxy(1,1);
		itoa(remaining_coin,arr,10);
		lcd_printf("COIN=");
		lcd_printf(arr);
		lcd_printf("  ");
		lcd_gotoxy(2,1);
		on_time = timeout /10;
		itoa(on_time,arr,10);
		lcd_printf("TIME LEFT=");
		lcd_printf(arr);
		lcd_printf("  ");
	}
}
