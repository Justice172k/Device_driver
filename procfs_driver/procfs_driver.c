#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h> 
#include <linux/uaccess.h> 
#include <linux/ioctl.h>
#include <linux/proc_fs.h>

#define KERNEL_VERSION	513

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

int32_t value = 0;

dev_t dev = 0; 
static struct class *dev_class;
static struct cdev device_cdev;

char xxx_array[20] = "try_pro_array\n";
static int len = 1;

static struct proc_dir_entry *parent;

static int open_proc(struct inode *inode, struct file *file)
{
    pr_info("proc file opend.....\t");
    return 0;
}

static int release_proc(struct inode *inode, struct file *file)
{
    pr_info("proc file released.....\n");
    return 0;
}

static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length,loff_t * offset)
{
    pr_info("proc file read.....\n");
    if(len)
    {
        len=0;
    }
    else
    {
        len=1;
        return 0;
    }
    
    if( copy_to_user(buffer,xxx_array,20) )
    {
        pr_err("Data Send : Err!\n");
    }
 
    return length;;
}

static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    pr_info("proc file wrote.....\n");
    
    if( copy_from_user(xxx_array,buff,len) )
    {
        pr_err("Data Write : Err!\n");
    }
    
    return len;
}

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

static ssize_t read_device(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
	pr_info("function read_device called\n");
	return 0;
}


static ssize_t write_device(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
	pr_info("function write_device called\n");
	return len;
}

static long ioctl_device(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                        {
                                pr_err("Data Write : Err!\n");
                        }
                        pr_info("Value = %d\n", value);
                        break;
                case RD_VALUE:
                        if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Data Read : Err!\n");
                        }
                        break;
                default:
                        pr_info("Default\n");
                        break;
        }
        return 0;
}

static struct file_operations fops = 
{
	.owner	= THIS_MODULE,
	.read	= read_device,
	.write	= write_device,
	.open	= open_device,
	.release= release_device,
	.unlocked_ioctl=ioctl_device,
};

static struct proc_ops proc_fops = {
        .proc_open = open_proc,
        .proc_read = read_proc,
        .proc_write = write_proc,
        .proc_release = release_proc
};

static int __init procfs_driver_init(void)
{
	if((alloc_chrdev_region(&dev, 2, 1, "xxx_device"))<0)
	{
		pr_err("error allocate major number\n");// cap phat major number
		return -1;
	}
	pr_info("Major = %d, Minor = %d \n", MAJOR(dev), MINOR(dev));
	
	cdev_init(&device_cdev, &fops);// khoi tao device_cdev theo fops
	
	if((cdev_add(&device_cdev, dev, 1))<0)
	{
		pr_err("kernel do not recive report add device\n");//them char device vao system
		goto r_class;
	}
	
	if((dev_class = class_create(THIS_MODULE, "mavis_class")) == NULL)
	{
		pr_err("don't create class\n");//tao ra 1 class chua cac thiet bi
		goto r_class;
	}
	
	if((device_create(dev_class, NULL, dev, NULL, "xxx_device_file"))==NULL)// tao ra cai device_file trong class
	{
		pr_err("can not create device file\n");
		goto r_device;
	}
	
	parent = proc_mkdir("xxx", NULL);
	if(parent = NULL)
	{
		pr_err("err create pro entry\n");
		goto r_device;
	}
	
	proc_create("xxx_proc", 0666, parent, &proc_fops);
	
	pr_info("create xxx device file succeed \n");
	return 0;
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);
	return -1;
}

static void __exit procfs_driver_exit(void)
{
	proc_remove(parent);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&device_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("driver was removed \nend \n");
}


module_init(procfs_driver_init);
module_exit(procfs_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mavis <justice172k@gmail.com>");
MODULE_DESCRIPTION("procfs driver");
MODULE_VERSION("1.6");
