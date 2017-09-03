/*
 * CFile1.c
 *
 * Created: 22/02/2016 22:13:04
 *  Author: angel
 */ 


#include "header.h"

volatile unsigned char digital_flag;
extern volatile char parse_index;
extern volatile char received_comand[];					//command to be executed
extern char str_read(void);
extern void parse_read();

/*external variables from PID*/
extern volatile float h;
extern volatile float Kp;
extern volatile float Ki;
extern volatile float Td;
extern volatile float Tf;
extern volatile float u_0;

void parse(void)
{
	digital_flag = 0;

	char temp = str_read();
	switch(temp)
	{
		/*sampling period*/
		case 'h':
		case 'H':
			def_sampling_period();
		break;

		/*Proportional Gain*/
		case 'k':
		case 'K':
			temp = str_read();
			if((temp == 'p') || (temp =='P'))		def_proprtional_gain();
			else if((temp == 'i') || (temp == 'I'))	def_integral_gain();
			else serial_error();
		break;
		
		case 't':
		case 'T':
			temp = str_read();
			if((temp == 'd') || (temp == 'D'))		def_derivative_gain();
			else if((temp == 'f') || (temp == 'F'))		def_filter_gain();
			else	serial_error();
		break;
		
		case 'o':
		case 'O':
			temp = str_read();
			if((temp == 'p') || (temp == 'P'))
			{
				temp = str_read();
				if(temp == '\0')
				{
					flag_operation = !flag_operation;		/*on off*/
					pid_init();
					config_output();
				}
				else serial_error();		
			}
			if(temp == '0')
			def_u0();
			else serial_error();
		break;
		
		case 'r':
		case 'R':
			def_ref_value();
		break;
		
		default:
			serial_error();
		break;
	} /*end main switch*/
} /*end function*/

void def_ref_value()
{
	unsigned int temp = str_read();
	unsigned char control_digit1 = str_read();			//get 0x20
	unsigned int temp_yrp = parse_number();					//Krp
	unsigned char control_digit2 = str_read();			//get '\0'
	
	if( ( (temp =='e') || (temp == 'E')) && (temp_yrp>=0) && (temp_yrp<=255) && (control_digit1==0x20) && (control_digit2=='\0') )
	{
		Yrp = (float)temp_yrp;
	}
	else
	serial_error();
	
}

void def_sampling_period()
{
	unsigned char control_digit1 = str_read();			//get 0x20
	unsigned char temp_units = parse_number();			//get 1,2,3 time units -> ys, ms, s
	unsigned char control_digit2 = str_read_last();			//get 0x20
	unsigned int samp_period = parse_number();			//get the time of sampling
	unsigned char control_digit3 = str_read();			//get '\0'
	
	if( (temp_units >= 0x00) && (temp_units <= 0x03) && (samp_period >= 0) && (samp_period <= 0xE38) && (control_digit1 == 0x20) &&
	(control_digit2 == 0x20) && (control_digit3 == '\0') )
	{
		secounds_dec = 0;
		old_secounds_dec = 0;
	
		if(temp_units == 1) /*ys*/
		{
			if(samp_period >= 100)
			{
				h = (float)samp_period*0.000001;	/*convert to ms*/
				TCCR1B &= ~0x07;			/*reset the prescaler and stop*/
				TCNT1 = 0;				/*counter of the timer to zero*/
				OCR1B  =  (samp_period * 2) ;
				TCCR1B = 0x02;			/*makes 0.5 ys*/
			}
		if(samp_period < 100)
			serial_writestr("too low sampling period\n");
		}/*ys*/
		
		if(temp_units == 2) /*ms*/
		{
			h = (float)samp_period*0.001;
			TCCR1B &= ~0x07;			/*reset the prescaler and stop*/
			TCNT1 = 0;				/*counter of the timer to zero*/
			OCR1B = ((int)((float)(samp_period*65535)/1048));
			TCCR1B = 0x04;	/* fclock/256 -> makes 16ys each machine cycle*/
		}/*ms*/
		
		if(temp_units == 3) /*s*/
		{
			h = ((float)samp_period);		/*convert to ms*/
			TCCR1B &= ~0x07;
			TCNT1 = 0;
			secounds_dec = samp_period;
			old_secounds_dec = samp_period;
			OCR1B = 15625;	/*makes 1 secound*/
			TCCR1B = 0x05;	/*fclock/1024 -> makes 64ys*/
		}/*s*/
	}/*end if*/
	else serial_error();	
} /*end function*/

void def_proprtional_gain()
{
	unsigned char control_digit1 = str_read();			//get 0x20
	unsigned int gain = parse_number();					//Krp
	unsigned char control_digit2 = str_read();			//get '\0'
	
	if( (gain>=0) && (gain<=100) && (control_digit1==0x20) && (control_digit2=='\0') )
	{
		Kp = (float)gain;
	}
	else
		serial_error();
}/*end function*/

void def_integral_gain()
{
	unsigned char control_digit1 = str_read();			//get 0x20
	unsigned int gain = parse_number();					//Krp
	unsigned char control_digit2 = str_read();			//get '\0'
		
	if( (gain>=0) && (gain<=100) && (control_digit1==0x20) && (control_digit2=='\0') )
	{
		Ki = (float)gain;
	}
	else
	serial_error();
}
void def_derivative_gain()
{
	unsigned char control_digit1 = str_read();			//get 0x20
	unsigned int gain = parse_number();					//Krp
	unsigned char control_digit2 = str_read();			//get '\0'
	
	if( (gain>=0) && (gain<=25) && (control_digit1==0x20) && (control_digit2=='\0') && ( ((float)gain < (1 / (4*Ki) )) || (Ki == 0)) )
	{
		Td = (float)gain;
	}
	else
	serial_error();
}
void def_filter_gain()
{
	unsigned char control_digit1 = str_read();			//get 0x20
	unsigned int gain = parse_number();					//Krp
	unsigned char control_digit2 = str_read();			//get '\0'
	
	if( (gain>=0) && ( (float)gain <= (0.03*Td)) && (control_digit1==0x20) && (control_digit2=='\0') )
	{
		Tf = (float)gain;
	}
	else
	serial_error();	
}

void def_u0()
{
	unsigned char control_digit1 = str_read();			//get 0x20
	unsigned int gain = parse_number();					//Krp
	unsigned char control_digit2 = str_read();			//get '\0'
	
	if( (gain>=0) && (gain <= 100) && (control_digit1==0x20) && (control_digit2=='\0') )
		u_0 = (float)gain;
	else
	serial_error();	
}