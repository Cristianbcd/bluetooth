/*
Para este proyecto se conecta un cristal de 16 MHz en XTAL1 y XTAL2 como muestra la pagina 31 del manual del ATmega164p
se debe tomar en cuanta la configuracion de los fuse en el micro para ello se modifica el LOW Fuse 0xEF, segun recomendaciones del mismo manual
CKSEL 3..0=1111 y SUT = 10 Ext Crystal Osc
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL //Se  establece que la frecuencia del reloj del micro es 16 MHz por el uso del cristal externo, UL significa unsigned long
#define BAUDRATE 9600 // Velocidad de la transmicion que es 9600 bps
#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1) //formula para el preescaler segun la pagina 174 del manual del ATmega164p

unsigned char DATOS[];
unsigned char ABECEDARIO[] = {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//ESP
								0x00,0x7C,0x7E,0x26,0x26,0x7E,0x7C,0x00,//A
								0x00,0x7E,0x7E,0x5A,0x5A,0x5A,0x24,0x00,//B
								0x00,0x7E,0x7E,0x66,0x66,0x66,0x66,0x00,//C
								0x00,0x7E,0x7E,0x66,0x66,0x3c,0x18,0x00,//D
								0x00,0x7e,0x7e,0x52,0x52,0x42,0x42,0x00,//E
								0x00,0x7e,0x7e,0x12,0x12,0x02,0x02,0x00,//F
								0x00,0x7e,0x7e,0x42,0x52,0x72,0x72,0x00,//G
								0x00,0x7e,0x7e,0x18,0x18,0x7e,0x7e,0x00,//H
								0x00,0x66,0x66,0x7e,0x7e,0x66,0x66,0x00,//I
								0x00,0x66,0x66,0x66,0x7e,0x7e,0x06,0x00,//J
								0x00,0x7e,0x7e,0x18,0x18,0x24,0x42,0x00,//K
								0x00,0x7e,0x7e,0x60,0x60,0x60,0x60,0x00,//L
								0x00,0x7e,0x06,0x0c,0x0c,0x06,0x7e,0x00,//M
								0x00,0x7e,0x04,0x08,0x10,0x20,0x7e,0x00,//N
								0x00,0x3c,0x7e,0x66,0x66,0x7e,0x3c,0x00,//O
								0x00,0x7e,0x7e,0x16,0x16,0x1e,0x0c,0x00,//P
								0x00,0x3c,0x42,0x42,0x62,0x42,0xbc,0x00,//Q
								0x00,0x7e,0x7e,0x12,0x12,0x3e,0x6e,0x00,//R
								0x00,0x4e,0x52,0x52,0x52,0x52,0x72,0x00,//S
								0x00,0x06,0x06,0x7e,0x7e,0x06,0x06,0x00,//T
								0x00,0x3e,0x7e,0x60,0x60,0x7e,0x3e,0x00,//U
								0x00,0x1e,0x3e,0x60,0x60,0x3e,0x1e,0x00,//V
								0x00,0x7e,0x60,0x30,0x30,0x60,0x7e,0x00,//W
								0x00,0x42,0x24,0x18,0x18,0x24,0x42,0x00,//X
								0x00,0x5e,0x5e,0x50,0x50,0x7e,0x7e,0x00,//Y
								0x00,0x42,0x62,0x52,0x4a,0x46,0x40,0x00,//Z
								0x00,0x40,0x44,0x7e,0x7e,0x40,0x40,0x00,//1
								0x00,0x72,0x52,0x52,0x52,0x52,0x5e,0x00,//2
								0x00,0x42,0x42,0x52,0x52,0x7e,0x3c,0x00,//3
								0x00,0x1e,0x1e,0x10,0x10,0x7e,0x7e,0x00,//4
								0x00,0x5e,0x52,0x52,0x52,0x72,0x22,0x00,//5
								0x00,0x7e,0x52,0x52,0x52,0x52,0x72,0x00,//6
								0x00,0x02,0x42,0x22,0x12,0x0a,0x06,0x00,//7
								0x00,0x3c,0x7e,0x4a,0x4a,0x7e,0x3c,0x00,//8
								0x00,0x4e,0x4e,0x4a,0x4a,0x7e,0x7e,0x00,//9
								0x00,0x3c,0x7e,0x66,0x66,0x7e,0x3c,0x00,//0
								0x00,0x46,0x26,0x10,0x08,0x64,0x62,0x00,//%
								0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x00,//0
								0x00,0x00,0x00,0x00,0x18,0x24,0x42,0x00,//(
								0x00,0x42,0x24,0x18,0x00,0x00,0x00,0x00,//)
								0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x00,///
								0x00,0x00,0x00,0xde,0xde,0x00,0x00,0x00,//!
								0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,//-
								0x00,0x18,0x18,0x7e,0x7e,0x18,0x18,0x00,//+
								0x00,0x5c,0x54,0xfe,0x54,0x74,0x00,0x00//$
								};
unsigned char recibido;
int contador = 0;
int contador_dos = 0;
int num = 0;
int numero_datos = 0;
int velocidad = 0;
int numero_datos_en_DATOS = 0;
int main(void)
{
	
	unsigned char A[] = {0x00,0x7C,0x7E,0x26,0x26,0x7E,0x7C,0x00};
	unsigned char B[] = {0x00,0x7E,0x7E,0x5A,0x5A,0x5A,0x24,0x00};
	unsigned char C[] = {0x00,0x7E,0x7E,0x66,0x66,0x66,0x66,0x00};
	unsigned char ESP[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	
	ADCSRA = 0x00;
	//Registros de USART
	//Registro de baud rate
	UBRR0 = BAUD_PRESCALER; //Se toma el valor calculado anteriormente
	//Status registers
	//Comunicacion asincronica de 8 bits de datos, 1 bit de parada, y un bit de inicio.
	UCSR0C = ((0<<USBS0)|(1 << UCSZ01)|(1<<UCSZ00));
	UCSR0B = ((1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)); //Se abilita el transmisor y el receptor y la bandera de recepcion
	//
	unsigned char USART_Receive();
	DDRA = 0xFF; //Salidas para el barrido de las columnas
	DDRB = 0xFF; //Salidas para el envio de datos a las filas
	DDRC = 0xFF; //Habilitar y deshabilitar latchs.
	//velocidad = 20;
	//numero_datos = 2;
	//recibido = USART_Receive();
	indicador();
	indicador2();
	configuracion();
    while(1)
    {
		//recibido = 'B';		
		
		for (int columna = 0; columna <=(8*numero_datos+16); columna ++)
		{
			for (int tiempo = 0; tiempo <= velocidad; tiempo++)
			{
			
			//matriz 3;
			PORTC = 0x0B;
			PORTA = 0x01;
			for (int contador = 0+columna; contador <= 7+columna;contador++ )
			{
				PORTB = DATOS[contador];
				_delay_ms(10);
				PORTA = PORTA * 2;
			}
			
			//matriz 2;
			PORTC = 0x0D;
			PORTA = 0x01;
			for (int contador = 0+columna; contador <= 7+columna;contador++ )
			{
				if (contador >= 7)
				{
				PORTB = DATOS[contador-8];
				}
				else
				{
				PORTB = 0x00;
				}
				_delay_ms(10);
				PORTA = PORTA * 2;
			}
			
			//matriz 1;
			PORTC = 0x0E;
			PORTA = 0x01;
			for (int contador = 0+columna; contador <= 7+columna;contador++ )
			{
				//PORTB = 0xf0;
				if (contador>=15)
				{
					PORTB = DATOS[contador-16];
				}
				else
				{
				PORTB = 0x00;
				}
				_delay_ms(10);
				PORTA = PORTA * 2;
			}
		}
		//contador = 0;
		}
		indicador2();
	}
}
//funcion para el envio de caracteres.
void USART_send( unsigned char data)
{
	//Mientras el buffer de transmicion no este vacio.
	//UDDRE0 bandera de registro de datos, si UDRE0 esta en alto, el buffer esta vacio.
	while(!(UCSR0A & (1<<UDRE0)));
	//Si el Buffer esta vacio, entonces se escribe datos para ser enviados.
	UDR0 = data;
}
//funcion para recibir caracteres.
unsigned char USART_Receive()
{
	while (!(UCSR0A & (1<<RXC0)));
	unsigned char data = UDR0;
	return data;
}

int numerodatos(int num)
{
	int numero = 0;
	
	if (recibido == '0')
	{
		numero = 0;
	}
	else if (recibido == '1')
	{
		numero = 1;
	}
	else if (recibido == '2')
	{
		numero = 2;
	}
	else if (recibido == '3')
	{
		numero = 3;
	}
	else if (recibido == '4')
	{
		numero = 4;
	}
	else if (recibido == '5')
	{
		numero = 5;
	}
	else if (recibido == '6')
	{
		numero = 6;
	}
	else if (recibido == '7')
	{
		numero = 7;
	}
	else if (recibido == '8')
	{
		numero = 8;
	}
	else if (recibido == '9')
	{
		numero = 9;
	}
	
	if (contador_dos==0)
	{
		num = num + 10*numero;
		contador_dos++;
	} 
	else if(contador_dos==1)
	{
		num = num + numero;
		contador_dos = 0;	
	}
	return num;
	//numero_datos = numero_datos + numero;
}
void Algoritmo()
{
	int puntero;
	
	if (recibido == ' ')
	{
		puntero = 0;
	}
	else if (recibido == 'A')
	{
		puntero = 1;
	}
	else if (recibido == 'B')
	{
		puntero = 2;
	}
	else if (recibido== 'C')
	{
		puntero = 3;
	}
	else if (recibido== 'D')
	{
		puntero = 4;
	}
	else if (recibido== 'E')
	{
		puntero = 5;
	}
	else if (recibido== 'F')
	{
		puntero = 6;
	}
	else if (recibido== 'G')
	{
		puntero = 7;
	}
	else if (recibido== 'H')
	{
		puntero = 8;
	}
	else if (recibido== 'I')
	{
		puntero = 9;
	}
	else if (recibido== 'J')
	{
		puntero = 10;
	}
	else if (recibido== 'K')
	{
		puntero = 11;
	}
	else if (recibido== 'L')
	{
		puntero = 12;
	}
	else if (recibido== 'M')
	{
		puntero = 13;
	}
	else if (recibido== 'N')
	{
		puntero = 14;
	}
	else if (recibido== 'O')
	{
		puntero = 15;
	}
	else if (recibido== 'P')
	{
		puntero = 16;
	}
	else if (recibido== 'Q')
	{
		puntero = 17;
	}
	else if (recibido== 'R')
	{
		puntero = 18;
	}
	else if (recibido== 'S')
	{
		puntero = 19;
	}
	else if (recibido== 'T')
	{
		puntero = 20;
	}
	else if (recibido== 'U')
	{
		puntero = 21;
	}
	else if (recibido== 'V')
	{
		puntero = 22;
	}
	else if (recibido== 'W')
	{
		puntero = 23;
	}
	else if (recibido== 'X')
	{
		puntero = 24;
	}
	else if (recibido== 'Y')
	{
		puntero = 25;
	}
	else if (recibido== 'Z')
	{
		puntero = 26;
	}
	else if (recibido== '1')
	{
		puntero = 27;
	}
	else if (recibido== '2')
	{
		puntero = 28;
	}
	else if (recibido== '3')
	{
		puntero = 29;
	}
	else if (recibido== '4')
	{
		puntero = 30;
	}
	else if (recibido== '5')
	{
		puntero = 31;
	}
	else if (recibido== '6')
	{
		puntero = 32;
	}
	else if (recibido== '7')
	{
		puntero = 33;
	}
	else if (recibido== '8')
	{
		puntero = 34;
	}
	else if (recibido== '9')
	{
		puntero = 35;
	}
	else if (recibido== '0')
	{
		puntero = 36;
	}
	else if (recibido== '%')
	{
		puntero = 37;
	}
	else if (recibido== '.')
	{
		puntero = 38;
	}
	else if (recibido== '(')
	{
		puntero = 39;
	}
	else if (recibido== ')')
	{
		puntero = 40;
	}
	else if (recibido== '/')
	{
		puntero = 41;
	}
	else if (recibido== '!')
	{
		puntero = 42;
	}
	else if (recibido== '-')
	{
		puntero = 43;
	}
	else if (recibido== '+')
	{
		puntero = 44;
	}
	else if (recibido== '$')
	{
		puntero = 45;
	}
	else
	{
		puntero = 0;
	}
	for (int ingresar = 0; ingresar<=7; ingresar ++)
	{
		DATOS[ingresar+(8*contador)] = ABECEDARIO[8*puntero + ingresar];
		numero_datos_en_DATOS = numero_datos_en_DATOS +1;
	}
	contador++;
}

ISR(USART0_RX_vect)
{
	configuracion();	
}

void indicador()
{
	PORTA = 0xFF;
	PORTB = 0xFF;
	//encendido matriz 1;
	PORTC = 0x0E;
	_delay_ms(1500);
	//encendido matriz 2;
	PORTC = 0x0D;
	_delay_ms(1500);
	//encendido matriz 3;
	PORTC = 0x0B;
	_delay_ms(1500);
	//encendido matriz 3;
	PORTC = 0x0B;
	_delay_ms(1500);
	//encendido matriz 2;
	PORTC = 0x0D;
	_delay_ms(1500);
	PORTC = 0x07;
	//encendido matriz 1
	PORTC = 0x0E;
	_delay_ms(1500);
	//encendido todas matrices;
	PORTC = 0x00;
	_delay_ms(1500);
	PORTC = 0x07;
}
void indicador2()
{
	//ida
	PORTC = 0x0B;
	PORTA = 0x80;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA / 2;
	}
	PORTC = 0x0D;
	PORTA = 0x80;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA / 2;
	}
	PORTC = 0x0E;
	PORTA = 0x80;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA / 2;
	}
	//vuelta
	PORTC = 0x0E;
	PORTA = 0x01;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA * 2;
	}
	PORTC = 0x0D;
	PORTA = 0x01;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA * 2;
	}
	PORTC = 0x0B;
	PORTA = 0x01;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA * 2;
	}
	//ida
	PORTC = 0x0B;
	PORTA = 0x80;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA / 2;
	}
	PORTC = 0x0D;
	PORTA = 0x80;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA / 2;
	}
	PORTC = 0x0E;
	PORTA = 0x80;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA / 2;
	}
	//vuelta
	PORTC = 0x0E;
	PORTA = 0x01;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA * 2;
	}
	PORTC = 0x0D;
	PORTA = 0x01;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA * 2;
	}
	PORTC = 0x0B;
	PORTA = 0x01;
	PORTB = 0xFF;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(200);
		PORTA = PORTA * 2;
	}
	//bajada
	PORTC = 0x08;
	PORTA = 0xFF;
	PORTB = 0x01;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(300);
		PORTB = PORTB * 2;
	}
	//subida
	PORTC = 0x08;
	PORTA = 0xFF;
	PORTB = 0x80;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(300);
		PORTB = PORTB / 2;
	}
	//bajada
	PORTC = 0x08;
	PORTA = 0xFF;
	PORTB = 0x01;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(300);
		PORTB = PORTB * 2;
	}
	//subida
	PORTC = 0x08;
	PORTA = 0xFF;
	PORTB = 0x80;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(300);
		PORTB = PORTB / 2;
	}
	//bajada
	PORTC = 0x08;
	PORTA = 0xFF;
	PORTB = 0x01;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(300);
		PORTB = PORTB * 2;
	}
	//subida
	PORTC = 0x08;
	PORTA = 0xFF;
	PORTB = 0x80;
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(300);
		PORTB = PORTB / 2;
	}
	for (int cont = 0; cont<=7 ; cont++)
	{
		_delay_ms(300);
		PORTA = PORTA * 2;
	}
		PORTC = 0x08;
		PORTB = 0xFF;
		PORTA = 0xFF;
		for (int cont = 0; cont<=10 ; cont++)
		{
			_delay_ms(600);
			PORTC = 0x0F;
			_delay_ms(600);
			PORTC = 0x08;
		}
}
void configuracion()
{
	for (int i = 0; i<= numero_datos_en_DATOS;i++)
	{
		DATOS[i]=0x00;
	}
	numero_datos_en_DATOS = 0;
	contador = 0;
	numero_datos = 0;
	contador_dos = 0;
	velocidad = 0;
	cli();
	for (int i = 0; i <= 1;i++)
	{
		recibido = USART_Receive();
		velocidad = numerodatos(velocidad);
	}
	//indicador();
	for (int i = 0; i <= 1;i++)
	{
		recibido = USART_Receive();
		numero_datos = numerodatos(numero_datos);
	}
	
	for (int i = 0; i <= numero_datos-1;i++)
	{
		recibido = USART_Receive();
		Algoritmo();
	}
	sei();
	indicador();
}
