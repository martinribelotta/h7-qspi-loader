/**
 ******************************************************************************
 * File Name          : QUADSPI.h
 * Description        : This file provides code for the configuration
 *                      of the QUADSPI instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __quadspi_H
#define __quadspi_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */

extern size_t MX_QUADSPI_Read(void *buf, uint32_t addr, size_t size);
extern int MX_QUADSPI_Erase(uint32_t addr, size_t size);
extern int MX_QUADSPI_EraseAll(void);
extern int MX_QUADSPI_Write(const void *buf, uint32_t addr, size_t size);
extern int MX_QUADSPI_EnterMMAP(void);
extern void MX_QUADSPI_ExitMMAP(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ quadspi_H */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
