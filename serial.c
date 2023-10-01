#include "MKL25Z4.h"                    // Device header
#include "constants.h"

void UART2_IRQHandler(void) {
  NVIC_ClearPendingIRQ(UART2_IRQn);

  // Ready to send next byte data
  if (UART2->S1 & UART_S1_TDRE_MASK) {
    UART2->D = serialData;
  }
  // Ready to receive new byte data
  if (UART2->S1 & UART_S1_RDRF_MASK) {
    serialData = UART2->D;
  }
  // Error case
  if (UART2->S1 &(UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
    uint8_t dummy = UART2->D;
  }
}