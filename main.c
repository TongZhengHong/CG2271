#include "MKL25Z4.h"                    // Device header
#include "RTE_Components.h"             // Component selection
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "constants.h"
#include CMSIS_device_header

volatile uint8_t serialData;
enum state currentState = STOP;

// Decode data from serial port 
// Decode state robot currently in, and read the speed/directions
void app_main(void *argument) {
	for (;;) {
		
	}
}

int main() {
	SystemCoreClockUpdate();
	init_led();
	init_pwm();
	init_serial(9600);
	
	osKernelInitialize();
	osThreadNew(app_main, NULL, NULL);
	osThreadNew(motor_control, NULL, NULL);
	osThreadNew(buzzer_control, NULL, NULL);
	osThreadNew(led_control, NULL, NULL);
	osKernelStart();
}