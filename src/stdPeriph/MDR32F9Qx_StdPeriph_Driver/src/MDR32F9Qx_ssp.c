/**
  ******************************************************************************
  * @file    MDR32F9Qx_ssp.c
  * @author  Phyton Application Team
  * @version V1.4.0
  * @date    01/02/2011
  * @brief   This file contains all the SSP firmware functions.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, PHYTON SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Phyton</center></h2>
  ******************************************************************************
  * FILE MDR32F9Qx_ssp.c
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_ssp.h"
#include "MDR32F9Qx_rst_clk.h"

#define ASSERT_INFO_FILE_ID FILEID__MDR32F9X_SSP_C

/** @addtogroup __MDR32F9Qx_StdPeriph_Driver MDR32F9Qx Standard Peripherial Driver
  * @{
  */

/** @defgroup SSP SSP
  * @{
  */

/** @defgroup SSP_Private_Defines SSP Private Defines
  * @{
  */

#define CR1_EN_Set                 ((uint16_t)0x0002)  /*!< SSP Enable Mask */
#define CR1_EN_Reset               ((uint16_t)0xFFFD)  /*!< SSP Disable Mask */

#if defined (USE_MDR1986VE3)
	#define SSP4_BRG_Mask			RST_CLK_UART_SSP_CLOCK_SSP4_BRG_Msk
	#define SSP4_BRG_Pos			RST_CLK_UART_SSP_CLOCK_SSP4_BRG_Pos
	#define SSP4_CLK_EN				RST_CLK_UART_SSP_CLOCK_SSP4_CLK_EN
#endif

#if defined (USE_MDR1901VC1T)
	#define SSP4_BRG_Mask			RST_CLK_SPP2_CLOCK_SSP4_BRG_Msk
	#define SSP4_BRG_Pos			RST_CLK_SPP2_CLOCK_SSP4_BRG_Pos
	#define SSP4_CLK_EN				RST_CLK_SPP2_CLOCK_SSP4_CLK_EN
#endif


/** @} */ /* End of group SSP_Private_Defines */

/** @defgroup SSP_Private_Functions SSP Private Functions
  * @{
  */

/**
  * @brief  Resets the SSPx peripheral registers to their default reset values.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values: SSP1, SSP2.
  * @retval None
  */
void SSP_DeInit(MDR_SSP_TypeDef* SSPx)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));

  SSPx->CR0   = 0;
  SSPx->CR1   = 0;
  SSPx->CPSR  = 0;
  SSPx->IMSC  = 0;
  SSPx->DMACR = 0;

  /* Clear SSP ICR[RTIC] and SSP ICR[RORIC] bits */
  SSPx->ICR = SSP_IT_RT | SSP_IT_ROR;

}

/**
  * @brief  Initializes the SSPx peripheral according to the specified
  *         parameters in the SSP_InitStruct.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  SSP_InitStruct: pointer to a SSP_InitTypeDef structure
  *         that contains the configuration information for the specified SSP peripheral.
  * @retval None
  */
void SSP_Init(MDR_SSP_TypeDef* SSPx, const SSP_InitTypeDef* SSP_InitStruct)
{
  uint32_t tmpreg;

  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_SSP_SPEED_FACTOR(SSP_InitStruct->SSP_SCR));
  assert_param(IS_SSP_SPEED_DIVIDER(SSP_InitStruct->SSP_CPSDVSR));
  assert_param(IS_SSP_MODE(SSP_InitStruct->SSP_Mode));
  assert_param(IS_SSP_WORD_LENGTH(SSP_InitStruct->SSP_WordLength));
  assert_param(IS_SSP_SPH(SSP_InitStruct->SSP_SPH));
  assert_param(IS_SSP_SPO(SSP_InitStruct->SSP_SPO));
  assert_param(IS_SSP_FRF(SSP_InitStruct->SSP_FRF));
  assert_param(IS_SSP_HARDWARE_FLOW_CONTROL(SSP_InitStruct->SSP_HardwareFlowControl));

  /* SSPx CPSR Configuration */
  SSPx->CPSR = SSP_InitStruct->SSP_CPSDVSR;

  /* SSPx CR0 Configuration */
  tmpreg = (SSP_InitStruct->SSP_SCR << SSP_CR0_SCR_Pos)
         + SSP_InitStruct->SSP_SPH
         + SSP_InitStruct->SSP_SPO
         + SSP_InitStruct->SSP_FRF
         + SSP_InitStruct->SSP_WordLength;
  SSPx->CR0 = tmpreg;

  /* SSPx CR1 Configuration */
  tmpreg = SSP_InitStruct->SSP_HardwareFlowControl + SSP_InitStruct->SSP_Mode;
  SSPx->CR1 = tmpreg;
}

/**
  * @brief  Fills each SSP_InitStruct member with its default value.
  * @param  SSP_InitStruct: pointer to a SSP_InitTypeDef structure
  *         that is to be initialized.
  * @retval None
  */
