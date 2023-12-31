#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"
#include "constants.h"

void move_left_forward(uint8_t speed);
void move_left_reverse(uint8_t speed);
void move_right_forward(uint8_t speed);
void move_right_reverse(uint8_t speed);

uint8_t mappedLeftSpeed, mappedRightSpeed;

// Takes the 3 bit magnitude of speed as argument and convert to PWM value
uint8_t convert_speed_PWM(uint8_t rawSpeed) {
	if (rawSpeed == 0) return 0;
	
	const float LOWER_PWM = 50.0;
	const float UPPER_PWM = 100.0;
	
	float range = UPPER_PWM - LOWER_PWM;
	return (uint8_t) LOWER_PWM + range * rawSpeed / 7.0;
}

void motor_control(void *argument) {
	for (;;) {
		// Convert input -7 ~ 7 input speed to PWM values
		mappedLeftSpeed = convert_speed_PWM(getMagnitude(leftMotorSpeed));
		mappedRightSpeed = convert_speed_PWM(getMagnitude(rightMotorSpeed));
		
		// Set LEFT motor speed
		if (isSerialPositive(leftMotorSpeed)) {
			move_left_forward(mappedLeftSpeed);
		} else {
			move_left_reverse(mappedLeftSpeed);
		}
		
		// Set RIGHT motor speed
		if (isSerialPositive(rightMotorSpeed)) {
			move_right_forward(mappedRightSpeed);
		} else {
			move_right_reverse(mappedRightSpeed);
		}
	}
}

void move_left_forward(uint8_t speed) {
	float percent = speed / 100.0;
	TPM1_C0V = 0;
	TPM1_C1V = MOTOR_PERIOD_TICKS * percent;
	// TPM2_C0V = MOTOR_PERIOD_TICKS * percent;
	// TPM2_C1V = 0;
}

void move_left_reverse(uint8_t speed) {
	float percent = speed / 100.0;
	TPM1_C0V = MOTOR_PERIOD_TICKS * percent;
	TPM1_C1V = 0;
	// TPM2_C0V = 0;
	// TPM2_C1V = MOTOR_PERIOD_TICKS * percent;
}

void move_right_forward(uint8_t speed) {
	float percent = speed / 100.0;
	// TPM1_C0V = MOTOR_PERIOD_TICKS * percent;
	// TPM1_C1V = 0;
	TPM2_C0V = MOTOR_PERIOD_TICKS * percent;
	TPM2_C1V = 0;
}

void move_right_reverse(uint8_t speed) {
	float percent = speed / 100.0;
	// TPM1_C0V = 0;
	// TPM1_C1V = MOTOR_PERIOD_TICKS * percent;
	TPM2_C0V = 0;
	TPM2_C1V = MOTOR_PERIOD_TICKS * percent;
}

void init_motor() {
	// Enable clock for PortB GPIO
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Enable clock for timer1, timer2 (motors)
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;

  // Enable timer function for all 4 motor pins
  PORTB->PCR[MOTOR_FRONT_LEFT] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[MOTOR_FRONT_LEFT] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[MOTOR_FRONT_RIGHT] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[MOTOR_FRONT_RIGHT] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[MOTOR_BACK_LEFT] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[MOTOR_BACK_LEFT] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[MOTOR_BACK_RIGHT] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[MOTOR_BACK_RIGHT] |= PORT_PCR_MUX(3);
	
	// Select internal clock as clock source for timer
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// TIMER1: Set prescaler of timer
  TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
  TPM1->SC &= ~(TPM_SC_CPWMS_MASK); // Up-counting only mode

  TPM1_C0SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses, Edge-aligned PWM
	
	TPM1_C1SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses, Edge-aligned PWM
	
	// TIMER2: Set prescaler of timer
  TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
  TPM2->SC &= ~(TPM_SC_CPWMS_MASK); // Up-counting only mode

  TPM2_C0SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses, Edge-aligned PWM
	
	TPM2_C1SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses, Edge-aligned PWM
	
	// Set PWM period for motor pins (timer1 and timer2)
  TPM1->MOD = MOTOR_PERIOD_TICKS;
	TPM2->MOD = MOTOR_PERIOD_TICKS;
}
