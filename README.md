# kernel-safebox

Hooking system call to build a secure storage area

# Dependencies

* readline
* ncurses
* crypto++

# Modules

Hook getdents & read & write

```bash
cd module && make
sudo insmod safebox.ko
```

# Client

Commands:  

* put \[filename ouside directory\] \[filename inside directory\] \[password\]
* get \[filename inside directory\] \[filename ouside directory\] \[password\]
* list \[password\] \[optional: match string\]

```bash
cd client && make
./client
```