#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>

dev_t dev = 0;

static struct class *dev_class_2;

static int __init auto_create_init(void)
{
	if((alloc_chrdev_region(&dev, 0, 1, "name_dev"))<0)
	{
		pr_err("not succeed allocat\n");
		return -1;
	}
	pr_info("major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
	
	if((dev_class_2 = class_create(THIS_MODULE, "dev_class")) == NULL)
	{
		pr_err("not succeed create class\n");
		goto r_class;
	}
	if((device_create(dev_class_2, NULL, dev, NULL, "device_file"))==NULL)
	{
		pr_err("not succeed create device file\n");
		goto r_device;
	}
	pr_info("create device file succeed\n");
	return 0;
	r_device:
		class_destroy(dev_class_2);
	r_class:
		unregister_chrdev_region(dev, 1);
	return -1;	
}
static void __exit auto_create_exit(void)
{
	device_destroy(dev_class_2, dev);
	class_destroy(dev_class_2);
	unregister_chrdev_region(dev, 1);
	pr_info("end\n");
}

module_init(auto_create_init);
module_exit(auto_create_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mavis <justice172k@gmail.com>");
MODULE_DESCRIPTION("auto create device file");
MODULE_VERSION("1.0");
