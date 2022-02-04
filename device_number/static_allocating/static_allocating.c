#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>

dev_t device = MKDEV(210, 0);

static int __init static_allocating_init(void)
{
	register_chrdev_region(device, 1, "first device");
	pr_info("major = %d Minor = %d \n", MAJOR(device), MINOR(device));
	pr_info("allocat succeed\n");
	return 0;
}
static void __exit static_allocating_exit(void)
{
	unregister_chrdev_region(device, 1);
	pr_info("end\n");
}

module_init(static_allocating_init);
module_exit(static_allocating_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mavis <justice172k@gmail.com>");
MODULE_DESCRIPTION("static allocating");
MODULE_VERSION("1.0");

