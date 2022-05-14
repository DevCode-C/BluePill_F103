#include "App_bsp.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

#if defined(SEMIHOSTING)
#include "stdio.h"
extern void initialise_monitor_handles(void);
#endif

int main(void)
{
    #if defined(SEMIHOSTING)
    initialise_monitor_handles();
    (void)printf("\n");
    #endif

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef InitialStructure = {0};
    InitialStructure.Mode = GPIO_MODE_OUTPUT_PP;
    InitialStructure.Pin = GPIO_PIN_13;
    InitialStructure.Speed = GPIO_SPEED_HIGH;
    InitialStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC,&InitialStructure);

    uint32_t valor = 0;
    uint32_t counter = 0;


    HAL_Init();
    /* code */
    valor = HAL_GetTick();
    for (; ; )
    {
        
        if ((HAL_GetTick() - valor) >= 100)
        {
            #if defined(SEMIHOSTING)
            (void) printf("Hola mundo, \t counter = %ld\n",counter);
            #endif
            HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
            counter ++ ; 
            valor = HAL_GetTick();
        }
        
    }
    
    return 0;
}
