#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "constants.h"

volatile uint8_t serialData;
volatile osSemaphoreId_t decodeDataSem;

enum state currentState = STOP;
uint8_t leftMotorSpeed, rightMotorSpeed;

/*
Packet decoding

First 4 bits: left motors
Last 4 bits: right motors

7 possible speeds + forward/reverse

0000 = 0

0001 ~ 0111 = +1 ~ +7
1000: Ignored for ease of processing

1001 ~ 1111 = -1 ~ -7

*/

// Decode incoming serial data from UART2 port and determine the state
void app_main (void *argument) {
	
  for (;;) {
		osSemaphoreAcquire(decodeDataSem, osWaitForever);
		
		leftMotorSpeed = serialData >> 4; // Extract uppper 4 bits
		rightMotorSpeed = serialData & 0b00001111; // Clear upper 4 bits
		
		if (serialData == END_COMMAND) {
			currentState = END;
			leftMotorSpeed = 0;
			rightMotorSpeed = 0;
			
		} else if (leftMotorSpeed == 0 && rightMotorSpeed == 0) {
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
