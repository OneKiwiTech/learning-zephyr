/**
 *	@author		Phu Truong
 *	@email		phutruong@kienminh.net
 *  @skype		ngocphu811
 *	@website	http://kienminh.net
 */

#include <zephyr.h>
#include <sys/printk.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME		1000
#define STACK_SIZE		1024
#define PRIORITY		5

void producer_thread(void *, void *, void *);
void consumer_thread(void *, void *, void *);

K_THREAD_STACK_DEFINE(producer_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(consumer_stack, STACK_SIZE);
static struct k_thread producer_data, consumer_data;

struct k_lifo my_lifo;

struct lifo_data_t {
	void *LIFO_reserved; /* 1st word reserved for use by fifo */
	uint8_t id;
	uint8_t value;
};

void main(void)
{
	k_lifo_init(&my_lifo);
	k_thread_create(&producer_data, producer_stack,
			STACK_SIZE, producer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);

	k_thread_create(&consumer_data, consumer_stack,
			STACK_SIZE, consumer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
	
}

void producer_thread(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	uint8_t id = 0;
	uint8_t value = 0;
	struct lifo_data_t tx_data = { .id = id, .value = value };

	printk("Start %s:\n", __func__);

	while (1) {
		k_lifo_put(&my_lifo, &tx_data);
		printk("%s: id=%d, value=%d\n", __func__, id, value);
		id++; value++;
		tx_data.id = id;
		tx_data.value = value;
		k_msleep(SLEEP_TIME);
	}
}

void consumer_thread(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	struct lifo_data_t *rx_data;

	printk("Start %s:\n", __func__);
	while (1)
	{
		rx_data = k_lifo_get(&my_lifo, K_NO_WAIT);
		printk("==================\n");
		printk("%s: id=%d, value=%d\n", __func__, rx_data->id, rx_data->value);
		//k_free(rx_data);
		k_msleep(4*SLEEP_TIME);
	}
}

/**
 * minicom -D /dev/ttyACM0
 * west build -b stm32l476g_disco lifo/lifo_thread2thread --pristine
 * west flash
 */

/********************** (C) COPYRIGHT http://kienminh.net/ *****END OF FILE****/