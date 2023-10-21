#include "MKL25Z4.h"                    // Device header
#include "constants.h"

void set_green_seq(uint8_t x);
uint8_t green_counter = 0;

void led_control(void *argument) {
	for (;;) {
		if (currentState == MOVING) {
			// Green continuous, Red all toggle 500ms
			set_green_seq(MASK(green_counter));
			green_counter++;
			if (green_counter == 8) {
				green_counter = 0; // Reset green led position
			}
			
			// Toggle RED led every 500ms
			PTE->PTOR = MASK(RED_PTE22);
			osDelay(500);
			
		} else { // STOP or END states
			// Turn on ALL green leds
			PTB->PDOR |= (MASK(GREEN_PTB8) | MASK(GREEN_PTB9) | MASK(GREEN_PTB10) | MASK(GREEN_PTB11));
			PTE->PDOR |= (MASK(GREEN_PTE2) | MASK(GREEN_PTE3) | MASK(GREEN_PTE4) | MASK(GREEN_PTE5));
			
			// Toggle RED led every 250ms
			PTE->PTOR = MASK(RED_PTE22);
			osDelay(250);
		}
	}
}

void set_green_seq(uint8_t sequence) {
	uint8_t portE = sequence >> 4;
	uint8_t portB = sequence &= 0b00001111; // Clear upper 4 bits
	
	char PORTB_LED_SHIFT = 8;
	char PORTE_LED_SHIFT = 2;
	
	// Turn OFF green leds
	PTB->PCOR |= (0b1111 << PORTB_LED_SHIFT);
	PTE->PCOR |= (0b1111 << PORTE_LED_SHIFT);
	
	// Turn ON green leds
	PTB->PSOR |= portB << PORTB_LED_SHIFT;
	PTE->PSOR |= portE << PORTE_LED_SHIFT;
}

void init_led() {
	// Enable clock for PortB and PortE GPIO
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;
	
	// Configure MUX settings for PORTB GPIO pins
	PORTB->PCR[GREEN_PTB8] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_PTB9] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_PTB10] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_PTB11] |= PORT_PCR_MUX(1);
	
	// Configure MUX settings for PORTE GPIO pins
	PORTE->PCR[RED_PTE22] |= PORT_PCR_MUX(1);
	PORTE->PCR[GREEN_PTE2] |= PORT_PCR_MUX(1);
	PORTE->PCR[GREEN_PTE3] |= PORT_PCR_MUX(1);
	PORTE->PCR[GREEN_PTE4] |= PORT_PCR_MUX(1);
	PORTE->PCR[GREEN_PTE5] |= PORT_PCR_MUX(1);
	
	// Set PORTB output pins
	PTB->PDDR |= (MASK(GREEN_PTB8) | MASK(GREEN_PTB9) | MASK(GREEN_PTB10) | MASK(GREEN_PTB11));
	
	// Set PORTE output pins
	PTE->PDDR |= MASK(RED_PTE22);
	PTE->PDDR |= (MASK(GREEN_PTE2) | MASK(GREEN_PTE3) | MASK(GREEN_PTE4) | MASK(GREEN_PTE5));
}