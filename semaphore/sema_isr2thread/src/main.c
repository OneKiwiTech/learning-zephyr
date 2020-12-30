/**
 *	@author		Phu Truong
 *	@email		phutruong@kienminh.net
 *  @skype		ngocphu811
 *	@website	http://kienminh.net
 */

#include "main.h"
#include "board.h"

#define STACK_SIZE 1024
#define PRIORITY 7

K_SEM_DEFINE(sema, 0, 1);

K_THREAD_STACK_DEFINE(led_stack, STACK_SIZE);
static struct k_thread led_data;

static struct gpio_callback button_cb_data;

struct device *led;
struct device *button;

void thread_led(void *p1, void *p2, void *p3);

static void button_callback(const struct device *dev,
				   struct gpio_callback *cb, uint32_t pins)
{
	bool value;

	ARG_UNUSED(pins);
	printk("button pressed!\n");
	if (k_sem_take(&sema, K_NO_WAIT) == 0) {
		value = gpio_pin_get(led, LED_PIN);
		gpio_pin_set(led, LED_PIN, !value);
	}
}

void main(void)
{
	led = init_led();
	if(led == NULL) return;

	button = init_button();
	if(button == NULL) return;

	gpio_init_callback(&button_cb_data, button_callback, BIT(BUTTON_PIN));
	gpio_add_callback(button, &button_cb_data);
	printk("Set up button at %s pin %d\n", BUTTON_LABEL, BUTTON_PIN);

	k_thread_create(&led_data, led_stack,
			STACK_SIZE, thread_led, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);

}


void thread_led(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	while (1)
	{
		k_sem_give(&sema);
		k_msleep(SLEEP_TIME/2);
	}
}

/**
 * minicom -D /dev/ttyACM0
 * west build -b stm32l476g_disco semaphore/sema_isr2thread --pristine
 * west flash
 */

/********************** (C) COPYRIGHT http://kienminh.net/ *****END OF FILE****/