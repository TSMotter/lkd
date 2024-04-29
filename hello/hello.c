#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Giuliano M");
MODULE_DESCRIPTION("Hello World module");
MODULE_VERSION("1.0.0");

/*
- A return of a negative value in any kernel code corresponds to the 
corresponding error code. Ex: return -EIO;

- Return 0 = success
*/
static int __init helloworld_init(void)
{
    printk(KERN_INFO "Hello world!\n");
    return 0;
}

static void __exit helloworld_exit(void)
{
    printk(KERN_INFO "Goodbye world!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);
