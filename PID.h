/*
 * PID.h
 *
 * Created: 15/05/2016 20:48:06
 *  Author: angel
 */ 


#ifndef PID_H_
#define PID_H_

#include "header.h"

	/*
	– Variável de referência, 
	– Variável controlada,
	– Variável de erro,
	– Variável de comando,
	– Valores de saturação da variável de comando,
	– Período de amostragem,						H
	– Ganho proporcional,							KP
	– Ganho integral,								KI
	– Ganho derivativo (Td)							
	– Ganho do filtro (Tf),						freq de corte 1/Tf
		*/


/*input parameters*/
volatile float Kp;
volatile float Ki;
volatile float Td;
volatile float Tf;
volatile float h;							/*sampling period*/
volatile float Yrp;							/*output ref*/
volatile float u_0;								/* */
volatile unsigned char flag_operation;		/*1 on, 0 off*/

/*operational parameters*/
volatile float Kph;							/*operational proportional gain*/
volatile float Kih;							/*operational integral gain*/
volatile float Kdf;							/**/
volatile float Kf;							/**/
volatile float last_error;					/*to save the error*/
volatile float sum_e;						/*error integration*/
volatile float last_u_d;					/*results from the derivate process*/
volatile float error;								/*to calculate the error*/
volatile float u_d;									/*u derivate*/
volatile float u;									/*output*/
volatile float u_sat_up;							/*upper saturation limit*/
volatile float u_sat_down;						/*down saturation limit*/
volatile int out_put;


void pid_init();	/*initiation of the parameters*/
void pid_reset();	/*rest to all the variables*/
void pid_function(float y);
void config_output();

#endif /* PID_H_ */