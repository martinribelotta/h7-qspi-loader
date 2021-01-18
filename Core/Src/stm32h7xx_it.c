#include "main.h"
#include "stm32h7xx_it.h"

static void hang(void) {
  for(;;);
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  hang();
}

void MemManage_Handler(void)
{
  hang();
}

void BusFault_Handler(void)
{
  hang();
}

void UsageFault_Handler(void)
{
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
