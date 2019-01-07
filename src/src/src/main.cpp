#include "MDR32Fx.h" 
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h" 

#include "FreeRTOS.h"
#include "task.h"

void initHSE(void);

void ledInit(void);
void ledWrite(bool onOff);

static void vBlinkTask( void *pvParameters );

int main() {
	SystemInit();
	initHSE();
	SystemCoreClockUpdate();

	ledInit();
	
	/* Create the tasks defined within this file. */
    xTaskCreate( vBlinkTask, "vBlinkTask", 50, nullptr, 
            tskIDLE_PRIORITY+1, nullptr );

    vTaskStartScheduler();

	while(1) {
	}
	
	return 0;
}

static void vBlinkTask( void * )
{
	bool on_off = true;
    for( ;; )
    {
		on_off = !on_off;

        PORT_WriteBit(MDR_PORTB, PORT_Pin_0, on_off ? Bit_SET : Bit_RESET);
		
        vTaskDelay( 500 );
    }
}

extern "C" {
	void vApplicationStackOverflowHook() {
		while(true) {

		}
	}

	void vApplicationIdleHook (void) {

	}
}

void ledInit(void) {
	PORT_InitTypeDef PORT_InitStructure;

	PORT_InitStructure.PORT_OE = PORT_OE_OUT;
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_InitStructure.PORT_Pin = (PORT_Pin_0);

	PORT_Init(MDR_PORTB, &PORT_InitStructure);
}

void initHSE(void) {

	RST_CLK_DeInit();

	RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	
	if (RST_CLK_HSEstatus() == ERROR) while (1);

	RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);

	RST_CLK_CPU_PLLcmd(ENABLE);

	if (RST_CLK_CPU_PLLstatus() == ERROR) while (1);

	RST_CLK_CPU_PLLuse(ENABLE);

	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
} 