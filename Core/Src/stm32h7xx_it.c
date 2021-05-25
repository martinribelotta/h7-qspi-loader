#include "main.h"
#include "stm32h7xx_it.h"

#include "semihosting.h"

static void hang(void) {
  for(;;);
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  host_halt(1);
  hang();
}

void MemManage_Handler(void)
{
  host_halt(2);
  hang();
}

void BusFault_Handler(void)
{
  host_halt(3);
  hang();
}

void UsageFault_Handler(void)
{
  host_halt(4);
  hang();
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
