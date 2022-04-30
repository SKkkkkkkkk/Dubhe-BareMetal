#ifndef QEMU
#include "dw_apb_uart.h"
#include <stdio.h>
#include "seehi_print.h"
#include "gic.h"
#include "irq_ctrl.h"
#include "dw_apb_timers.h"
#include "systimer.h"

void test_irq_handler(void)
{
	static int i = 0;
	seehi_printf("%d in irq\n",i++);
	(void)TIMERX2->Timer2EOI;
}
int main()
{
	GIC_Enable();
	systimer_init();
	seehi_uart_config_baudrate(SEEHI_UART_BAUDRATE_115200, 20000000, SEEHI_UART1);
	seehi_printf("hello world\n");
	timer_init_config_t timer_init_config = {
		.int_mask = false, .loadcount = 20000000, .timer_id = Timerx2_T2, .timer_mode = Mode_User_Defined
	};
	timer_init(&timer_init_config);
	IRQ_SetHandler(Timer1_2_IRQn, test_irq_handler); //设置中断处理函数
	IRQ_SetPriority(Timer1_2_IRQn, 0 << 3); //设置优先级
	IRQ_Enable(Timer1_2_IRQn); //使能该中断
	timer_enable(timer_init_config.timer_id);

	while(1)
	{
		static int i = 0;
		seehi_printf("%ds\n",i++);
		systimer_delay(1, IN_S);
	}
	return 0;
}


#else
#include "main_qemu.c"
#endif