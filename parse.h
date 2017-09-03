/*
 * parse_read.h
 *
 * Created: 22/02/2016 22:13:35
 *  Author: angel
 */ 


#ifndef PARSE_READ_H_
#define PARSE_READ_H_

#include <avr/io.h>
#include "header.h"

volatile unsigned int secounds_dec;					//when is a sample period with seconds
volatile unsigned int old_secounds_dec;				//when is a sample period with seconds
volatile unsigned char flag_operation;

/*functions*/
void parse(void);
void def_sampling_period(void);
void def_proprtional_gain(void);
void def_integral_gain(void);
void def_derivative_gain(void);
void def_filter_gain(void);
void def_ref_value(void);
void def_u0(void);

#endif /* PARSE_READ_H_ */