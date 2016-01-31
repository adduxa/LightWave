#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

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
	enum mode_t mode = WAVE;
	color_rgb_t color;
	/*
	float h = 0;
	color_hsl_t qw = {0, 1, 0.5};
	while(1) {
		color_write(color_white_correct(color_hsl_to_rgb(qw), 100, 255, 10));
		qw.H += 1;
		qw.H = fmod(qw.H, 360.0);
		_delay_ms(10);
	}
	*/
	
	
	while (1) {
		size_t size;
		
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
				color_hsl_t col_hsl = wave_color(wave);
				printf("Color HSL: %0d %0d %0d\n", (int)col_hsl.H, (int)(col_hsl.S*100), (int)(col_hsl.L*100));
				
				color_rgb_t col_rgb = color_hsl_to_rgb(col_hsl);
				col_rgb = color_white_correct(col_rgb, 100, 255, 10);
				printf("Color RGB: %0x %0x %0x\n", col_rgb.R, col_rgb.G, col_rgb.B);
				if(mode == WAVE) {
					color_write(col_rgb);
				}
				break;
			
			case 'W':
				//puts("got mini wave");
				
				if(0);
				packet_mini_wave_t mini_wave;
				size = fread(&mini_wave, sizeof(packet_mini_wave_t), 1, stdin);
				if(size != 1) {
					printf("Unexpected EOF: %d\n", size);
					fflush(stdin);
					break;
				}
				
				wave.wave[0] = mini_wave.bass;
				wave.vol = mini_wave.vol;
				
				col_hsl = wave_color(wave);
				//printf("Color HSL: %0d %0d %0d\n", (int)col_hsl.H, (int)(col_hsl.S*100), (int)(col_hsl.L*100));
				
				col_rgb = color_hsl_to_rgb(col_hsl);
				//col_rgb = color_white_correct(col_rgb, 255, 100, 80);
				col_rgb = color_white_correct(col_rgb, 100, 255, 10);
				//printf("Color RGB: %0x %0x %0x\n", col_rgb.R, col_rgb.G, col_rgb.B);
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
				PORTB ^= _BV(PORTB5);
		}
		
		//fflush(stdin);
	}

	return 0;
}