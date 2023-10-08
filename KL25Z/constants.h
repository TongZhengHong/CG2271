enum state {
  MOVING, STOP, END
};

#define TIMER_PRESCALER 4 // prescalar = 16
#define MOTOR_PWM_FREQ 25000 // 25 kHz to reduce noise
// 48 MHz clock / 16 prescaler = 3 MHz. For 25 kHz, reset counter at 120
int MOTOR_PERIOD_TICKS = 48000000 / TIMER_PRESCALER / MOTOR_PWM_FREQ;

// Motor pins
#define MOTOR_FRONT_LEFT 	0 // PTB0: TPM1_CH0
#define MOTOR_FRONT_RIGHT 1 // PTB1: TPM1_CH1
#define MOTOR_BACK_LEFT 	2 // PTB2: TPM2_CH0
#define MOTOR_BACK_RIGHT 	3 // PTB3: TPM2_CH1

// Buzzer pin
#define BUZZER_PIN 2 // PTC2: TPM0_CH1

// Serial pins
#define SERIAL_TX 22 // PTE22: UART2_TX
#define SERIAL_RX 23 // PTE23: UART2_RX

// LED pins
// ??? 

// Global varables shared across c files
extern volatile unsigned char serialData;
extern enum state currentState;

// Initialisation function declarations
void init_led();
void init_pwm();
void init_serial(unsigned int baud_rate);

// Thread function declarations
void motor_control(void *argument);
void buzzer_control(void *argument);
void led_control(void *argument);
