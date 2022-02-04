#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

int value, arr[4];
char *name;
int cb_value = 0;
// khoi tao tham so truyen vao kernel

module_param(value, int, S_IRUSR|S_IWUSR);
module_param(name, charp, S_IRUSR|S_IWUSR);
module_param_array(arr, int, NULL, S_IRUSR|S_IWUSR);

// Khoi tao ham thong bao tham so thay doi trong kernel
int notify_param(const char *val, const struct kernel_param *kp)
{
	int res = param_set_int(val, kp);
	if(res==0)
	{
		pr_info("cb function succeed\n");
		pr_info("new cb_value = %d\n", cb_value);
		return 0;
	}
	return -1;
}

const struct kernel_param_ops my_param_ops =
{
	.set = &notify_param,
	.get = &param_get_int,
};

module_param_cb(cb_value, &my_param_ops, &cb_value, S_IRUGO|S_IWUSR);

static int __init pass_argv_init(void)
{
	int i = 0;
	pr_info("statrted\n");
	pr_info("value = %d \n", value);
	pr_info("cb_value = %d \n", cb_value);
	pr_info("name = %s \n", name);
	for(i = 0; i < 4; i++)
		pr_info("arr[%d] = %d\n", i, arr[i]);
	return 0;
}
static void __exit pass_argv_exit(void)
{
	pr_info("end\n");
}

module_init(pass_argv_init);
module_exit(pass_argv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mavis <justice172k@gmail.com>");
MODULE_DESCRIPTION("a simple pass argv driver");
MODULE_VERSION("2:1.0");

