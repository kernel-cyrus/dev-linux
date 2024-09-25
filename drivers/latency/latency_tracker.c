#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/tracepoint.h>
#include <trace/events/sched.h> 

static void tracker_sched_wakeup_handler(void *ignore, struct task_struct *p)
{
	p->ts_wakeup = ktime_get_ns();
}

static void tracker_sched_switch_handler(void *ignore, bool preempt,
		   struct task_struct *prev, struct task_struct *next,
		   unsigned int prev_state)
{
	unsigned long long delay;
	if (!next->ts_wakeup)
		return;
	delay = ktime_get_ns() - next->ts_wakeup;
	if (delay > 100000)
		trace_printk("Task %s (pid=%d) is scheduled. delay: %lld\n", next->comm, next->pid, delay);
	next->ts_wakeup = 0;
}

static int __init latency_tracker_init(void)
{
	int ret;

	printk(KERN_INFO "latency-tracker init.\n");

	ret = register_trace_sched_wakeup(tracker_sched_wakeup_handler, NULL);
	if (ret) {
		printk(KERN_INFO "sched_wakeup handler init failed.\n");
	}

	ret = register_trace_sched_wakeup_new(tracker_sched_wakeup_handler, NULL);
	if (ret) {
		printk(KERN_INFO "sched_wakeup_new handler init failed.\n");
	}

	ret = register_trace_sched_switch(tracker_sched_switch_handler, NULL);
	if (ret) {
		printk(KERN_INFO "sched_switch handler init failed.\n");
	}

	return 0;
}

static void __exit latency_tracker_exit(void) {
	printk(KERN_INFO "latency-tracker exit.\n");
	unregister_trace_sched_wakeup(tracker_sched_wakeup_handler, NULL);
	unregister_trace_sched_wakeup_new(tracker_sched_wakeup_handler, NULL);
	unregister_trace_sched_switch(tracker_sched_switch_handler, NULL);
}

module_init(latency_tracker_init);
module_exit(latency_tracker_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cyrus Huang");
MODULE_DESCRIPTION("Light-weight system latency tracker.");
MODULE_VERSION("1.0");