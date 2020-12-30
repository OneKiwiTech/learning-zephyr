/**
 *	@author		Phu Truong
 *	@email		phutruong@kienminh.net
 *  @skype		ngocphu811
 *	@website	http://kienminh.net
 */

#include "board.h"

struct device *init_led(void)
{
	struct device *led;
	int result;

	led = device_get_binding(LED_LABEL);
	if (led == NULL) {
		printk("Didn't find LED device %s\n", LED_LABEL);
		return NULL;
	}

	result = gpio_pin_configure(led, LED_PIN, GPIO_OUTPUT_ACTIVE | LED_FLAGS);
	if (result != 0) {
		printk("Error %d: failed to configure LED device %s pin %d\n",
		       result, LED_LABEL, LED_PIN);
		return NULL;
	}

	printk("Set up LED at %s pin %d\n", LED_LABEL, LED_PIN);

	return led;
}

struct device *init_button(void)
{
	struct device *button;
	int result;

	button = device_get_binding(BUTTON_LABEL);
	if (button == NULL) {
		printk("Error: didn't find %s device\n", BUTTON_LABEL);
		return;
	}

	result = gpio_pin_configure(button, BUTTON_PIN, BUTTON_FLAGS);
	if (result != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       result, BUTTON_LABEL, BUTTON_PIN);
		return;
	}

	result = gpio_pin_interrupt_configure(button,
					   BUTTON_PIN,
					   GPIO_INT_EDGE_TO_ACTIVE);
	if (result != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			result, BUTTON_LABEL, BUTTON_PIN);
		return;
	}
	return button;
}

/********************** (C) COPYRIGHT http://kienminh.net/ *****END OF FILE****/