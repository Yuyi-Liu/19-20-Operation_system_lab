#include "lock.h"
#include "time.h"
#include "stdio.h"
#include "sched.h"
#include "queue.h"
#include "screen.h"

pcb_t pcb[NUM_MAX_TASK];

/* current running task PCB */
pcb_t *current_running;

/* global process id */
pid_t process_id = 1;

static void check_sleeping()
{/*
	pcb_t *pcb_sleep_head = sleep_quene.head; //取出睡眠队列的头
	if(pcb_sleep_head != NULL)   //非空
	{
		//如果休眠时间已到
		if(get_timer() - pcb_sleep_head->begin_time >= pcb_sleep_head->sleep_time)
		{
			pcb_sleep_head = (pcb_t *)queue_dequeue(&sleep_queue);   //注意,是&sleep_queue不是sleep_queue！
			queue_push(&read_queue, pcb_sleep_head);
		}
	}
	//调用一次仅会检查一次*/
}

void scheduler(void)
{
    // TODO schedule
    // Modify the current_running pointer.
	pcb_t *next_pcb;
	next_pcb = (pcb_t *)queue_dequeue(&ready_queue);  //出栈
	
	if(current_running->status != TASK_BLOCKED)
	{
		current_running->status = TASK_READY;//如果不是被阻塞，就回到队列中
		if(current_running->pid != 1)      //不让启动pcb回到队列
			queue_push(&ready_queue,current_running);
	}
	current_running = next_pcb;
	current_running->status = TASK_RUNNING;
	
	return ;


}

void do_sleep(uint32_t sleep_time)
{
    // TODO sleep(seconds)
}

void do_block(queue_t *queue)
{
    // block the current_running task into the queue
	current_running->status = TASK_BLOCKED;
    queue_push(queue, (void *)current_running);
    do_scheduler();
	
}

void do_unblock_one(queue_t *queue)
{
    // unblock the head task from the queue
	 pcb_t *block_head = (pcb_t *)queue_dequeue(queue);
    block_head->status = TASK_READY;
    queue_push(&ready_queue, block_head);     //这里不需要do_schedule
	
}

void do_unblock_all(queue_t *queue)
{
    // unblock all task in the queue
	pcb_t *block_list;
    while(!queue_is_empty(queue))
    {
        block_list = (pcb_t *)queue_dequeue(queue);
        block_list->status = TASK_READY;
        queue_push(&ready_queue, block_list);
    }
	
}
