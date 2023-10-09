#include "MKL25Z4.h"                    // Device header
#include "constants.h"
#include "math.h"

void init_led() {
	
}

void init_pwm() {
	// Enable clock for PortB GPIO
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

  // Enable timer function for all 4 motor pins
  PORTB->PCR[MOTOR_FRONT_LEFT] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[MOTOR_FRONT_LEFT] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[MOTOR_FRONT_RIGHT] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[MOTOR_FRONT_RIGHT] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[MOTOR_BACK_LEFT] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[MOTOR_BACK_LEFT] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[MOTOR_BACK_RIGHT] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[MOTOR_BACK_RIGHT] |= PORT_PCR_MUX(3);
	
	// Enable clock for PortC GPIO
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	// Enable timer function for buzzer pin
	PORTB->PCR[BUZZER_PIN] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[BUZZER_PIN] |= PORT_PCR_MUX(4);

  // Enable clock for timer1, timer2 (motors) and timer0 (buzzer)
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;
	
	// Select internal clock as clock source for timer
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// TIMER1: Set prescaler of timer = 16
  TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
  TPM1->SC &= ~(TPM_SC_CPWMS_MASK); // Up-counting only mode

  TPM1_C0SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses, Edge-aligned PWM
	
	// TIMER2: Set prescaler of timer = 16
  TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
  TPM2->SC &= ~(TPM_SC_CPWMS_MASK); // Up-counting only mode

  TPM2_C0SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses, Edge-aligned PWM
	
	// TIMER0: Set prescaler of timer = 16
  TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
  TPM0->SC &= ~(TPM_SC_CPWMS_MASK); // Up-counting only mode

  TPM0_C0SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses, Edge-aligned PWM
	
	// Set PWM period for motor pins (timer1 and timer2)
  TPM1->MOD = MOTOR_PERIOD_TICKS;
	TPM2->MOD = MOTOR_PERIOD_TICKS;
	
	TPM1_C0V = MOTOR_PERIOD_TICKS / 2;
	TPM1_C1V = 0;
	TPM2_C0V = MOTOR_PERIOD_TICKS / 2;
	TPM2_C1V = 0;
}
