#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"
#include "constants.h"
#include "music.h"

void play_despacito();
void play_rickroll();

void play_freq(int freq);
void stop_sound();

void buzzer_control(void *argument) {
	// Checks current state of program and play the corresponding music
	for (;;) {
		play_despacito();
		osDelay(1000);
		
		play_rickroll();
		osDelay(5000);
	}
}

void play_despacito() {
	int tempo = 512;
	int wholenote = (60000 * 4) / tempo;
	
	int multiplier = 10;
	int size = sizeof(despacito_melody) / sizeof(despacito_melody[0]);
	
	for (int thisNote = 0; thisNote < size; thisNote++) {
		play_freq(despacito_melody[thisNote]);
		osDelay(wholenote / despacito_duration[thisNote]);
		
		stop_sound();
		osDelay(wholenote / despacito_duration[thisNote] * 1.3); // 1.3x normal
	}
}

void play_rickroll() {
	int tempo = 256; 
	int notes = sizeof(rickroll_melody) / sizeof(rickroll_melody[0]) / 2;
	int wholenote = (60000 * 4) / tempo;
	int divider = 0, noteDuration = 0;
	
	for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
		divider = rickroll_melody[thisNote + 1];
		
    if (divider > 0) { // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) { // dotted notes 
      noteDuration = wholenote / -divider;
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
		
		play_freq(rickroll_melody[thisNote] / 2);
		osDelay(noteDuration * 0.9); // 90% of noteDuration
		
		stop_sound();
		osDelay(noteDuration); 
	}
	
	stop_sound();
}

void play_freq(int freq) {
	if (freq == 0) {
		stop_sound();
		
	} else {
	// 48 Mhz / prescaler = 8
		int width = 6000000 / freq;
		TPM0->MOD = width - 1;
		TPM0_C1V = width / 2;
	}
}

void stop_sound() {
	TPM0_C1V = 0;
}

void init_buzzer() {
	// Enable clock for PortC GPIO and timer0 (buzzer)
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	// Enable timer function for buzzer pin
	PORTC->PCR[BUZZER_PIN] &= ~PORT_PCR_MUX_MASK;
  PORTC->PCR[BUZZER_PIN] |= PORT_PCR_MUX(4);
	
	// Select internal clock as clock source for timer
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// TIMER0: Set prescaler of timer
  TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
  TPM0->SC &= ~(TPM_SC_CPWMS_MASK); // Up-counting only mode

  TPM0_C1SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses, Edge-aligned PWM
}
