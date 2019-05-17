// Header for gpsclock.c
#ifndef GPSCLOCK_H_
	#define GPSCLOCK_H_

/*****************************
 * Pin definitions
 *****************************/

// LED Output
/*
#define LED_NOLOCK		PD4
#define LED_NOLOCK_DDR	DDRD
#define LED_NOLOCK_PORT PORTD
#define LED_2DLOCK		PD3
#define LED_2DLOCK_DDR	DDRD
#define LED_2DLOCK_PORT PORTD
#define LED_3DLOCK 		PD2
#define LED_3DLOCK_DDR  DDRD
#define LED_3DLOCK_PORT PORTD
*/
/*****************************
 * Macros
 *****************************/

// UART baud rate. most GPS modules use 9600 baud
	#define UART_BAUD_RATE      9600


/*****************************
 * Auxiliary Macros
 * DO NOT CHANGE!
 *****************************/

// to change the LCD properties please edit lcd_definitions.h

#define TIME_X 0
#define TIME_Y 0

// some 16x1 think they're 8x2 displays
	#if (LCD_DISP_LENGTH == 8) && (LCD_LINES == 2)
		#define TXT_TEMPLATE "xx:xx:xx\n xx xxSa"
		#define LOCK_X 1
		#define LOCK_Y 1
		#define SAT_X 4
		#define SAT_Y 1

	// 20x4
	#elif (LCD_DISP_LENGTH == 20) && (LCD_LINES == 4)
		#define TXT_TEMPLATE "xx:xx:xxCET"
		#define LOCK_X 0
		#define LOCK_Y 1
		#define sat_x 8
		#define sat_y 1
		#define alt_x 0
		#define alt_y 2
		#define lon_x 0
		#define lon_y 2
		#define lat_x 0
		#define lat_y 2
		#define time_x 0
		#define time_y 2
		#define RTC_TXT		/* Macro to activate the "RTC" text*/

		#ifdef RTC_TXT
			#define RTC_X 13
			#define RTC_Y 0
		#endif 	// RTC_TXT

	#endif // 16x2 ,20x4

// Receiver states:
// 0 - no command
// 1 - receiving command
// 2 - receiving GPGGA
// 3 - receiving GPGSA
#define NO_CMD		0
#define RECV_CMD 	1
#define RECV_GPGGA	2
#define RECV_GPGSA	3

#endif // GPSCLOCK_H_

/*****************************
 * Function Prototypes
 ****************************/

void displayTime(char *utctime); 

void displayLat(char *lat); 

void displayLon(char *lon); 

void get_GPGGA(void);


