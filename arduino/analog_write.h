#ifndef _ANALOG_WRITE_H_
#define	_ANALOG_WRITE_H_

void digital_write(volatile int *data_port, int pin, int value);
void analog_write(uint8_t val, volatile uint8_t *ocr, volatile uint8_t *port, uint8_t pin, volatile uint8_t *tcr, uint8_t tcr_bit);

#endif