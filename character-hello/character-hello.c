#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Giuliano M");
MODULE_DESCRIPTION("Hello World character device");
MODULE_VERSION("1.0.0");

/*
- Will store any data that is written to us here
- Anyone who writes to our `/dev/<>` file, kernel will provide access to the
the data that was written, in this buffer.
- Ex: If someone writes "hello" to the file, we'll be able to access that data 
in this buffer here.
- This related to the read and write functions
*/
static char kern_buf[100];

/*
- These functions are gonna be called by the kernel when you operate on the file
referent to this device
*/
static int dev_open(struct inode* inode, struct file* file);
/* Here, `char* buf` is the buffer provided to us that we can read from */
static ssize_t dev_read(struct file* file, char* buf, size_t len, loff_t* off);
static ssize_t dev_write(struct file* file, const char* buf, size_t len, loff_t* off);
static int dev_release(struct inode* inode, struct file* file);

/*
- Function pointer are implemented in this structure
- The file structure has something called "private data" which is a void*
    - You can allocate memory and assign that void* to this memory location
    - It'll act as a class member variable, that is, you can keep context 
        between different independent calls to dev_read, dev_write, dev_open, dev_release

- These file operations (fops) are the same way as the file system works
*/
static struct file_operations fops =
{
    .read=dev_read,
    .write=dev_write,
    .open=dev_open,
    .release=dev_release
};


/*
- A return of a negative value in any kernel code corresponds to the 
corresponding error code. Ex: return -EIO;

- Return 0 = success
*/
static int __init helloworld_init(void)
{
    /* Registering the chrdevice with MAJOR=98 and name "mydev" */
    int t = register_chrdev(98, "mydev", &fops);
    if (t < 0)
    {
        printk(KERN_ERR "[Hello.ko] Problem registering the character device");
        return -EIO;
    }

    printk(KERN_INFO "[Hello.ko] init OK\n");
    return 0;
}

static int dev_open(struct inode* inode, struct file* file)
{
    printk(KERN_ERR "[Hello.ko] open OK\n");
    return 0;
}

static ssize_t dev_read(struct file* file, char* buf, size_t len, loff_t* off)
{
    /* If trying to read above the size of the buffer, return 0 indicating that
    nothing was read */
    if(*off >= sizeof(kern_buf))
    {
        printk(KERN_INFO "[Hello.ko] read ERR\n");
        return 0;
    }

    len = len > sizeof(kern_buf) ? sizeof(kern_buf) : len;

    /* Safe routine that allow to copy FROM kernel space INTO user space, in the
    provided buffer */
    copy_to_user(buf, kern_buf, len);
    printk(KERN_INFO "[Hello.ko] read OK\n");
    return len;
}

static ssize_t dev_write(struct file* file, const char* buf, size_t len, loff_t* off)
{

    if (len >= sizeof(kern_buf))
    {
        printk(KERN_INFO "[Hello.ko] write ERR\n");
        return -EIO;
    }

    /* Safe routine that allow to copy FROM user space INTO kernel space */
    memset(kern_buf, 0, sizeof(kern_buf));
    copy_from_user(kern_buf, buf, len);
    kern_buf[len] = 0; // NULL terminator
    printk(KERN_INFO "[Hello.ko] write OK\n");
    return len;
}

static int dev_release(struct inode* inode, struct file* file)
{
    printk(KERN_ERR "[Hello.ko] release OK\n");
    return 0;
}

static void __exit helloworld_exit(void)
{
    /* Unregistering the chrdevice with MAJOR=98 and name "mydev" */
    unregister_chrdev(98, "mydev");
    printk(KERN_INFO "[Hello.ko] exit OK\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);
