/*
 * adc.c
 *
 * Created: 07/03/2016 16:32:52
 *  
 */ 

#include "adc.h"

extern volatile unsigned char flag_operation;


void config_timer1()
{
		TCCR1A = (1<<COM1B0);  	/*CTC mode*/
		TCCR1B |= (1<<WGM12) ;
		TIMSK1 |= (1 << OCIE1B);
}

/*ADC default Configration*/
void adc_configuration()
{
	/*runs in free mode*/
	ADMUX |= (1 << REFS0);	/*default to avcc amd pin0 input*/
 	/*enables adc; adc auto trigger enable;  adc start conversion;  activates the interrupt*/
	 /*fosc/128 -> 8ys take a sample*/
	ADCSRA |= (1 << ADATE) | (1<<ADEN) | (1 << ADIE) | (1<<ADSC) | (1<<ADPS1) | (1<<ADPS0);

}

/*ADC interrupt response*/
ISR(ADC_vect)
{
	read_flag = 1;
	sample = ADC;
	ADCSRA |= (1<<ADIF); /*ACK interrupt flag*/
	read_flag = 0;
}

ISR(TIMER1_COMPB_vect)
{	
	if(flag_operation)	/*if its in operation mode*/
	{
		if(secounds_dec > 0)			/*decrements when operation takes more than 1 second*/
			secounds_dec = secounds_dec - 1;
		else
		{
			while(read_flag);
			pid_function((float)sample);
			TCNT1 = 0;	/*reset the value of the counter*/
		}
	}/*end if*/
} /*end isr*/