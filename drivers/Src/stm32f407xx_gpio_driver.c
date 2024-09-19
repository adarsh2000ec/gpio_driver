/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Sep 17, 2024
 *      Author: adarsh
 */


#include <stm32f407xx_gpio_driver.h>

void GPIO_ClockControl(GPIO_RegDef_t *pGPIOx , uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pGPIOx == GPIOA)
        {
            GPIOA_PCLK_EN();
        }
        else if(pGPIOx == GPIOB)
        {
            GPIOB_PCLK_EN();
        }
        else if(pGPIOx == GPIOC)
        {
            GPIOC_PCLK_EN();
        }
        else if(pGPIOx == GPIOD)
        {
            GPIOD_PCLK_EN();
        }
        else if(pGPIOx == GPIOE)
        {
            GPIOE_PCLK_EN();
        }
        else if(pGPIOx == GPIOF)
        {
            GPIOF_PCLK_EN();
        }
        else if(pGPIOx == GPIOG)
        {
            GPIOG_PCLK_EN();
        }
        else if(pGPIOx == GPIOH)
        {
            GPIOH_PCLK_EN();
        }
        else if(pGPIOx == GPIOI)
        {
            GPIOI_PCLK_EN();
        }
    }

    else
    {
        if(pGPIOx == GPIOA)
        {
            GPIOA_PCLK_DI();
        }
        else if(pGPIOx == GPIOB)
        {
            GPIOB_PCLK_DI();
        }
        else if(pGPIOx == GPIOC)
        {
            GPIOC_PCLK_DI();
        }
        else if(pGPIOx == GPIOD)
        {
            GPIOD_PCLK_DI();
        }
        else if(pGPIOx == GPIOE)
        {
            GPIOE_PCLK_DI();
        }
        else if(pGPIOx == GPIOF)
        {
            GPIOF_PCLK_DI();
        }
        else if(pGPIOx == GPIOG)
        {
            GPIOG_PCLK_DI();
        }
        else if(pGPIOx == GPIOH)
        {
            GPIOH_PCLK_DI();
        }
        else if(pGPIOx == GPIOI)
        {
            GPIOI_PCLK_DI();
        }
    }
}


void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint32_t temp=0;
    // configure the mode of gpio pin
    if((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode) <= GPIO_MODE_ANALOG )
    {
        temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode)<<(2*(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER &= ~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER |= temp;
        
    }
    else
    {
        //implementation pending (interrupt mode)
        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
            EXTI->FTSR |= (1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
            EXTI->RTSR &= ~(1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        }
        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
            EXTI->RTSR |= (1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
            EXTI->FTSR &= ~(1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        }
        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
            EXTI->RTSR |= (1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
            EXTI->FTSR |= ~(1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        }
        //2. configure the GPIO port selection in SYSCFG_EXTICR
        uint8_t temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)/4;
        uint8_t temp2 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)%4;
        uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
        SYSCFG_PCLK_EN();
        SYSCFG->EXTICR[temp1]=(portcode<<(temp2*4)); //ONLY SINGLE PORT  DOUBT

        // 3. enable the interrupt

        EXTI->IMR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);


    }
    temp=0;

    //configure the speed of pin
    temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR |= temp;

    temp=0;

    //configure the pull up and down settings
    temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR |= temp;

    temp = 0;

    //configure the op type
    temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->OTYPER &= ~(0x1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OTYPER |= temp;

    temp=0;

    // configure alternate functionality

    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        //0-7 altLow
        //8-15 althigh
        uint8_t temp1,temp2;
        temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber  /   8;  // gets the lower or higher byte
        temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber  %   8;  // goes to the position of a bit
        temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode<<4*(temp2);
        pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xf<<4*(temp2));
        pGPIOHandle->pGPIOx->AFR[temp1] |= temp;
       
        
    }
    temp = 0;
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx , uint8_t pin)
{
    uint8_t read = 0;
    read = (uint8_t)(pGPIOx->IDR>>pin) &(0x00000001) ;                              // 0010000
    return read;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    uint16_t value;
    value = (uint16_t)pGPIOx->IDR;
    return value;
}

void WriteToOutputPin(GPIO_RegDef_t *pGPIOx ,uint8_t pin , uint8_t value)
{
    if(value == GPIO_PIN_SET)
    {
        pGPIOx->ODR |= (1<<pin);
    }
    else
    {
        pGPIOx->ODR &= ~(1<<pin);
    }
    
}

void WriteToOutputport(GPIO_RegDef_t *pGPIOx ,uint16_t value)
{
    pGPIOx->ODR = value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    pGPIOx->ODR ^= (1<<PinNumber);
}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
        if(pGPIOx == GPIOA)
        {
            GPIOA_REG_RESET();
        }
        else if(pGPIOx == GPIOB)
        {
            GPIOB_REG_RESET();
        }
        else if(pGPIOx == GPIOC)
        {
            GPIOC_REG_RESET();
        }
        else if(pGPIOx == GPIOD)
        {
            GPIOD_REG_RESET();
        }
        else if(pGPIOx == GPIOE)
        {
            GPIOE_REG_RESET();
        }
        else if(pGPIOx == GPIOF)
        {
            GPIOF_REG_RESET();
        }
        else if(pGPIOx == GPIOG)
        {
            GPIOG_REG_RESET();
        }
        else if(pGPIOx == GPIOH)
        {
            GPIOH_REG_RESET();
        }
        else if(pGPIOx == GPIOI)
        {
            GPIOI_REG_RESET();
        }
}

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER2 |= ( 1 << (IRQNumber % 64) );
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER2 |= ( 1 << (IRQNumber % 64) );
		}
	}
}
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    uint8_t temp1 = IRQNumber / 4;
    uint8_t temp2 = IRQNumber % 4;
    uint8_t shift_amount = ( 8 * temp2) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;    //Last 4 bits unused 

	*(  NVIC_PR_BASE_ADDR + temp1 ) |=  ( IRQPriority << shift_amount );

}
void GPIO_IRQHandling(uint8_t PinNumber)
{
    //clear the exti pr register corresponding to the pin number
	if(EXTI->PR & ( 1 << PinNumber))
	{
		//clear
		EXTI->PR |= ( 1 << PinNumber);
	}
}