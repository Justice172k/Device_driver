#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include<linux/slab.h>   

#define mem_size        1024 

dev_t dev = 0;
static struct class *dev_class;
static struct cdev device_cdev;
uint8_t *kernel_buffer;

static int open_device(struct inode *inode, struct file *file)
{
	pr_info("function open_device called\n");
	return 0;
}

static int release_device(struct inode *inode, struct file *file)
{
	pr_info("function release_device called\n");
	return 0;
}

static ssize_t read_device(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        if( copy_to_user(buf, kernel_buffer, mem_size) )
        {
                pr_err("Data Read : Err!\n");
        }
        pr_info("Data Read : Done!\n");
	pr_info("function read_device called\n");
	return mem_size;
}


static ssize_t write_device(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        if( copy_from_user(kernel_buffer, buf, len) )
        {
                pr_err("Data Write : Err!\n");
        }
	pr_info("function write_device called\n");
	return len;
}

static struct file_operations fops = 
{
	.owner	= THIS_MODULE,
	.read	= read_device,
	.write	= write_device,
	.open	= open_device,
	.release= release_device,
};

static int __init real_driver_init(void)
{
	if((alloc_chrdev_region(&dev, 2, 1, "real_device"))<0)
	{
		pr_err("error allocate major number\n");
		return -1;
	}
	pr_info("Major = %d, Minor = %d \n", MAJOR(dev), MINOR(dev));
	
	cdev_init(&device_cdev, &fops);
	
	if((cdev_add(&device_cdev, dev, 1))<0)
	{
		pr_err("kernel do not recive report add device\n");
		goto r_class;
	}
	
	if((dev_class = class_create(THIS_MODULE, "real_class")) == NULL)
	{
		pr_err("don't create class\n");
		goto r_class;
	}
	
	if((device_create(dev_class, NULL, dev, NULL, "real_device_file"))==NULL)
	{
		pr_err("can not create device file\n");
		goto r_device;
	}
	if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0)
	{
            pr_info("Cannot allocate memory in kernel\n");
            goto r_device;
        }
        strcpy(kernel_buffer, "Hello_World");
	pr_info("create ops device file succeed \n");
	return 0;
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);
	return -1;
}

static void __exit real_driver_exit(void)
{
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&device_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("driver was removed \n end \n");
}

module_init(real_driver_init);
module_exit(real_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mavis <justice172k@gmail.com>");
MODULE_DESCRIPTION("real driver");
MODULE_VERSION("1.4");

