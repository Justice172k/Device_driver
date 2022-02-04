#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>

dev_t device = 0;

static int __init dyn_allocating_init(void)
{
	if(alloc_chrdev_region(&device, 0, 1, "first device")<0)
	{
		pr_info("not succeed\n");
		return -1;
	}
	pr_info("major = %d Minor = %d \n", MAJOR(device), MINOR(device));
	pr_info("allocat succeed\n");
	return 0;
}
static void __exit dyn_allocating_exit(void)
{
	unregister_chrdev_region(device, 1);
	pr_info("end\n");
}

module_init(dyn_allocating_init);
module_exit(dyn_allocating_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mavis <justice172k@gmail.com>");
MODULE_DESCRIPTION("dynamically allocating");
MODULE_VERSION("1.0");
