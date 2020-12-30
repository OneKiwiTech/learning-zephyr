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
#define LIST_LEN		5

void producer_thread(void *, void *, void *);
void consumer_thread(void *, void *, void *);

K_THREAD_STACK_DEFINE(producer_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(consumer_stack, STACK_SIZE);
static struct k_thread producer_data, consumer_data;

struct k_fifo fifo;

struct fifo_data_t {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	uint8_t data;
};

struct fifo_data_t fdata[LIST_LEN];

void main(void)
{
	struct fifo_data_t tx_data;
	struct fifo_data_t *rx_data;
	k_fifo_init(&fifo);
	for (int i = 0; i < LIST_LEN; i++)
	{
		tx_data.data = i;
		k_fifo_put(&fifo, &tx_data);
	}
	struct fifo_data_t *head = &fdata[0];
	struct fifo_data_t *tail = &fdata[LIST_LEN - 1];
	//k_fifo_put_list(&fifo, (uint32_t *)head, (uint32_t *)tail);

	if(k_fifo_is_empty(&fifo) != 0){
		printk("FIFO queue is empty\n");
	} else {
		printk("FIFO data is available\n");
	}
	struct fifo_data_t *peek_head = k_fifo_peek_head(&fifo);
	struct fifo_data_t *peek_tail = k_fifo_peek_tail(&fifo);
	printk("peek_tail data=%d\n", peek_tail->data);
	printk("peek_head data=%d\n", peek_head->data);
	/*
	k_thread_create(&producer_data, producer_stack,
			STACK_SIZE, producer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);

	k_thread_create(&consumer_data, consumer_stack,
			STACK_SIZE, consumer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
	*/
	while (1)
	{
		rx_data = k_fifo_get(&fifo, K_NO_WAIT);
		printk("%s: data=%d\n", __func__, rx_data->data);
		k_msleep(SLEEP_TIME);
	}
	
	
}

void producer_thread(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	uint8_t id = 0;
	struct fifo_data_t tx_data = { .data = id };

	printk("Start %s:\n", __func__);

	if(k_fifo_is_empty(&fifo) != 0){
		printk("FIFO queue is empty\n");
	} else {
		printk("FIFO data is available\n");
	}

	while (1) {
		k_fifo_put(&fifo, &tx_data);
		printk("%s: id=%d, \n", __func__, id);
		id++;
		tx_data.data = id;
		k_msleep(SLEEP_TIME);
	}
}

void consumer_thread(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	struct fifo_data_t *rx_data;

	printk("Start %s:\n", __func__);
	while (1)
	{
		rx_data = k_fifo_get(&fifo, K_NO_WAIT);
		printk("==================\n");
		printk("%s: id=%d\n", __func__, rx_data->data);
		//k_free(rx_data);
		k_msleep(4*SLEEP_TIME);
	}
}

/**
 * minicom -D /dev/ttyACM0
 * west build -b stm32l476g_disco fifo/fifo_api --pristine
 * west flash
 */

/********************** (C) COPYRIGHT http://kienminh.net/ *****END OF FILE****/