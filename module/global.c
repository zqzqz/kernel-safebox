#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <linux/unistd.h>  /* sys_call_table __NR_* system call function indices */
#include <linux/dirent.h> /* linux_dirent64 */

unsigned long *syscall_table = NULL;
uid_t manager_uid = 1000;
const char manager_dir[256] = "/home/ubuntu/safebox/box";

asmlinkage int (*original_write)(unsigned int, const char __user *, size_t);
asmlinkage int (*original_read)(unsigned int, char __user *, size_t);
asmlinkage int (*original_creat)(const char __user *, int);
asmlinkage int (*original_unlink)(const char *);
asmlinkage int (*original_getdents64)(unsigned int, struct linux_dirent64 *, unsigned int);
asmlinkage int (*original_getdents)(unsigned int, struct linux_dirent64 *, unsigned int);