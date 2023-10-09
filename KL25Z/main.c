#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "constants.h"

volatile uint8_t serialData;
volatile osSemaphoreId_t decodeDataSem;

// Decode incoming serial data from UART2 port and determine the state
void app_main (void *argument) {
  for (;;) {
		osSemaphoreAcquire(decodeDataSem, osWaitForever);
		// Decode here...
	}
}
 
int main (void) {
  // System Initialization
	init_motor();
	init_buzzer();
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
