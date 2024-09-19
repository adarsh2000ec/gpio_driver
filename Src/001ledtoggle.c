#include "stm32f407xx.h"

void delay(void)
{
    for(uint32_t i=0; i<500000 ; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed;
    GpioLed.pGPIOx  = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;  //if PUPD , KEEP DEFAULT IF OD CHANGE IT TO PULL UP

    GPIO_ClockControl(GPIOD , ENABLE);
    GPIO_Init(&GpioLed);

    while(1)
    {
        GPIO_ToggleOutputPin(GPIOD , GPIO_PIN_NO_12);
        delay();
    }
    return 0;
}
