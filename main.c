/*************************************************************************
Title:    Interprets NMEA messages on the UART
Author:   
File:     main.c
Software: AVR-GCC 
Hardware: HD44780 compatible LCD text display
		  GPS module sending NMEA messages over serial
		  i2C pins for LCD, 1 for UART
 $GPGGA,123519,4807.038,N,01131.324,E,1,08,0.9,545.4,M,46.9,M, , *42
		1  ,   2   ,2,    3    ,3,    4,5, 6  ,7,      8  ,9,  10
	01 - 123519 = Acquisition du FIX a 12:35:19 UTC
	02 - 4807.038,N = Latitude 48∞07.038' N
	03 - 131.324,E = Longitude 11∞31.324' E
	04 - 1 = Fix qualification : (0 = non valide, 1 = Fix GPS, 2 = Fix DGPS)
	05 - 08 = Number os satellites en poursuite.
	06 - 0.9 = DOP (Horizontal dilution of position) Dilution horizontale.
	07 - 545.4,M = Altitude, en Metres, au dessus du MSL (mean see level) niveau moyen des OcÈans.
	08 - 46.9,M = Correction de la hauteur de la gÈoide en Metres par rapport a l'Èllipsoide WGS84 (MSL).
	09 - (Champ vide) = nombre de secondes ÈcoulÈes depuis la derniere mise a jour DGPS.
	10 - (Champ vide) = Identification de la station DGPS.
	11 - *42 = Checksum
	12 - Non reprÈsentÈs CR et LF.

**************************************************************************/
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h> // kvoli scanf
#include <string.h> // kvoli strncpy, strncmp
#include <avr/pgmspace.h> // kvoli makru PSTR (v printf_P) 

#include <util/delay.h>
#include "lcdpcf8574/lcdpcf8574.h"	//i2c addr 3F 
#include "gpsclock.h"

#define REVISION "R3 2019"

#define MAX_STRLEN 75	//75
#define OFFSET_UTC 2
#define BAUDRATE 9600UL // aktu·lnÌ baudrate (maximum baudrate F_CPU/16, resp. F_CPU/8 s CLK2X)
#define LED_ON PORTB.OUTCLR = PIN5_bm // rozsviù LED na PB5 (p¯ipojen· proti VCC)
#define LED_OFF PORTB.OUTSET = PIN5_bm // zhasni LED na PB5 (p¯ipojen· proti VCC)

// Makro na v˝poËet hodnoty BAUD registeru (BAUDVAL) 
#if BAUDRATE < F_CPU/16 // pokud nenÌ baudrate vÏtöÌ neû maximum
 #if (((10*4*F_CPU)/BAUDRATE) % 10) > 4 // zaokrouhlov·nÌ (round)
  #define BAUDVAL (((4*F_CPU)/BAUDRATE)+1) // zaokrouhlÌme nahoru (round up)
 #else
  #define BAUDVAL ((4*F_CPU)/BAUDRATE) // zaokrouhlÌme dol˘ (floor)
 #endif
#else
 #warning "Baudrate is out of range ! (max baudrate without CLK2X is F_CPU/16 (1.25Mb/s if F_CPU=20MHz)"
 #define BAUDVAL 0
#endif

void USART_init(unsigned int ubrr);
unsigned char usart_receive(void);
void USART_flush(void);


volatile unsigned char sentence[MAX_STRLEN],gga[MAX_STRLEN],rmc[MAX_STRLEN],vtg[MAX_STRLEN]; // ¯etÏzec s p¯Ìkazem
volatile unsigned char rx_nmea[MAX_STRLEN];
volatile unsigned char senten=0,prijem=0,prijaty_znak=0; // p¯Ìznak ûe dorazila cel· zpr·va
//static unsigned char rx_nmea[MAX_STRLEN]; // zde doËasnÏ ukl·d·me p¯ich·zejÌcÌ znaky
//static uint8_t cnt=0; // poËÌtadlo p¯ijat˝ch znak˘, 

