#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>


static int __init hello_world_init(void)
{
	pr_info("hello world \n");
	return 0;
}
static void __exit hello_world_exit(void)
{
	pr_info("thanks\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mavis <justice172k@gmail.com>");
MODULE_DESCRIPTION("a simple hello world driver");
MODULE_VERSION("2:1.0");
