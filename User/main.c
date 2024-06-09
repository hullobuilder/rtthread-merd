#include "board.h"
#include "rtthread.h"

static rt_thread_t led_thread = RT_NULL;

static void led_thread_entry(void* parameter)
{
    while(1){
        LOCAL_LED_ON;
        rt_thread_delay(500);
        rt_kprintf("led_thread running: led_on\r\n");
        
        LOCAL_LED_OFF;
        rt_thread_delay(500);
        rt_kprintf("led_thread running: led_off\r\n");
    }
}


int main(void)
{
    led_thread = rt_thread_create("led_show", led_thread_entry, RT_NULL, 512, 3, 20);
    if(led_thread != RT_NULL)
        rt_thread_startup(led_thread);
    else
        return -1;
}
