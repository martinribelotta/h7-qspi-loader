#include "semihosting.h"

extern void SysTick_Handler(void);
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);

extern unsigned int FlashAlgo;

extern unsigned int _estack;
extern unsigned int _sbss;
extern unsigned int _ebss;

extern int main(void);
extern void SystemInit(void);

void hang_handler(void)
{
    for(;;);
}

void Reset_Handler(void)
{
    SystemInit();
    for (unsigned int *ptr = &_sbss; ptr < &_ebss; ptr++) {
        *ptr = 0;
    }
    host_halt(main());
    __builtin_unreachable();
}

void *g_pfnVectors[] __attribute__((section(".isr_vector"))) = {
    &_estack,                /* 0x00 */
    Reset_Handler,          /* 0x04 */
    NMI_Handler,            /* 0x08 */
    HardFault_Handler,      /* 0x0C */
    MemManage_Handler,      /* 0x10 */
    BusFault_Handler,       /* 0x14 */
    UsageFault_Handler,     /* 0x18 */
    &FlashAlgo,             /* 0x1C */
    0,                      /* 0x20 */
    0,                      /* 0x24 */
    0,                      /* 0x28 */
    SVC_Handler,            /* 0x2C */
    DebugMon_Handler,       /* 0x30 */
    0,                      /* 0x34 */
    PendSV_Handler,         /* 0x38 */
    SysTick_Handler,        /* 0x3C */
  
};
