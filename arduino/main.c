#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdio.h>

#include "lib/uart/uart.h"
#include "packets.h"
#include "color.h"
#include "color_write.h"
#include "wave.h"


int main(void) {
	uart_init();
	stdout = &uart_output;
	stdin = &uart_input;
	
	/* Setting ports to output */
    DDRB |= _BV(PORTB5); //Pin 13 led
	DDRD |= _BV(DDD6); //R
	DDRD |= _BV(DDD5); //G
	DDRD |= _BV(DDD3); //B
	
	/* PWM manual: https://sites.google.com/site/qeewiki/books/avr-guide/pwm-on-the-atmega328 */
	/* Setting PWM for D5 */
	//TCCR0A |= _BV(COM0B1); // set none-inverting mode
	TCCR0A |= _BV(WGM01) | (1 << WGM00); // set fast PWM Mode
	TCCR0B |= _BV(CS00); // set prescaler to 8 and starts PWM
	
	/* Setting PWM for D6 */
	//TCCR0A |= _BV(COM0A1); // set none-inverting mode
	TCCR0A |= _BV(WGM01) | (1 << WGM00); // set fast PWM Mode
	TCCR0B |= _BV(CS00); // set prescaler to 8 and starts PWM
	
	/* Setting PWM for D3 */
	//TCCR2A |= _BV(COM2B1); // set none-inverting mode
    TCCR2A |= _BV(WGM21) | (1 << WGM20); // set fast PWM Mode
    TCCR2B |= _BV(CS20); // set prescaler to 8 and starts PWM

	/* Initial PWM values */
	//OCR0B = 0; //PORTD5
	//OCR0A = 0; //PORTD6
	//OCR2B = 0; //PORTD3
	
	
	enum mode_t {
		WAVE = '1',
		SINGLE_COLOR,
		FADE_COLOR,
		WHITE_LIGHT,
		OFF
	};
	enum mode_t mode = OFF;
	color_rgb_t color;
	
	while (1) {
		size_t size;
		/* Blink led by toggling state of PORTB5 (Arduino digital 13). */
        PORTB ^= _BV(PORTB5);
		
		uint8_t command;
		size = fread(&command, 1, 1, stdin);
		if(size != 1) {
			printf("Unexpected EOF: %d", size);
			fflush(stdin);
			break;
		}
		switch(command) {
			case 'w':
				puts("got wave");
				packet_wave_t wave;
				size = fread(&wave, sizeof(packet_wave_t), 1, stdin);
				if(size != 1) {
					printf("Unexpected EOF: %d\n", size);
					fflush(stdin);
					break;
				}
				
				color_rgb_t col_rgb = color_hsl_to_rgb(wave_color(wave));
				//col_rgb = color_white_correct(col_rgb, 255, 100, 80);
				if(mode == WAVE) {
					color_write(col_rgb);
				}
				break;
			
			case 'r':
				puts("got rgb");
				packet_set_rgb_t color_rgb_packet;
				size = fread(&color_rgb_packet, sizeof(packet_set_rgb_t), 1, stdin);
				if(size != 1) {
					printf("Unexpected EOF: %d\n", size);
					fflush(stdin);
					break;
				}
				
				printf("Color RGB: %0x %0x %0x\n", color_rgb_packet.R, color_rgb_packet.G, color_rgb_packet.B);
				
				mode = SINGLE_COLOR;
				
				color.R = color_rgb_packet.R;
				color.G = color_rgb_packet.G;
				color.B = color_rgb_packet.B;
				color_write(color);
				break;
			
			case 'h':
				puts("got hsl");
				packet_set_hsl_t color_hsl_packet;
				size = fread(&color_hsl_packet, sizeof(packet_set_hsl_t), 1, stdin);
				if(size != 1) {
					printf("Unexpected EOF: %d\n", size);
					fflush(stdin);
					break;
				}
				
				printf("Color RGB: %0d %0d %0d\n", color_hsl_packet.H, color_hsl_packet.S, color_hsl_packet.L);
				
				mode = SINGLE_COLOR;
				
				color_hsl_t color_hsl = {color_hsl_packet.H, color_hsl_packet.S, color_hsl_packet.L};
				color = color_hsl_to_rgb(color_hsl);
				color_write(color);
				break;
			
			case 'm':
				puts("got mode");
				packet_set_mode_t mode_packet;
				size = fread(&mode_packet, sizeof(packet_set_mode_t), 1, stdin);
				if(size != 1) {
					printf("Unexpected EOF: %d\n", size);
					fflush(stdin);
					break;
				}
				
				printf("Mode: %#x\n", mode_packet.mode);
				
				mode = mode_packet.mode;
				
				color_rgb_t white = {255, 255, 255};
				color_rgb_t black = {0, 0, 0};
				switch(mode) {
					case SINGLE_COLOR:
						color_write(color);
						break;
					case WHITE_LIGHT:
						color_write(white);
						break;
					case OFF:
						color_write(black);
						break;
				}
				break;
			
			default:
				printf("Unknown command: %#x\n", command);
		}
		
		fflush(stdin);
	}

	return 0;
}