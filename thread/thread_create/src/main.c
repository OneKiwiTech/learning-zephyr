/**
 *	@author		Phu Truong
 *	@email		phutruong@kienminh.net
 *  @skype		ngocphu811
 *	@website	http://kienminh.net
 */

#include <zephyr.h>
#include <sys/printk.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME		500
#define STACK_SIZE		512
#define PRIORITY		5

void threadA(void *, void *, void *);
void threadB(void *, void *, void *);
void threadC(void *, void *, void *);

K_THREAD_STACK_DEFINE(threadB_stack, STACK_SIZE);
static struct k_thread threadB_data;

K_THREAD_STACK_DEFINE(threadC_stack, STACK_SIZE);
static struct k_thread threadC_data;

K_THREAD_DEFINE(thread_a, STACK_SIZE, threadA, NULL, NULL, NULL,
		PRIORITY, 0, 0);

k_tid_t threadA_tid, threadB_tid, threadC_tid;
void main(void)
{
	uint8_t index = 0;
	const char *thread_a, *thread_b, *thread_c;
	/* spawn threadB */
	threadB_tid = k_thread_create(&threadB_data, threadB_stack,
			STACK_SIZE, threadB, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);

	k_thread_name_set(threadB_tid, "thread_b");

	
	while (1)
	{
		switch (index)
		{
		case 0:
			printk("Main thread\n");
			break;
		case 1:
			thread_a = k_thread_name_get(threadA_tid);
			thread_b = k_thread_name_get(threadB_tid);
			thread_c = k_thread_name_get(threadC_tid);
			printk("%s: %s, %s, %s\n", __func__, thread_a, thread_b, thread_c);
			break;
		case 2:
			printk("threadA suspend\n");
			k_thread_suspend(threadA_tid);
			break;
		case 20:
			printk("threadA resume\n");
			k_thread_resume(threadA_tid);
			break;
		case 40:
			printk("threadA abort\n");
			k_thread_abort(threadA_tid);
			break;
		default:
			break;
		}
		index++;
		k_msleep(SLEEP_TIME/2);
		
	}
}

void threadA(void *p1, void *p2, void *p3)
{
	uint8_t index = 0;

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);
	
	threadA_tid = k_current_get();
	k_thread_name_set(threadA_tid, "thread_a");

	printk("Start %s:\n", __func__);

	/* spawn threadC */
	threadC_tid = k_thread_create(&threadC_data, threadC_stack,
			STACK_SIZE, threadC, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);

	k_thread_name_set(threadC_tid, "thread_c");

	while (1)
	{
		printk("%s: tick %03d !\n", __func__, index++);
		k_msleep(SLEEP_TIME);
	}
}

void threadB(void *p1, void *p2, void *p3)
{
	uint8_t index = 0;

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	printk("Start %s:\n", __func__);

	while (1)
	{
		printk("%s: tick %03d !\n", __func__, index++);
		k_msleep(2*SLEEP_TIME);
	}
}

void threadC(void *p1, void *p2, void *p3)
{
	uint8_t index = 0;

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	printk("Start %s:\n", __func__);

	while (1)
	{
		printk("%s: tick %03d !\n", __func__, index++);
		k_msleep(3*SLEEP_TIME);
	}
}


/**
 * minicom -D /dev/ttyACM0
 * west build -b stm32l476g_disco thread/thread_create --pristine
 * west flash
 */

/********************** (C) COPYRIGHT http://kienminh.net/ *****END OF FILE****/