#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"
#include "constants.h"

void UART2_IRQHandler(void) {
  NVIC_ClearPendingIRQ(UART2_IRQn);
	
  // Ready to receive new byte data
  if (UART2->S1 & UART_S1_RDRF_MASK) {
		// Receive incoming data
    serialData = UART2->D;
		
		// Trigger main loop to decode
		osSemaphoreRelease(decodeDataSem); 
  }
	
  // Error case
  if (UART2->S1 &(UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
    uint8_t dummy = UART2->D;
  }
}

// Returns the lower 3 bits of serial data (Ignore MSB)
uint8_t getMagnitude(uint8_t rawSpeed) {
	return rawSpeed & 0b111;
}

// Checks 4th bit of serial data to check POSITIVE
char isSerialPositive(uint8_t rawSpeed) {
	return !(rawSpeed & 0b1000); // MSB = 0
}

// Checks 4th bit of serial data to check NEGATIVE
char isSerialNegative(uint8_t rawSpeed) {
	return rawSpeed & 0b1000; // MSB = 1
}

// Sets up UART Receiver (TX not needed)
void init_serial(uint32_t baud_rate) {
	uint32_t divisor;

  // Enable clock to UART and PortE
  SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Set PTE23 to UART mode
	PORTE->PCR[SERIAL_RX] &= ~PORT_PCR_MUX_MASK; // Clear PCR Mux (for choosing pin function
  PORTE->PCR[SERIAL_RX] |= PORT_PCR_MUX(4);

  // Disable RX before config
  UART2->C2 &= ~UART_C2_RE_MASK;

  // Set baud rate to 4800 bps
  uint32_t BUS_CLOCK = DEFAULT_SYSTEM_CLOCK / 2; 
  divisor = BUS_CLOCK / (16 * baud_rate);
  UART2->BDH = UART_BDH_SBR(divisor >> 8);
  UART2->BDL = UART_BDL_SBR(divisor);

  // No parity, 8 bits, 2 stop bits, other settings
  UART2->C1 = UART2->S2 = UART2->C3 = 0;
  
  // Enable UART receiver
  UART2->C2 |= UART_C2_RE_MASK;

	// Setup interrupts for UART
  NVIC_SetPriority(UART2_IRQn, 128);
  NVIC_ClearPendingIRQ(UART2_IRQn);
  NVIC_EnableIRQ(UART2_IRQn);
	
	// Enable hardware interrupts for receive	
	UART2->C2 |= UART_C2_RIE_MASK; 
}
