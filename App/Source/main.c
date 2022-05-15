#include "App_bsp.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

#if defined(SEMIHOSTING)
#include "stdio.h"
extern void initialise_monitor_handles(void);
#endif

static void vSetupHardware( void );
static void vTask_10ms( void *p );
static void vTask_50ms( void *p );
static void vTask_100ms( void *p );
static void vTask_500ms( void *p );

int main(void)
{

    static StaticTask_t Task10ms_Buffer; 
    static StaticTask_t Task50ms_Buffer; 
    static StaticTask_t Task100ms_Buffer;
    static StaticTask_t Task500ms_Buffer;

    /* Memory buffer for the task stacks */
    static StackType_t Task10ms_Stack[ configMINIMAL_STACK_SIZE ];
    static StackType_t Task50ms_Stack[ configMINIMAL_STACK_SIZE ];
    static StackType_t Task100ms_Stack[ configMINIMAL_STACK_SIZE ];
    static StackType_t Task500ms_Stack[ configMINIMAL_STACK_SIZE ];

    vSetupHardware();

    (void)xTaskCreateStatic( vTask_10ms, "10msTask", configMINIMAL_STACK_SIZE, NULL, 4u, Task10ms_Stack, &Task10ms_Buffer );
    (void)xTaskCreateStatic( vTask_50ms, "50msTask", configMINIMAL_STACK_SIZE, NULL, 3u, Task50ms_Stack, &Task50ms_Buffer );
    (void)xTaskCreateStatic( vTask_100ms, "100msTask", configMINIMAL_STACK_SIZE, NULL, 2u, Task100ms_Stack, &Task100ms_Buffer );
    (void)xTaskCreateStatic( vTask_500ms, "500msTask", configMINIMAL_STACK_SIZE, NULL, 1u, Task500ms_Stack, &Task500ms_Buffer );

    vTaskStartScheduler();

    return 0u;
}

/**
 * @brief   **provide the memory that is used by the Idle task**
 *
 * This function it only exist as a callback so the application can declare statically the IDLE TCB 
 * structure and the stack buffer the freertos will use for the IDLE task
 * 
 * @param  ppxIdleTaskTCBBuffer pointer to return the Tasck Control BLock for the IDLE task
 * @param  ppxIdleTaskStackBuffer pointer to return the buffer array will be used as stack
 * @param  pulIdleTaskStackSize pointer to return the stack size
 *
 * @retval  None
 */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE; /* cppcheck-suppress misra-c2012-10.6*/
}

static void vSetupHardware( void )
{
    /* initialize semihosting */
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
    /*Application initializations*/
    // Os_SetupHardware();
}

/**
 * @brief   **10 millisecond freertos task**
 *
 * This freertos function will call the application 10ms function periodically  
 * User shall never place any code in this function
 * 
 * @param  p pointer pass parameters, Not in use for this application
 *
 * @retval  None
 */
static void vTask_10ms( void *p )
{
    TickType_t Seed = xTaskGetTickCount();
    (void)p;

    for(;;)
    {
        /*Run tyhe following function periodically at a rate of 10ms*/
 
        /*Block this task for exactly 10ms*/
        vTaskDelayUntil( &Seed, 10u );
    }
}


/**
 * @brief   **50 millisecond freertos task**
 *
 * This freertos function will call the application 50ms function periodically  
 * User shall never place any code in this function
 * 
 * @param  p pointer pass parameters, Not in use for this application
 *
 * @retval  None
 */
static void vTask_50ms( void *p )
{
    TickType_t Seed = xTaskGetTickCount();
    (void)p;

    for(;;)
    {
        /*Run tyhe following function periodically at a rate of 50ms*/

        /*Block this task for exactly 50ms*/
        vTaskDelayUntil( &Seed, 50u );
    }
}


/**
 * @brief   **100 millisecond freertos task**
 *
 * This freertos function will call the application 100ms function periodically  
 * User shall never place any code in this function
 * 
 * @param  p pointer pass parameters, Not in use for this application
 *
 * @retval  None
 */
static void vTask_100ms( void *p )
{
    TickType_t Seed = xTaskGetTickCount();
    (void)p;

    for(;;)
    {
        /*Run tyhe following function periodically at a rate of 100ms*/
        /*Block this task for exactly 100ms*/
        vTaskDelayUntil( &Seed, 100u );
    }
}


/**
 * @brief   **500 millisecond freertos task**
 *
 * This freertos function will call the application 500ms function periodically  
 * User shall never place any code in this function
 * 
 * @param  p pointer pass parameters, Not in use for this application
 *
 * @retval  None
 */
static void vTask_500ms( void *p )
{
    TickType_t Seed = xTaskGetTickCount();
    (void)p;

    for(;;)
    {
        /*Run tyhe following function periodically at a rate of 500ms*/
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);

        /*Block this task for exactly 500ms*/
        vTaskDelayUntil( &Seed, 500u );
    }
}

/**
 * @brief   **Idle task**
 *
 * The idle task runs at the very lowest priority, so such an idle hook function will only get 
 * executed when there are no tasks of higher priority that are able to run
 * 
 * @retval  None
 */
void vApplicationIdleHook(void) /* cppcheck-suppress misra-c2012-8.4 */
{
    // Os_IdleTask();
}


/**
 * @brief   **Idle Hook**
 *
 * The tick interrupt can optionally call an application defined hook (or callback) function - 
 * the tick hook. The tick hook provides a convenient place to implement timer functionality
 * 
 * @retval  None
 */
void vApplicationTickHook(void)
{
    // Os_TickHook();
}


/**
 * @brief   **Print the file and line where the function was called**
 *
 * Use the function with to point it out where a potential error happend, the output will only be
 * visible trough the debug server (OpenOCD) so it is mandatory to run the code inside a debug
 * session. It is recomended to call this fucntion trhough the macro assert_macro
 *
 * @param file string with the name of the file where the function was called. macro __FILE__ shall
 * be used
 * @param line the line number of the file where the function was called, macro __LINE__ shall be
 * used
 *
 * @retval none
 */
void assert_failed( const char *file, int line )
{
    // (void)printf( "Suspected error on file: %s, line %d\r\n", file, line );
    while( 1 )
    {
    }
}

/**
 * @brief   **Hard Fault interrupt vector**
 *
 * Any time an ilegal operation in the microcontroller is perform the program execution will jump
 * inside this ISR, This function will only invoke the macro assert_macro to print out the file and
 * line where the error was caused
 */
void HardFault_Handler( void ) /* cppcheck-suppress misra-c2012-8.4 */
{
    // assert_macro( 0u );
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
}