void SSP_StructInit(SSP_InitTypeDef* SSP_InitStruct)
{
  /* SSP_InitStruct members default value */
  /* Initialize the SPI_Mode member */
  SSP_InitStruct->SSP_Mode = SSP_ModeMaster;
  /* initialize the SSP_WordLength member */
  SSP_InitStruct->SSP_WordLength = SSP_WordLength8b;
  /* Initialize the SSP_SPO member */
  SSP_InitStruct->SSP_SPO = SSP_SPO_Low;
  /* Initialize the SSP_SPH member */
  SSP_InitStruct->SSP_SPH = SSP_SPH_1Edge;
  /* Initialize the SSP_FRF member */
  SSP_InitStruct->SSP_FRF = SSP_FRF_SPI_Motorola;
  /* Initialize the SSP max speed */
  SSP_InitStruct->SSP_CPSDVSR = 2;
  SSP_InitStruct->SSP_SCR = 0;

}

/**
  * @brief  Enables or disables the specified SSP peripheral.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  NewState: new state of the SSPx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SSP_Cmd(MDR_SSP_TypeDef* SSPx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SSP by setting the SSE bit in the CR1 register */
    SSPx->CR1 |= CR1_EN_Set;
  }
  else
  {
    /* Disable the selected SSP by clearing the SSE bit in the CR1 register */
    SSPx->CR1 &= CR1_EN_Reset;
  }
}

/**
  * @brief  Enables or disables the specified SSP interrupts.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  SSP_IT: specifies the SSP interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *           @arg SSP_IT_TX
  *           @arg SSP_IT_RX
  *           @arg SSP_IT_RT
  *           @arg SSP_IT_ROR
  * @param  NewState: new state of the specified SSPx interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SSP_ITConfig(MDR_SSP_TypeDef* SSPx, uint32_t SSP_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_SSP_CONFIG_IT(SSP_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    SSPx->IMSC |= SSP_IT;
  }
  else
  {
    SSPx->IMSC &= ~SSP_IT;
  }
}

/**
  * @brief  Checks whether the specified SSP interrupt has occurred or not.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  SSP_IT: specifies the SSP interrupt source to check.
  *         This parameter can be one of the following values:
  *           @arg SSP_IT_TX
  *           @arg SSP_IT_RX
  *           @arg SSP_IT_RT
  *           @arg SSP_IT_ROR
  * @retval The new state of SSP_IT (SET or RESET).
  */
ITStatus SSP_GetITStatus(MDR_SSP_TypeDef* SSPx, uint32_t SSP_IT)
{
  ITStatus bitstatus;

  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_SSP_CONFIG_IT(SSP_IT));

  if ((SSPx->RIS & SSP_IT) == SSP_IT)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  return (bitstatus);
}

/**
  * @brief  Checks whether the specified SSP interrupt (masked) has occurred or not.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  SSP_IT: specifies the SSP interrupt source to check.
  *         This parameter can be one of the following values:
  *           @arg SSP_IT_TX
  *           @arg SSP_IT_RX
  *           @arg SSP_IT_RT
  *           @arg SSP_IT_ROR
 * @retval The new state of SSP_IT (SET or RESET).
  */
ITStatus SSP_GetITStatusMasked(MDR_SSP_TypeDef* SSPx, uint32_t SSP_IT)
{
  ITStatus bitstatus;

  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_SSP_CONFIG_IT(SSP_IT));

  if (SSPx->MIS & SSP_IT)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  return (bitstatus);
}

/**
  * @brief  Clears the SSPx�s interrupt pending bits.
  * @param  SSPx: Select the SSP or the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  SSP_IT: specifies the interrupt pending bit to clear.
  *         This parameter can be one of the following values:
  *           @arg SSP_IT_RT
  *           @arg SSP_IT_ROR
  * @retval None
  */
void SSP_ClearITPendingBit(MDR_SSP_TypeDef* SSPx, uint32_t SSP_IT)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_SSP_RESET_IT(SSP_IT));

  SSPx->ICR |= SSP_IT;
}

/**
  * @brief  Enables or disables the SSP�s DMA interface.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  SSP_DMAReq: specifies the DMA request.
  *         This parameter can be any combination of the following values:
  *           @arg SSP_DMA_RXE: SSP DMA receive request
  *           @arg SSP_DMA_TXE: SSP DMA transmit request
  * @param  NewState: new state of the DMA Request sources.
  *         This parameter can be: ENABLE or DISABLE.
  * @note The DMA mode is not available for SSP5.
  * @retval None
  */
void SSP_DMACmd(MDR_SSP_TypeDef* SSPx, uint32_t SSP_DMAReq, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_SSP_DMAREQ(SSP_DMAReq));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the DMA transfer for selected requests in the SSP DMACR register */
    SSPx->DMACR |= SSP_DMAReq;
  }
  else
  {
    /* Disable the DMA transfer for selected requests in the SSP DMACR register */
    SSPx->DMACR &= (uint16_t)~SSP_DMAReq;
  }
}

/**
  * @brief  Transmits single data through the SSPx peripheral.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  Data: the data to transmit.
  * @retval None
  */
void SSP_SendData(MDR_SSP_TypeDef* SSPx, uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));

  /* Transmit Data */
  SSPx->DR = Data;

}

