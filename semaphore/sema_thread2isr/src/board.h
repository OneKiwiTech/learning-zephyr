/**
 *	@author		Phu Truong
 *	@email		phutruong@kienminh.net
 *  @skype		ngocphu811
 *	@website	http://kienminh.net
 */

#ifndef __BOARD_H
#define __BOARD_H

#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#define FLAGS_OR_ZERO(node)						\
	COND_CODE_1(DT_PHA_HAS_CELL(node, gpios, flags),		\
		    (DT_GPIO_FLAGS(node, gpios)),			\
		    (0))

#define LED_NODE		DT_ALIAS(led0)
#define BUTTON_NODE		DT_ALIAS(sw0)

#if DT_NODE_HAS_STATUS(LED_NODE, okay)
	#define LED_LABEL		DT_GPIO_LABEL(LED_NODE, gpios)
	#define LED_PIN			DT_GPIO_PIN(LED_NODE, gpios)
	#define LED_FLAGS		DT_GPIO_FLAGS(LED_NODE, gpios)
#else
	/* A build error here means your board isn't set up to blink an LED. */
	#error "Unsupported board: LED_RED devicetree alias is not defined"
	#define LED_LABEL		""
	#define LED_PIN			0
	#define LED_FLAGS		0
#endif

#if DT_NODE_HAS_STATUS(BUTTON_NODE, okay)
	#define BUTTON_LABEL	DT_GPIO_LABEL(BUTTON_NODE, gpios)
	#define BUTTON_PIN		DT_GPIO_PIN(BUTTON_NODE, gpios)
	#define BUTTON_FLAGS	(GPIO_INPUT | FLAGS_OR_ZERO(BUTTON_NODE))
#else
	#error "Unsupported board: sw0 devicetree alias is not defined"
	#define BUTTON_LABEL	""
	#define BUTTON_PIN		0
	#define BUTTON_FLAGS	0
#endif

struct device *init_led(void);
struct device *init_button(void);

#endif /* __BOARD_H */

/********************** (C) COPYRIGHT http://kienminh.net/ *****END OF FILE****/