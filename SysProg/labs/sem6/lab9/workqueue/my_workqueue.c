#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zhabin Dmitriy");
MODULE_DESCRIPTION("workqueue");

#define SCANCODE_MASK 0x7f
#define STATUS_MASK 0x80
#define IRQ 1
static int dev_id;
static int scancode, status;

static char *ascii_map[] = {
    "[ESC]", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "bs", "[Tab]", "Q",
    "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "[Enter]", "[CTRL]", "A", "S", "D",
    "F", "G", "H", "J", "K", "L", ";", "\'", "`", "[LShift]", "\\", "Z", "X", "C", "V", "B", "N", "M",
    ",", ".", "/", "[RShift]", "[PrtSc]", "[Alt]", " ", "[Caps]", "F1", "F2", "F3", "F4", "F5",
    "F6", "F7", "F8", "F9", "F10", "[Num]", "[Scroll]", "[Home(7)]", "[Up(8)]", "[PgUp(9)]", "-",
    "[Left(4)]", "[Center(5)]", "[Right(6)]", "+", "[End(1)]", "[Down(2)]", "[PgDn(3)]", "[Ins]", "[Del]"};

static struct workqueue_struct *my_wq;

void my_workqueue_function(struct work_struct *work)
{    
    status = scancode & STATUS_MASK;
    if (!status)
    {
        char *key = ascii_map[(scancode & SCANCODE_MASK) - 1];
        printk(KERN_INFO "my_workqueue: KEYBOARD INTERRUPT: %s\n", key);
    }
}

DECLARE_WORK(workname, my_workqueue_function);

irqreturn_t my_irq_handler(int irq, void *dev)
{
	if (irq == IRQ)
    {
        scancode = inb(0x60);
		queue_work(my_wq, &workname);
		return IRQ_HANDLED;
	}
	return IRQ_NONE;
}

static int __init my_workqueue_init(void)
{
	int ret = request_irq(IRQ, my_irq_handler, IRQF_SHARED, "my_irq_handler", &dev_id);
	if (ret)
    {
		printk(KERN_ERR "my_tasklet: my_irq_handler wasn't registered\n");
		return ret;
	}

	if (!(my_wq = create_workqueue("my_queue")))
    {
		free_irq(IRQ, &dev_id);
		printk(KERN_INFO "my_workqueue: workqueue wasn't created");
		return -ENOMEM;
	}

	printk(KERN_INFO "my_workqueue: module loaded\n");
	return 0;
}

static void __exit my_workqueue_exit(void)
{
	flush_workqueue(my_wq);
	destroy_workqueue(my_wq);
	free_irq(IRQ, &dev_id);
	printk(KERN_INFO "my_workqueue: module unloaded\n");
}

module_init(my_workqueue_init)
module_exit(my_workqueue_exit)
