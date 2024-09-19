#include "stm32f407xx.h"
#include <string.h>
void delay(void)
{
    for(uint32_t i=0; i<500000/2 ; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed ,GPIOBtn;
    memset(&GpioLed,0,sizeof(GpioLed));
    	memset(&GPIOBtn,0,sizeof(GPIOBtn));
    GpioLed.pGPIOx  = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;  //if PUPD , KEEP DEFAULT IF OD CHANGE IT TO PULL UP

    GPIO_ClockControl(GPIOD , ENABLE);
    GPIO_Init(&GpioLed);

    /* For button */
    GPIOBtn.pGPIOx  = GPIOB  ;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;  //if PUPD , KEEP DEFAULT IF OD CHANGE IT TO PULL UP

    GPIO_ClockControl(GPIOB , ENABLE);
    GPIO_Init(&GPIOBtn);
    GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10,15);
    GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10,ENABLE);
     while(1);
    return 0;
}
void EXTI15_10_IRQHandler(void)
{
    delay();
    GPIO_IRQHandling(12);
    GPIO_ToggleOutputPin(GPIOD , GPIO_PIN_NO_12);
}
