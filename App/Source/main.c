#include "App_bsp.h"

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
    uint32_t valor = 0;
    uint32_t counter = 0;

    HAL_Init();
    /* code */
    valor = HAL_GetTick();
    for (; ; )
    {
        
        if ((HAL_GetTick() - valor) >= 10)
        {
            (void) printf("Hola mundo, \t counter = %ld\n",counter);
            counter ++ ; 
            valor = HAL_GetTick();
        }
        
    }
    
    return 0;
}
