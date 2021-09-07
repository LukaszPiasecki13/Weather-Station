//-------------------------------------------------------------------------------------------------
//
// Konfiguracja sygna��w steruj�cych wy�wietlaczem.
// Mo�na zmieni� stosownie do potrzeb.
//
//-------------------------------------------------------------------------------------------------
#define LCD_RS_DIR		DDRD
#define LCD_RS_PORT 	PORTD
#define LCD_RS			(1 << PD4)

#define LCD_E_DIR		DDRD
#define LCD_E_PORT		PORTD
#define LCD_E			(1 << PD5)

#define LCD_DB4_DIR		DDRD
#define LCD_DB4_PORT	PORTD
#define LCD_DB4			(1 << PD0)

#define LCD_DB5_DIR		DDRD
#define LCD_DB5_PORT	PORTD
#define LCD_DB5			(1 << PD1)

#define LCD_DB6_DIR		DDRD
#define LCD_DB6_PORT	PORTD
#define LCD_DB6			(1 << PD2)

#define LCD_DB7_DIR		DDRD
#define LCD_DB7_PORT	PORTD
#define LCD_DB7			(1 << PD3)

//-------------------------------------------------------------------------------------------------
//
// Instrukcje kontrolera Hitachi HD44780
//
//-------------------------------------------------------------------------------------------------

#define HD44780_CLEAR					0x01

#define HD44780_HOME					0x02

#define HD44780_ENTRY_MODE				0x04
#define HD44780_EM_SHIFT_CURSOR		0
#define HD44780_EM_SHIFT_DISPLAY	1
#define HD44780_EM_DECREMENT		0
#define HD44780_EM_INCREMENT		2

#define HD44780_DISPLAY_ONOFF			0x08
#define HD44780_DISPLAY_OFF			0
#define HD44780_DISPLAY_ON			4
#define HD44780_CURSOR_OFF			0
#define HD44780_CURSOR_ON			2
#define HD44780_CURSOR_NOBLINK		0
#define HD44780_CURSOR_BLINK		1

#define HD44780_DISPLAY_CURSOR_SHIFT	0x10
#define HD44780_SHIFT_CURSOR		0
#define HD44780_SHIFT_DISPLAY		8
#define HD44780_SHIFT_LEFT			0
#define HD44780_SHIFT_RIGHT			4

#define HD44780_FUNCTION_SET			0x20
#define HD44780_FONT5x7				0
#define HD44780_FONT5x10			4
#define HD44780_ONE_LINE			0
#define HD44780_TWO_LINE			8
#define HD44780_4_BIT				0
#define HD44780_8_BIT				16

#define HD44780_CGRAM_SET				0x40

#define HD44780_DDRAM_SET				0x80

//-------------------------------------------------------------------------------------------------
//
// Deklaracje funkcji
//
//-------------------------------------------------------------------------------------------------

void LCD_WriteCommand(unsigned char);
void LCD_WriteData(unsigned char);
void LCD_WriteText(char *);
void LCD_GoTo(unsigned char, unsigned char);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_Initalize(void);

//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiaj�ca p�bajt na magistral� danych
//
//-------------------------------------------------------------------------------------------------
void _LCD_OutNibble(unsigned char nibbleToWrite)
{
	if(nibbleToWrite & 0x01)
	LCD_DB4_PORT |= LCD_DB4;
	else
	LCD_DB4_PORT  &= ~LCD_DB4;

	if(nibbleToWrite & 0x02)
	LCD_DB5_PORT |= LCD_DB5;
	else
	LCD_DB5_PORT  &= ~LCD_DB5;

	if(nibbleToWrite & 0x04)
	LCD_DB6_PORT |= LCD_DB6;
	else
	LCD_DB6_PORT  &= ~LCD_DB6;

	if(nibbleToWrite & 0x08)
	LCD_DB7_PORT |= LCD_DB7;
	else
	LCD_DB7_PORT  &= ~LCD_DB7;
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wy�wietacza (bez rozr�nienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
void _LCD_Write(unsigned char dataToWrite)
{
	LCD_E_PORT |= LCD_E;
	_LCD_OutNibble(dataToWrite >> 4);
	LCD_E_PORT &= ~LCD_E;
	LCD_E_PORT |= LCD_E;
	_LCD_OutNibble(dataToWrite);
	LCD_E_PORT &= ~LCD_E;
	_delay_us(50);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wy�wietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteCommand(unsigned char commandToWrite)
{
	LCD_RS_PORT &= ~LCD_RS;
	_LCD_Write(commandToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pami�ci wy�wietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteData(unsigned char dataToWrite)
{
	LCD_RS_PORT |= LCD_RS;
	_LCD_Write(dataToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wy�wietlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteText(char * text)
{
	while(*text)
	LCD_WriteData(*text++);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia wsp�rz�dnych ekranowych
//
//-------------------------------------------------------------------------------------------------
void LCD_GoTo(unsigned char x, unsigned char y)
{
	LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja czyszczenia ekranu wy�wietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Clear(void)
{
	LCD_WriteCommand(HD44780_CLEAR);
	_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywr�cenia pocz�tkowych wsp�rz�dnych wy�wietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Home(void)
{
	LCD_WriteCommand(HD44780_HOME);
	_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
void LCD_Initalize(void)
{
	unsigned char i;
	LCD_DB4_DIR |= LCD_DB4; // Konfiguracja kierunku pracy wyprowadze�
	LCD_DB5_DIR |= LCD_DB5; //
	LCD_DB6_DIR |= LCD_DB6; //
	LCD_DB7_DIR |= LCD_DB7; //
	LCD_E_DIR 	|= LCD_E;   //
	LCD_RS_DIR 	|= LCD_RS;  //
	_delay_ms(15); // oczekiwanie na ustalibizowanie si� napiecia zasilajacego
	LCD_RS_PORT &= ~LCD_RS; // wyzerowanie linii RS
	LCD_E_PORT &= ~LCD_E;  // wyzerowanie linii E

	for(i = 0; i < 3; i++) // trzykrotne powt�rzenie bloku instrukcji
	{
		LCD_E_PORT |= LCD_E; //  E = 1
		_LCD_OutNibble(0x03); // tryb 8-bitowy
		LCD_E_PORT &= ~LCD_E; // E = 0
		_delay_ms(5); // czekaj 5ms
	}

	LCD_E_PORT |= LCD_E; // E = 1
	_LCD_OutNibble(0x02); // tryb 4-bitowy
	LCD_E_PORT &= ~LCD_E; // E = 0

	_delay_ms(1); // czekaj 1ms
	LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wy��czenie wyswietlacza
	LCD_WriteCommand(HD44780_CLEAR); // czyszczenie zawartos�i pamieci DDRAM
	_delay_ms(2);
	LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);// inkrementaja adresu i przesuwanie kursora
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // w��cz LCD, bez kursora i mrugania
}

//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.c
//
//-------------------------------------------------------------------------------------------------


