/*
 * adc.h
 *
 * Created: 07/03/2016 16:33:06
 *  Author: Nuno
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "header.h"

volatile int sample;
volatile unsigned int temp_sample;
volatile unsigned char read_flag;

void config_timer1(void);			/*read the value from adc in each interrupt*/
void adc_configuration(void);		/*default configuration to adc*/ 
#endif /* ADC_H_ */