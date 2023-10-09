#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"
#include "constants.h"

void UART2_IRQHandler(void) {
  NVIC_ClearPendingIRQ(UART2_IRQn);
	
  // Ready to receive new byte data
  if (UART2->S1 & UART_S1_RDRF_MASK) {
    serialData = UART2->D;
		
		// Trigger main loop to decode
		osSemaphoreRelease(decodeDataSem); 
  }
	
  // Error case
  if (UART2->S1 &(UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
    uint8_t dummy = UART2->D;
  }
}

void init_serial(uint32_t baud_rate) {
	uint32_t divisor;

  // Enable clock to UART and PortE
  SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Set PTE22 & PTE23 to UART mode
  // PORTE->PCR[SERIAL_TX] |= PORT_PCR_MUX(4);
  PORTE->PCR[SERIAL_RX] |= PORT_PCR_MUX(4);

  // Disable TX and RX before config
  UART2->C2 &= ~(UARTLP_C2_TE_MASK | UARTLP_C2_RE_MASK);

  // Set baud rate to 4800 bps
  int BUS_CLOCK = DEFAULT_SYSTEM_CLOCK / 2; 
  divisor = BUS_CLOCK / (16 * baud_rate);
  UART2->BDH = UART_BDH_SBR(divisor >> 8);
  UART2->BDL = UART_BDL_SBR(divisor);

  // No parity, 8 bits, 2 stop bits, other settings
  UART2->C1 = UART2->S2 = UART2->C3 = 0;
  
  // Enable transmitter and receiver
  UART2->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;

	// Setup interrupts for UART
  NVIC_SetPriority(UART2_IRQn, 128);
  NVIC_ClearPendingIRQ(UART2_IRQn);
  NVIC_EnableIRQ(UART2_IRQn);
}
