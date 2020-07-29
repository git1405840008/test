#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define CDEVNAME "mycdev"

int major = 0;
char kbuf[128] = {0};
int mycdev_open(struct inode *inode, struct file *file)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
ssize_t mycdev_read(struct file *file, char __user *ubuf, 
		size_t size, loff_t *offs)
{
	int ret;
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	if(size > sizeof(kbuf)) size = sizeof(kbuf);
	ret = copy_to_user(ubuf,kbuf,size);
	if(ret){
		printk("copy data to user error");
		return -EIO ; //返回错误码
	}

	return size;
}
ssize_t mycdev_write(struct file *file, 
		const char __user *ubuf,
		size_t size, loff_t *offs)
{
	int ret;
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	if(size > sizeof(kbuf))size = sizeof(kbuf);
	ret = copy_from_user(kbuf,ubuf,size);
	if(ret){
		printk("copy data from user error");
		return -EIO ; //返回错误码
	}
	printk("kbuf = %s\n",kbuf);

	return size;
}
int mycdev_close(struct inode *inode, struct file *file)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}

static struct file_operations fops = {
	.open    = mycdev_open,
	.read    = mycdev_read,
	.write   = mycdev_write,
	.release = mycdev_close,
};

//入口
static int __init demo_init(void)
{
	//注册字符设备驱动
	major = register_chrdev(major,CDEVNAME,&fops);
	if(major < 0){
		printk("register char devices driver error\n");
		return major; //失败返回的是错误码
	}

	return 0;
}

//出口
static void  __exit demo_exit(void)
{
	//注销字符设备驱动
	unregister_chrdev(major,CDEVNAME);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
