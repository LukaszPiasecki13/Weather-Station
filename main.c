
//-------------------------------------------------------------------------------------------------
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

#include "hd44780.c"


int main(void)
{
	LCD_Initalize();
	LCD_Clear();
	LCD_Home();
	 
	int temp = 10;
	char str[10];
	int wilg = 80;
	char str1[10];
	sprintf(str, "%d", temp);
	sprintf(str1, "%d", wilg);
	
	while(1)
	{
		LCD_Clear();
		LCD_WriteText("Temperatura:");
		LCD_WriteText(str);
		LCD_WriteText(" C");
		LCD_GoTo(0,1);
		LCD_WriteText("Wilgotnosc:");
		LCD_WriteText(str1);
		LCD_WriteText("%");
		_delay_ms(5000);
		
	}
}