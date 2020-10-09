/*
 * appl.c
 *		Motor Control System:-
 *			- Motor Direction Control over Push Button; INT2 pin.
 *			- Motor Speed Control over Potentiometer; ADC7 pin.
 *			- Speed Display over LCD  connected over PORTD and PORTC.
 *			- Motor Connection through Driver:-
 *				- Input Pins (PB5 and PB4 pins).
 *				- PWM Input 'Speed control' (OC0 pin).
 *
 *  Created on: Sep 23, 2020
 *      Author: Ahmed Montasser
 */
#include "ADC.h"
#include "LCD.h"
#include "motor_dc.h"
#include "external_interrupt.h"

void motorDirectionButton_ISRcallBackFunc(void);

volatile MOTOR_config motor_config = {0, FORWARD_DIR};

int main(void)
{
	uint8 lcd_string[15] = "Motor Speed : ";

	__ADC_init(ADC_F_CPU_8);
	_LCD_init();
	_LCD_displayString(lcd_string);

	__INT2_init(RISING_EDGE);
	__INT2_setISRCallBackFuncPointer(motorDirectionButton_ISRcallBackFunc);

	while(TRUE)
	{
		motor_config.speed = ((float32)__ADC_PB_readChannel_8bits(ADC7))/255*100;
		_LCD_adjustCursorPosition(1,0);
		_LCD_displayIntegerVariable(motor_config.speed);
		if(motor_config.speed < 100)
		{
			_LCD_displayCharacter(' ');
		}
		_DCMOTOR_motorOn(&motor_config);
		APPROX_HALF_SEC_DELAY;
	}
	return 0;
}

void motorDirectionButton_ISRcallBackFunc(void)
{
	if(motor_config.direction == BACKWARD_DIR)
	{
		motor_config.direction = FORWARD_DIR;
	}
	else
	{
		motor_config.direction = BACKWARD_DIR;
	}

}
