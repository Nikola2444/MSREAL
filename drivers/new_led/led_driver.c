#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>

#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Led driver");

#define DRIVER_NAME "led"

static dev_t first;
static struct class *cl;
static struct cdev c_dev;
static struct device *dev;


static int end_read = 0;

struct led_info {
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
};