int main(void) {
	int i,j_ag,j_a,j_t,j_d,j_s,j_lon,j_lat,j_sat,ac_alti,geoid,j_v;
	char lcd_buffer[20],time[10],date[7],altitude[15],altitude_g[5],lon[15],lat[15],speed[5],sat[3],status[2]; 
	int coma_gga=0,coma_rmc=0,coma_vtg=0;		
		uint8_t led = 0; //display ON
	for(i=0;i<MAX_STRLEN;i++)
	{sentence[i]=0;gga[i]=0;rmc[i]=0;vtg[i]=0;
	}
		
	//-------init LCD 
	lcd_init(LCD_DISP_ON);
    lcd_home();
	lcd_led(led);
	
	// initialize UART and enable interrupts
	//uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	//uart_init(UART_BAUD_SELECT(9600,16000000UL));
	
	USART_init(103); // 103-9600baud
	sei();
	
	lcd_puts_P("GPS-Clock "REVISION);
	lcd_puts_P("\n  GPS-data");
	_delay_ms(1500);
	
	lcd_clrscr();
		
while(1){
	
	if(senten){
		senten=0;
		//RMC ---------------------------------------------------------------
		
		coma_rmc=0;coma_gga=0;coma_vtg=0;j_t=0,j_v=0,j_d=0;j_ag=0;j_a=0;j_sat=0;j_s=0;j_lat=0;j_lon=0;
		for(i=4;i<(MAX_STRLEN-4);i++)
			 {
			 	if(rmc[i] == ',')coma_rmc++;
				if((coma_rmc == 1) && (j_t < 6)){time[j_t] = rmc[i+1];j_t++;} // time
				if((coma_rmc == 9) && (j_d < 6)){date[j_d] = rmc[i+1];j_d++;} // date
				if((coma_rmc == 2) && (j_v < 1)){status[j_v] = rmc[i+1];j_v++;} // date
				if(gga[i] == ',')coma_gga++;	
				if((coma_gga == 7) && (j_sat < 2) ){sat[j_sat] = gga[i+1];j_sat++;} // number of satellites being tracked
				if((coma_gga == 9) && (j_a < 4)){altitude[j_a] = gga[i+1];j_a++;} // altitude
				if((coma_gga == 11) && (j_ag < 2)){altitude_g[j_ag] = gga[i+1];j_ag++;} // height of geoid
				if(vtg[i] == ',')coma_vtg++;
				if((coma_vtg == 7) && (j_s < 3)){speed[j_s] = vtg[i+1];j_s++;} //speed km/h, knot->km/h *1.852
				
				if(( coma_rmc == 3) || (coma_rmc == 4) ){lat[j_lat] = rmc[i+1];j_lat++;} // latitude
				if((coma_rmc == 5) || (coma_rmc == 6)){lon[j_lon] = rmc[i+1];j_lon++;} // longitude
			} //for
			time[1] = time[1] + 2; //OFFSET_UTC
			//altitude to int 
			// altitude_i = altitude[]-48;
			ac_alti = atoi(altitude);
			geoid = atoi(altitude_g);
			ac_alti = ac_alti - geoid;

			lcd_gotoxy(0,0);
			lcd_puts(time);lcd_puts_P("-DATE-");lcd_puts(date);lcd_puts(status);
			lcd_gotoxy(0,1);
			lcd_puts(sat);
			lcd_puts_P("-ALT");
			//lcd_puts(altitude);
			
			sprintf(lcd_buffer,"%.0im ",ac_alti);
			lcd_puts(lcd_buffer);
			
			lcd_puts(speed);lcd_puts_P("km/h");
			lcd_gotoxy(0,2);
			lcd_puts(lat);
			lcd_gotoxy(0,3);
			lcd_puts(lon);
			
			
			} // if(senten)
			
		} //end of while main loop
		
		
} //end of main

//-----------------------------------------------
ISR(USART_RX_vect){
 char znak,zac; // pomocn· premenna
 int j;
 static int cnt=0;
 //static char cnt=0;

  //err = USART0.RXDATAH; // prÌpadna kontrola chyb
 znak = UDR0;
 if(znak == '$' ){zac=1;cnt=0;}	
 if((zac == 1) && (znak != '\n') && ( znak != '\r') && (cnt<MAX_STRLEN-2) ){sentence[cnt]=znak;cnt++;}
 else{ rx_nmea[cnt]='\0';
 		for(j=0;j<MAX_STRLEN;j++)sentence[j]=rx_nmea[j];
		//strncpy(sentence,rx_nmea,MAX_STRLEN);
	 	cnt=0;
	 }

 if( (senten == 1) && (sentence[4] == 'M') && (sentence[5] == 'C')){
		for(j=0;j<MAX_STRLEN;j++)rmc[j]=sentence[j];}
		//strncpy(rmc,sentence,MAX_STRLEN);}
 if( (senten == 1) && (sentence[4] == 'G') && (sentence[5] == 'A')){
		for(j=0;j<MAX_STRLEN;j++)gga[j]=sentence[j];}
		//strncpy(gga,sentence,MAX_STRLEN);}
 if( (senten == 1) && (sentence[4] == 'T') && (sentence[5] == 'G')){
 		for(j=0;j<MAX_STRLEN;j++)vtg[j]=sentence[j];}
		//strncpy(vtg,sentence,MAX_STRLEN);}
 senten=1;
}
//-------------------------------------------------
void USART_init(unsigned int ubrr){
	/* set baud rate 103 for 9600baud*/
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*enable receiver and transmitter, interrupts*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* UMSEL = 0 - asyn */
	/*set frame format 8bit None 1 stopbit - default*/
	UCSR0C = (0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UPM00);
}

unsigned char usart_receive( void ){
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

void USART_flush(void){
	unsigned char dummy;
	while (UCSR0A & (1<<RXC0)) dummy = UDR0;
}


