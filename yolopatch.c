#include <linux/module.h>
#include <linux/init.h>

static int __init mymodule_init(void)
{
    printk ("420 NOSCOPE SWAGSHOT.\n");
    *(unsigned char *) 0xffffffff81360853 = 0x9;
    return 0;
}

static void __exit mymodule_exit(void)
{
    printk ("Unloading my module.\n");
    return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("BSD");
