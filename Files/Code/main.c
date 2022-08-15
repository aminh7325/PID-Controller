#define get_bit(reg,bitnum) ((reg & (1<<bitnum))>>bitnum)
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


void LCDStr (char *str);
void LCDposition (char r, char position, char *str);
void Clear(void);
void Timer(void);
void table(int digit);
char keyboard(void);
void Command( unsigned char Cmd );
void LCDCHARACTER( unsigned char DATA );
int Ki, Kdt, Kd,Kpt, Kp, Kit ;

int main()
{
	DDRA = 0xFC;
	DDRB = 0xF0;
	DDRC = 0xFF;
	DDRD = 0xFF;
	ADMUX  = 0xC0;
	ADCSRA = 0x87;
	DDRA = 0xFC;
	_delay_ms(20);
	Command(0x02);
	Command(0x28);
	Command(0x0c);
	Command(0x06);
	Command(0x01);
	_delay_ms(2);
	Clear();
	LCDStr("Kp :");
	Command(0xC0);
	Kpt = keyboard();
	Kp  = keyboard() + Kpt*10;
	Clear();
	LCDStr("Ki :");
	Command(0xC0);
	Kit = keyboard();
	Ki  = keyboard() + Kit*10;
	Clear();
	LCDStr("Kd :");
	Command(0xC0);
	Kdt = keyboard();
	Kd  = keyboard() + Kdt*10;
	Clear();
	LCDStr("PID Control :");
	Command(0xC0);
	LCDStr("P=");
	LCDCHARACTER(48 + Kpt);
	LCDCHARACTER(48 + Kp - 10*Kpt);
	LCDStr(",I=");
	LCDCHARACTER(48 + Kit);
	LCDCHARACTER(48 + Ki - 10*Kit);
	LCDStr(",D=");
	LCDCHARACTER(48 + Kdt);
	LCDCHARACTER(48 + Kd - 10*Kdt);

	int P1 = 0, I0 = 0, I1 = 0, D0 = 0, D1 = 0;
	int ADCOut0 = 0, ADCOut1 = 0;

	while(1)
	{
		_delay_ms(20);
		ADCSRA |= (1<<6);
		while(get_bit(ADCSRA,6) == 1)
		{

		}
		ADCOut0 = ADCOut1;
		ADCOut1 = ADCL|(ADCH<<8);
		ADCOut1 = ADCOut1/4;
		I0 = I1;
		D0 = D1;
		P1 = Kp*ADCOut1;
		I1 = (Ki/100)*(ADCOut1 + ADCOut0) + I0;
		D1 = (Kd*100)*(ADCOut1 - ADCOut0) + D0;
		PORTC = P1 + I1 + D1;
		
		char key = 16;
		PORTB = 0xF0;
		
		PORTB = 0b11100000;
		_delay_ms(2);
		if(!(PINB & 0b00000001))
		key = 7;
		if(!(PINB & 0b00000010))
		key = 4;
		if(!(PINB & 0b00000100))
		key = 1;
		if(!(PINB & 0b00001000))
		key = 10;
		PORTB = 0b11110000;
		
		PORTB = 0b11010000;
		_delay_ms(2);
		if(!(PINB & 0b00000001))
		key = 8;
		if(!(PINB & 0b00000010))
		key = 5;
		if(!(PINB & 0b00000100))
		key = 2;
		if(!(PINB & 0b00001000))
		key = 0;
		PORTB = 0b11110000;
		
		PORTB = 0b10110000;
		_delay_ms(2);
		if(!(PINB & 0b00000001))
		key = 9;
		if(!(PINB & 0b00000010))
		key = 6;
		if(!(PINB & 0b00000100))
		key = 3;
		if(!(PINB & 0b00001000))
		key = 11;
		PORTB = 0b11110000;
		
		PORTB = 0b01110000;
		_delay_ms(2);
		if(!(PINB & 0b00000001))
		key = 12;
		if(!(PINB & 0b00000010))
		key = 13;
		if(!(PINB & 0b00000100))
		key = 14;
		if(!(PINB & 0b00001000))
		key = 15;
		PORTB = 0b11110000;
		if (key == 10) main();
		if (key == 11) Timer();

	}
}

