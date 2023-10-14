#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "constants.h"

volatile uint8_t serialData;
volatile osSemaphoreId_t decodeDataSem;

enum state currentState = STOP;

int8_t leftMotorSpeed;
int8_t rightMotorSpeed;

/*
Packet decoding

First 4 bits: left motors
Last 4 bits: right motors

8 possible speeds + forward/reverse

0000 = 0

0001 ~ 0111 = +1 ~ +7
1000 = +8 -> For more forward values

1001 ~ 1111 = -1 ~ -7

*/

// Convert raw speed to signed speed value
int8_t decode_speed(uint8_t rawSpeed) {
	char isMax = rawSpeed == 0b1000;
	char isForward = !(rawSpeed & 0b1000); // MSB = 0
	char isReverse = rawSpeed & 0b1000; // MSB = 1
	
	if (isMax || isForward) {
		// Max forward speed or positive speed
		return (int8_t) rawSpeed;
		
	} else if (isReverse) { 
		// Negative speed = reverse
		uint8_t magnitude = rawSpeed &= 0b0111; // Clear MSB for magnitude
		return ((int8_t) magnitude) * -1;
	}
	
	return 0; // Stop (speed = 0) by default
}

// Decode incoming serial data from UART2 port and determine the state
void app_main (void *argument) {
  for (;;) {
		osSemaphoreAcquire(decodeDataSem, osWaitForever);
		
		uint8_t rawLeft = serialData >> 4; // Extract uppper 4 bits
		uint8_t rawRight = serialData & 0b00001111; // Clear upper 4 bits
		
		leftMotorSpeed = decode_speed(rawLeft);
		rightMotorSpeed = decode_speed(rawRight);
		
		if (serialData == END_COMMAND) {
			currentState = END;
			leftMotorSpeed = 0;
			rightMotorSpeed = 0;
			
		} else if (rawLeft == 0 && rawRight == 0) {
			currentState = STOP;
		} else {
			currentState = MOVING;
		}
	}
}
 
int main (void) {
  // System Initialization
	init_motor();
	init_buzzer();
	init_led();
	init_serial(9600);
	
	const osThreadAttr_t main_thread_attr = {
		.priority = osPriorityAboveNormal1
	};
	
	osKernelInitialize();
	decodeDataSem = osSemaphoreNew(1, 0, NULL);
	osThreadNew(app_main, NULL, &main_thread_attr);
	osThreadNew(motor_control, NULL, NULL);
	osThreadNew(buzzer_control, NULL, NULL);
	osThreadNew(led_control, NULL, NULL);
	osKernelStart();
}
