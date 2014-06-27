/*
 * Copyright (c) 2014 Matilda <sadieperkins@riseup.net>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any 
 * purpose with or without fee is hereby granted, provided that the above 
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES 
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY 
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE 
 * USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/bug.h>
#include <linux/ctype.h>
#include <linux/debugfs.h>
#include <linux/errno.h>
#include <linux/kallsyms.h>
#include <linux/kobject.h>
#include <linux/kthread.h>
#include <linux/pagemap.h>
#include <linux/sched.h>
#include <linux/sort.h>
#include <linux/stop_machine.h>
#include <linux/sysfs.h>
#include <linux/time.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

static int __init mymodule_init(void)
{
    uint8_t *target = 0xffffffff8135a5e3;
    struct page **pages = kmalloc(sizeof(*pages), GFP_KERNEL);
    void *vmapped;

    printk("entering mymodule_init \n");
    if (NULL == pages) {
        return 69;
    }

    pages[0] = virt_to_page((void *) ((uint64_t) target & PAGE_MASK));
    
    if (NULL == pages[0]) {
        kfree(pages);
        return 69;
    }

    vmapped = vmap(pages, 1, VM_MAP, PAGE_KERNEL);
    kfree(pages);
    if (NULL == vmapped) {
        return 42;
    }

    printk("trying noscope swagshot\n");
    printk("420 NOSCOPE READ A BYTE, %d\n", * (uint8_t *) (vmapped + offset_in_page(target)));
    if (0x3 == * (uint8_t *) (vmapped + offset_in_page(target))) { 
        /* CORRECT BYTE LET THE YOLOPATCHING BEGIN */
    * (uint8_t *) (vmapped + offset_in_page(target)) = 0x09;
    flush_icache_range(target - 100, target + 100);
    printk("420 NOSCOPE SWAGSHOTTED #yolo #swag #420\n");
    } else {
        printk("DIDNT FIND THE EXPECTED BYTE, NO PATCH DONE\n");
    }


    vunmap(vmapped);
    return 0;
}

static void __exit mymodule_exit(void)
{
    printk("Unloading my module.\n");
    return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("BSD");
