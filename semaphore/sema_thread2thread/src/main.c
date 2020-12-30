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

void thread_give_data(void *, void *, void *);
void thread_take_data(void *, void *, void *);

K_THREAD_STACK_DEFINE(give_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(take_stack, STACK_SIZE);
static struct k_thread give_data, take_data;

K_SEM_DEFINE(sema, SEM_INIT, SEM_MAX);
struct k_sem sem;

void main(void)
{
	k_sem_init(&sem, SEM_INIT, 1);
	k_thread_create(&give_data, give_stack,
			STACK_SIZE, thread_give_data, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);

	k_thread_create(&take_data, take_stack,
			STACK_SIZE, thread_take_data, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
	
}

void thread_give_data(void *p1, void *p2, void *p3)
{

	uint16_t count = 0;

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	printk("Start %s:\n", __func__);

	while (1) {
		k_sem_give(&sema);
		k_msleep(SLEEP_TIME);
		count = k_sem_count_get(&sema);

		printk("%s: count %d\n", __func__, count);

		if(count == SEM_MAX) {
			k_sem_reset(&sema);
			k_sem_give(&sem);
		}
		
	}

}

void thread_take_data(void *p1, void *p2, void *p3)
{

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	printk("Start %s:\n", __func__);
	while (1)
	{
		
		if (k_sem_take(&sema, K_NO_WAIT) != 0) {
        printk("%s: data not available!\n", __func__);
		} else {
			printk("%s: fetch available data!\n", __func__);
		}
		if (k_sem_take(&sem, K_NO_WAIT) != 0) {
			printk("%s: sema is countting!\n", __func__);
		} else {
			printk("%s: sema reset!\n", __func__);
		}
		k_msleep(2*SLEEP_TIME);
	}
	
	
}

/**
 * minicom -D /dev/ttyACM0
 * west build -b stm32l476g_disco semaphore/sema_thread2thread --pristine
 * west flash
 */

/********************** (C) COPYRIGHT http://kienminh.net/ *****END OF FILE****/