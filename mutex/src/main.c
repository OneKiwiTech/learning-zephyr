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

#define SEM_INIT		(0U)
#define SEM_MAX			(5U)

void thread_increase(void *, void *, void *);
void thread_decrease(void *, void *, void *);

K_THREAD_STACK_DEFINE(increase_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(decrease_stack, STACK_SIZE);
static struct k_thread increase_data, decrease_data;

struct k_mutex mutex;
uint8_t count;
void main(void)
{

	count = 0;
	k_mutex_init(&mutex);

	k_thread_create(&increase_data, increase_stack,
			STACK_SIZE, thread_increase, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);

	k_thread_create(&decrease_data, decrease_stack,
			STACK_SIZE, thread_decrease, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
	
}

void thread_increase(void *p1, void *p2, void *p3)
{

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	printk("Start %s:\n", __func__);

	while (1) {
		if (k_mutex_lock(&mutex, K_NO_WAIT) == 0) {
			printk("%s: mutex successfully locked\n", __func__);
			count++;
			printk("%s: count %d\n", __func__, count);
			k_mutex_unlock(&mutex);
		} else {
			printk("%s: cannot lock mutex\n", __func__);
		}
		k_msleep(2*SLEEP_TIME);
	}

}

void thread_decrease(void *p1, void *p2, void *p3)
{

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	printk("Start %s:\n", __func__);

	while (1) {
		if (k_mutex_lock(&mutex, K_NO_WAIT) == 0) {
			printk("%s: mutex successfully locked\n", __func__);
			count--;
			printk("%s: count %d\n", __func__, count);
			k_mutex_unlock(&mutex);
		} else {
			printk("%s: cannot lock mutex\n", __func__);
		}
		k_msleep(SLEEP_TIME);
	}
}

/**
 * minicom -D /dev/ttyACM0
 * west build -b stm32l476g_disco mutex --pristine
 * west flash
 */

/********************** (C) COPYRIGHT http://kienminh.net/ *****END OF FILE****/