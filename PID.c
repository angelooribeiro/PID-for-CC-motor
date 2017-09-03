/*
 * PID.c
 *
 * Created: 15/05/2016 20:47:54
 *  Author: angel
 */ 

#include "PID.h"

/*initiation of the parameters*/
void pid_init()
{
	Yrp = 2.5;
	Kp = 0;
	Ki = 40;
	Td = 0;
	Kih = Ki*h;
	Kph = Kp/h;
	Kdf = (Td/(Tf+h));
	Kf = (Tf/(Tf+h));
	u_sat_down = 0;
	u_sat_up = 0xFFFF;
	last_error = 0;					/*to save the error*/
	sum_e = 0;						/*error integration*/
	last_u_d = 0;
}



void pid_function(float y)
{	
	y = (float)((y*5)/1024);
	error = Yrp - y;
	if(flag_operation)
	{
		//u_d =( (Kdf*(error-last_error)) + (Kf*last_u_d) ) ;
		//u = ( (Kp*error) + (Kih*sum_e) + u_d);
		sum_e = sum_e+error;
		u= Kih*sum_e;
		last_error = error;
		last_u_d = u_d;
		//if(u>u_sat_up)	u=u_sat_up;
		//else if(u<u_sat_down)	u=u_sat_down;
		sum_e = sum_e+error;
		out_put = (int)u;
		serial_write(out_put>>8);
		serial_write(out_put);
	}
	else
	last_error = error;
}  

void config_output()
{
	/*pwm PH3 pin6*/
	TCCR4A |= (1<<WGM42) | (1<<WGM40) | (1<<COM4A1) ;	/*fast pwm 8 bists compare output on A*/
	TCCR4B |= (1<<WGM42) | (1<<CS40);					/*FOC0A: Force Output Compare A*/
	TIMSK4 |= (1<<OCIE4A);								/*Output Compare Match A Interrupt Enable*/
	DDRH |= (1<<PH3);	
}

ISR(TIMER4_COMPA_vect)
{
	OCR4A = out_put;
}