/**
  * @brief  Returns the most recent received data by the SSPx peripheral.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @retval The received data (7:0) and error flags (15:8).
  */
uint16_t SSP_ReceiveData(MDR_SSP_TypeDef* SSPx)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));

  /* Receive Data */
  return ((uint16_t)(SSPx->DR));
}

/**
  * @brief  Checks whether the specified SSP flag is set or not.
  * @param  SSPx: Select the SSP or the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2, SSP3, SSP4 or SSP5.
  * @param  SSP_FLAG: specifies the flag to check.
  *         This parameter can be one of the following values:
  *           @arg SSP_FLAG_BSY
  *           @arg SSP_FLAG_RFF
  *           @arg SSP_FLAG_RNE
  *           @arg SSP_FLAG_TNF
  * @retval The new state of SSP_FLAG (SET or RESET).
  */
FlagStatus SSP_GetFlagStatus(MDR_SSP_TypeDef* SSPx, uint32_t SSP_FLAG)
{
  FlagStatus bitstatus;

  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_SSP_FLAG(SSP_FLAG));

  if (SSPx->SR & SSP_FLAG)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  return (bitstatus);
}

/**
  * @brief  Initializes the SSPx peripheral Clock according to the
  *         specified parameters.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  SSP_BRG: specifies the HCLK division factor.
  *         This parameter can be one of the following values:
  *           @arg SSP_HCLKdiv1
  *           @arg SSP_HCLKdiv2
  *           @arg SSP_HCLKdiv4
  *           @arg SSP_HCLKdiv8
  *           @arg SSP_HCLKdiv16
  *           @arg SSP_HCLKdiv32
  *           @arg SSP_HCLKdiv64
  *           @arg SSP_HCLKdiv128
  * @retval None
  */
void SSP_BRGInit ( MDR_SSP_TypeDef* SSPx, uint32_t SSP_BRG ) {
	uint32_t tmpreg;

	/* Check the parameters */
	assert_param(IS_SSP_ALL_PERIPH(SSPx));
	assert_param(IS_SSP_CLOCK_BRG(SSP_BRG));

#ifdef USE_MDR1986VE3 /* For Cortex M1 */
	if ( (SSPx != MDR_SSP1) && (SSPx != MDR_SSP2) && (SSPx != MDR_SSP3)) {
		tmpreg = MDR_RST_CLK->UART_SSP_CLOCK;
	}
	else
#endif	// #ifdef USE_MDR1986VE3 /* For Cortex M1 */
#if defined (USE_MDR1901VC1T)
	if(SSPx == MDR_SSP4)
		tmpreg = MDR_RST_CLK->SPP2_CLOCK;
	else
#endif
		tmpreg = MDR_RST_CLK->SSP_CLOCK;



	if (SSPx == MDR_SSP1) {
		tmpreg |= RST_CLK_SSP_CLOCK_SSP1_CLK_EN;
		tmpreg &= ~RST_CLK_SSP_CLOCK_SSP1_BRG_Msk;
		tmpreg |= SSP_BRG;
	}
	else{
		if (SSPx == MDR_SSP2) {
			tmpreg |= RST_CLK_SSP_CLOCK_SSP2_CLK_EN;
			tmpreg &= ~RST_CLK_SSP_CLOCK_SSP2_BRG_Msk;
			tmpreg |= (SSP_BRG << 8);
		}
#if defined  (USE_MDR1986VE3) || defined (USE_MDR1901VC1T)
		else{
			if(SSPx == MDR_SSP3) {
				tmpreg |= RST_CLK_SSP_CLOCK_SSP3_CLK_EN;
				tmpreg &= ~RST_CLK_SSP_CLOCK_SSP3_BRG_Msk;
				tmpreg |= (SSP_BRG << RST_CLK_SSP_CLOCK_SSP3_BRG_Pos);
			}

			else{
				if(SSPx == MDR_SSP4) {
					tmpreg |= SSP4_CLK_EN;
					tmpreg &= ~SSP4_BRG_Mask;
					tmpreg |= (SSP_BRG << SSP4_BRG_Pos);
				}
			}
		}
#endif // #ifdef USE_MDR1986VE3 /* For Cortex M1 */
	}
#ifdef USE_MDR1986VE3 /* For Cortex M1 */
	if( (SSPx != MDR_SSP1) && (SSPx != MDR_SSP2) && (SSPx != MDR_SSP3) ){
		MDR_RST_CLK->UART_SSP_CLOCK = tmpreg;
	}
	else
#endif // #ifdef USE_MDR1986VE3 /* For Cortex M1 */
#if defined (USE_MDR1901VC1T)
	if(SSPx == MDR_SSP4)
		MDR_RST_CLK->SPP2_CLOCK = tmpreg;
#endif
		MDR_RST_CLK->SSP_CLOCK = tmpreg;

}


/** @} */ /* End of group SSP_Private_Functions */

/** @} */ /* End of group SSP */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Driver */

/******************* (C) COPYRIGHT 2011 Phyton *********************************
*
* END OF FILE MDR32F9Qx_ssp.c */

