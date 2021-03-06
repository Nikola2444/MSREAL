#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/io.h>
#include <linux/fs.h>		//required for fops
#include <linux/uaccess.h>	//required for 'cpoy_from_user' and 'copy_to_user'
#include <linux/signal.h>	//required for kernel-to-userspace signals

MODULE_LICENSE("GPL");

#define IRQ_NUM		61		// interrupt line
#define TIMER_BASE	0x42800000

#define TIMER_SIZE	0x0000FFFF
#define TCSR0		0x00000000
#define TLR0		0x00000004
#define TCR0		0x00000008
#define T0INT		0x00000100

#define TIMER_TCSR0	0x42800000	// address of timer 0's control and status register
#define TIMER_TLR0	0x42800004	// address of timer 0's load register
#define TIMER_TCR0	0x42800008	// address of timer 0's counter register
#define LOAD_VAL	0xE2329AFF	// address load value (500000000 cycles) ~5 sec
unsigned long *pTIMER_TCSR0;		// pointer to timer 0 control and status register
unsigned long *pTIMER_TLR0;		// pointer to timer 0 load register
unsigned long *pTIMER_TCR0;		// pointer to timer 0 counter register

#define DEVICE_NAME "syscall"		// device name
#define SYSCALL_MAJOR 22		// device major number
#define BUF_LEN 80			// max buffer length
#define SUCCESS 0			// success return value

//unsigned long *pSYSCALL_Virtual;	// base address
static int Device_Open = 0;		// device status

static char msg[BUF_LEN];		// the msg the device will give when asked
static char *msg_Ptr;
static int intcount;

// write routine (called when write() is used in user-space)
ssize_t syscall_write(struct file *flip, const char *buf, size_t length, loff_t *offset)
{
	printk("syscall_write.\n");				// debug: procedure call message
	if (copy_from_user(msg, buf, length) != 0)		// read buffer from user space
		return -EFAULT;					// return error if it failed
	//printk("Received: %s \n",msg);				// debug: what string is received

	if (strncmp(msg,"1",1) == 0)				// enable timer command
	{
		printk("Driver enables timer.\n");		// print timer status message
		iowrite32(LOAD_VAL,pTIMER_TLR0);		// prepare timer cycle 
		iowrite32(0x000000B0,pTIMER_TCSR0);		// load the load-register
		iowrite32(0x000000D0,pTIMER_TCSR0);		// Generate mode,upcounter, reload generate value
								// no load, enable IRQ, enable Timer
		return length;					// return 0 to application
	} 
	else if (strncmp(msg,"0",1) == 0)				// disable timer command
	{
		printk("Driver disables timer.\n");		// print timer status message
		iowrite32(0x00000050,pTIMER_TCSR0);		// Generate mode, upcounter, reload generate value
								// no load, enable IRQ, disable Timer
		return length;					// return 0 to application
	} 
	else 
	{
		printk("Driver received wrong value.\n");	// Print error message
		return -EFAULT;					// unknown value received
	}
}

// read routine (called when read() is used in user-space)
ssize_t syscall_read(struct file *flip, char *buf, size_t length, loff_t *offset)
{
	unsigned long timerval;
	// read timer value

	printk("syscall_read.\n");				// debug: procedure call message
	timerval = ioread32(pTIMER_TCR0);
	printk("Read() call value : %lu Cycles\n",timerval);	// display timer value
	
	if (copy_to_user(buf, &msg, length) != 0)		// send counter value
		return -EFAULT;
	else 
		return 0;
}

// open routine (called when a device opens /dev/syscall)
static int syscall_open(struct inode *inode, struct file *file)
{
	if (Device_Open)					// if the device is allready open,
		return -EBUSY;					// return with an error

	Device_Open++;						// 'open' device
	//sprintf(msg,"You tried to open the syscall module.\n");	// print open message
	msg_Ptr = msg;						
	try_module_get(THIS_MODULE);
	return 0;
}

// close routine (called whne a device closes /dev/syscall)
static int syscall_close(struct inode *inode, struct file *file)
{
	Device_Open--;						// 'close' device

	module_put(THIS_MODULE);
	return 0;
}

// device init and file operations
struct file_operations syscall_fops = {
	.read = syscall_read,		// read()
	.write = syscall_write,		// write()
	.open = syscall_open,		// open()
	.release = syscall_close,	// close()
};

// timer interrupt handler
static irqreturn_t irq_handler(int irq,void*dev_id)		
{      
	unsigned long temp;
	unsigned long timervalue;
      
	timervalue = ioread32(pTIMER_TCR0);			// Read Timer/Counter Register
	printk("Interrupt! Timer counter value : %lu Cycles\n",(timervalue-LOAD_VAL));	// Display timer value

	temp = ioread32(pTIMER_TCSR0);				// clear timer IRQ
	temp |= T0INT;
	iowrite32(temp, pTIMER_TCSR0);

	intcount++;
	if (intcount>=100)					// after 100 interrupts
	{
		printk("100 interrupts have been registered.\nDisabling timer");// print timer status message
		iowrite32(0x00000050,pTIMER_TCSR0);		//disable timer;
	}

	return IRQ_HANDLED;
}

// init module      
static int __init mod_init(void)  
{
	//unsigned long temp;
	printk(KERN_ERR "Init syscall module. \n");

	if (request_irq(IRQ_NUM,irq_handler, 0, DEVICE_NAME, NULL))  //request timer interrupt
	{
		printk(KERN_ERR "Not Registered IRQ. \n");
		return -EBUSY;
	}
	printk(KERN_ERR "Registered IRQ. \n");

	pTIMER_TCSR0 = ioremap_nocache(TIMER_TCSR0,0x4);	// map timer 0 control and status register
	pTIMER_TLR0 = ioremap_nocache(TIMER_TLR0,0x4);		// map timer 0 load register
	pTIMER_TCR0 = ioremap_nocache(TIMER_TCR0,0x4);		// map timer 0 count register
	//iowrite32(LOAD_VAL, pTIMER_TLR0);			// place load value in load register
	//temp = ioread32(pTIMER_TLR0);				// debug: read load value to check
	//printk("Load value: %lu.\n",temp);			// debug: print the read load value
	//iowrite32(0x000000B0, pTIMER_TCSR0);			// load TLR
	//iowrite32(0x000000D0, pTIMER_TCSR0);			// Generate mode,downcounter,reload generate
								// value,no load,enable IRQ,enable Timer
	intcount = 0;						// set interrupt count to 0, driver will unload on 100 interrupts
	// manual node creation
	if (register_chrdev(SYSCALL_MAJOR, DEVICE_NAME, &syscall_fops))
		printk("Error: cannot register to major device 22.\n");
	
	printk("Type: mknod /dev/%s c %d 0\n",DEVICE_NAME, SYSCALL_MAJOR);
	printk("And remove it after unloading the module.\n");

	return SUCCESS;
} 

// exit module
static void __exit mod_exit(void)  		
{
	iounmap(pTIMER_TCSR0);				// unregister timer hardware
	iounmap(pTIMER_TLR0);
	iounmap(pTIMER_TCR0);
	free_irq(IRQ_NUM, NULL);			// unregister timer interrupt
	unregister_chrdev(SYSCALL_MAJOR, "syscall");	// unregister device
	printk(KERN_ERR "Exit syscall Module. \n");	// print unload message
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_AUTHOR ("Tom");
MODULE_DESCRIPTION("Testdirver for the Xilinx AXI Timer IP core & system calls.");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("custom:syscall");
