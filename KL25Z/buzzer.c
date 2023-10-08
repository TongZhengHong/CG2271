#include "MKL25Z4.h"                    // Device header

void buzzer_control(void *argument) {
	// Checks current state of program and play the corresponding music
}

void set_frequency(int freq) {
	// 48 Mhz / prescaler
	int width = 375000 / freq;
	
	TPM0->MOD = width - 1;
	TPM0_C1V = width / 2;
}