char keyboard()
{
	char key = 16;
	while(1)
	{
		PORTB = 0xF0;
		
		PORTB = 0b11100000;
		_delay_ms(2);
		if(!(PINB & 0b00000001))
		key = 7;
		if(!(PINB & 0b00000010))
		key = 4;
		if(!(PINB & 0b00000100))
		key = 1;
		if(!(PINB & 0b00001000))
		key = 10;
		PORTB = 0b11110000;
		
		PORTB = 0b11010000;
		_delay_ms(2);
		if(!(PINB & 0b00000001))
		key = 8;
		if(!(PINB & 0b00000010))
		key = 5;
		if(!(PINB & 0b00000100))
		key = 2;
		if(!(PINB & 0b00001000))
		key = 0;
		PORTB = 0b11110000;
		
		PORTB = 0b10110000;
		_delay_ms(2);
		if(!(PINB & 0b00000001))
		key = 9;
		if(!(PINB & 0b00000010))
		key = 6;
		if(!(PINB & 0b00000100))
		key = 3;
		if(!(PINB & 0b00001000))
		key = 11;
		PORTB = 0b11110000;
		
		PORTB = 0b01110000;
		_delay_ms(2);
		if(!(PINB & 0b00000001))
		key = 12;
		if(!(PINB & 0b00000010))
		key = 13;
		if(!(PINB & 0b00000100))
		key = 14;
		if(!(PINB & 0b00001000))
		key = 15;
		PORTB = 0b11110000;

		if (key == 10) main();
		if (key == 11) Timer();

		if (key != 16)
		{
			if(key < 10)
			LCDCHARACTER(48 + key);
			_delay_ms(25);
			return key;
			break;
		}
	}
}

void Command( unsigned char Cmd )
{
	PORTA = (PORTA & 0x0F) | (Cmd & 0xF0);
	PORTA &= ~ (1<<PA3);
	PORTA |= (1<<PA2);
	_delay_us(1);
	PORTA &= ~ (1<<PA2);

	_delay_us(200);

	PORTA = (PORTA & 0x0F) | (Cmd << 4);
	PORTA |= (1<<PA2);
	_delay_us(1);
	PORTA &= ~ (1<<PA2);
	_delay_ms(2);
}
void LCDStr (char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)
	{
		LCDCHARACTER (str[i]);
	}
}

void LCDposition (char r, char position, char *str)
{
	if (r == 0 && position<16)
	Command((position & 0x0F)|0x80);
	else if (r == 1 && position<16)
	Command((position & 0x0F)|0xC0);
	LCDStr(str);
}

void LCDCHARACTER( unsigned char DATA )
{
	PORTA = (PORTA & 0x0F) | (DATA & 0xF0);
	PORTA |= (1<<PA3);
	PORTA|= (1<<PA2);
	_delay_us(1);
	PORTA &= ~ (1<<PA2);

	_delay_us(200);

	PORTA = (PORTA & 0x0F) | (DATA << 4);
	PORTA |= (1<<PA2);
	_delay_us(1);
	PORTA &= ~ (1<<PA2);
	_delay_ms(2);
}

void table(int digit)
{
	if (digit < 10) LCDCHARACTER(digit + 48);
	else if (digit == 10) LCDCHARACTER('A');
	else if (digit == 11) LCDCHARACTER('B');
	else if (digit == 12) LCDCHARACTER('C');
	else if (digit == 13) LCDCHARACTER('D');
	else if (digit == 14) LCDCHARACTER('E');
	else if (digit == 15) LCDCHARACTER('F');
}


void Clear()
{
	Command (0x01);
	_delay_ms(2);
	Command (0x80);
}

void Timer(void)
{
	int Check = 1;
	int  Minute = 0, Second = 0;
	Clear();
	LCDStr("Timer Mode :");
	Command(0xC0);
	LCDStr("00:00");
	while(Check != 0)
	{
		Command(0xC0);
		Second = Second + 1;
		_delay_ms(78);
		Minute = Minute + Second / 60;
		Second = Second % 60;
		Minute = Minute % 60;
		LCDCHARACTER(48 + Minute / 10);
		LCDCHARACTER(48 + Minute % 10);
		LCDCHARACTER(':');
		LCDCHARACTER(48 + Second / 10);
		LCDCHARACTER(48 + Second % 10);
		if (Minute == 16) Check = 0;
		_delay_ms(1);
	}
	Clear();
	for( int i = 0 ; i < 3 ; i++)
	{
		int digit1,digit0;
		PORTD = rand()%256;
		LCDStr("0x");
		digit0 = PORTD % 0x10;
		digit1 = PORTD / 0x10;
		table(digit1);
		table(digit0);
		LCDCHARACTER(' ');
		_delay_ms(1000);
	}
	Command(0xC0);
	for( int i = 0 ; i < 3 ; i++)
	{
		int digit1,digit0;
		PORTD = rand()%256;
		LCDStr("0x");
		digit0 = PORTD % 0x10;
		digit1 = PORTD / 0x10;
		table(digit1);
		table(digit0);
		LCDCHARACTER(' ');
		_delay_ms(1000);
	}
	main();
}

