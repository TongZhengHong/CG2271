#include "cmsis_os2.h"

enum state {
  MOVING, STOP, END
};

#define TIMER_PRESCALER 3 // prescalar = 8
// PWM period for 25 kHz to prevent noise from motors
#define MOTOR_PERIOD_TICKS 119 // 48 Mhz / 8 (prescaler) / 50 kHz (PWM freq) - 1 

// Motor pins
#define MOTOR_FRONT_LEFT 	0 // PTB0: TPM1_CH0
#define MOTOR_FRONT_RIGHT 1 // PTB1: TPM1_CH1
#define MOTOR_BACK_LEFT 	2 // PTB2: TPM2_CH0
#define MOTOR_BACK_RIGHT 	3 // PTB3: TPM2_CH1

// Buzzer pin
#define BUZZER_PIN 2 // PTC2: TPM0_CH1

// Serial pins
// #define SERIAL_TX 22 // PTE22: UART2_TX
#define SERIAL_RX 23 // PTE23: UART2_RX

// LED pins
// ??? 

// Global varables shared across c files
extern volatile unsigned char serialData;
extern volatile osSemaphoreId_t decodeDataSem;
extern enum state currentState;

// Initialisation function declarations
void init_led();
void init_motor();
void init_buzzer();
void init_serial(unsigned int baud_rate);

// Thread function declarations
void motor_control(void *argument);
void buzzer_control(void *argument);
void led_control(void *argument);
