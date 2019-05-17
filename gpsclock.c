//gpsclock.c
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//#include "uart.h"
#include "gpsclock.h"

/*
void get_GPGGA(void){
	
	unsigned int c; 		// received char from the UART
	unsigned char input;	// the same, converted to char
	char gps_time[10], latitude[14], longitude[14], sat_numb[5], DOP[5], alt[8];
	uint8_t GPGGA_lock, count_j, count_k, latitude_n_s, longitude_w_e;
	
	GPGGA_lock = 1;
	while(GPGGA_lock){
		c = uart_getc(); 	// usart_receive();
		input = (unsigned char) c;		// convert to char
		if(input == '$'){
		c = uart_getc();		
		input = (unsigned char) c;//usart_receive();
			if(input == 'G'){
			c = uart_getc();		
			input = c;//usart_receive();
			//input = uart_getc(); //usart_receive();
				if(input == 'P'){
					//input = uart_getc(); //usart_receive();
					c = uart_getc();		
					input = (unsigned char) c;//usart_receive();
					if(input == 'G'){
						//input = uart_getc(); //usart_receive();
						c = uart_getc();		
						input = (unsigned char) c;//usart_receive();
						if(input == 'G'){
							//input = uart_getc(); //usart_receive();
							c = uart_getc();		
							input = (unsigned char) c;//usart_receive();
							if(input == 'A'){
								c = uart_getc();		
								input = (unsigned char) c;//usart_receive();
								//input = uart_getc(); //usart_receive();
								if(input == ','){
									// Get time
									c = uart_getc();		
									input = (unsigned char) c;//usart_receive();
									//input = uart_getc(); //usart_receive();
									gps_time[0] = input;
									for(count_j=1;input != ',';count_j++){
										c = uart_getc();		
										input = c;//usart_receive();
										//input = uart_getc(); //usart_receive();
										gps_time[count_j] = input;									
									}
									// Get LATitude---------------------------
									input = uart_getc(); //usart_receive();
									latitude[0] = input;
									for(count_j=1;input != ',';count_j++){ //for(count_j=1;input != ',';count_j++){
										input = uart_getc(); //usart_receive();
										latitude[count_j] = input;								
									}
									// N/S
									input = uart_getc(); //usart_receive();
									latitude_n_s = input;
									// Jump ,
									input = uart_getc(); //usart_receive();
									input = uart_getc(); //usart_receive();
									
									// Get LONgitude --------------------------
									input = uart_getc(); //usart_receive();
									longitude[0] = input;
									for(count_j=1;input != ',';count_j++){
										input = uart_getc(); //usart_receive();
										longitude[count_j] = input;								
									}
									// W/E
									input = uart_getc(); //usart_receive();
									longitude_w_e = input;
									// jump 1,
									input = uart_getc(); //usart_receive();
									input = uart_getc(); //usart_receive();
									input = uart_getc(); //usart_receive();
									// Sat ----------------
									input = uart_getc(); //usart_receive();
									sat_numb[0] = input;
									for(count_j=1;input != ',';count_j++){
										input = uart_getc(); //usart_receive();
										sat_numb[count_j] = input;
									}
									
									// DOP --------------------
									input = uart_getc(); //usart_receive();
									DOP[0] = input;
									for(count_j=1;input!=',';count_j++){
										input = uart_getc(); //usart_receive();
										DOP[count_j] = input;
									}
									// ALTitude -----------------
									input = uart_getc(); //usart_receive();
									alt[0] = input;
									for(count_j=1;input!=',';count_j++){
										input = uart_getc(); //usart_receive();
										alt[count_j] = input;
									}
									GPGGA_lock = 0; // end of loop
								}	//if
							}
						} //if
					} //if
				} //if		
			}
		} //if
	} //if
	} //while

*/
/*
void displayLon(char *lon) {
	lcd_gotoxy(TIME_X, TIME_Y+3); // first line, first character
	lcd_putc(lon[0]);
	lcd_putc(lon[1]);
	lcd_putc(lon[2]);
	lcd_putc(lon[3]);
	lcd_putc(lon[4]);
	lcd_putc(lon[5]);
	lcd_putc(lon[6]);
	lcd_putc(lon[7]);
}
*/
/*
void displayLat(char *lat) {
	lcd_gotoxy(TIME_X, TIME_Y+2); // first line, first character
	lcd_putc(lat[0]);
	lcd_putc(lat[1]);
	lcd_putc(lat[2]);
	lcd_putc(lat[3]);
	lcd_putc(lat[4]);
	lcd_putc(lat[5]);
	lcd_putc(lat[6]);
	lcd_putc(lat[7]);
}
*/
/*
void displayTime(char *utctime) {
	lcd_gotoxy(TIME_X, TIME_Y); // first line, first character
	lcd_putc(utctime[0]);
	lcd_putc(utctime[1]);
	lcd_putc(':');
	lcd_putc(utctime[2]);
	lcd_putc(utctime[3]);
	lcd_putc(':');
	lcd_putc(utctime[4]);
	lcd_putc(utctime[5]);
}
*/
