## OpenCAS 内核模块加载

| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2020/12/01 |672152841 |
##### 内核模块基本研发步骤介绍

- 模块加载函数，当通过insmod或者modprobe命令加载内核模块，模块加载函数会自动在内核中执行模块初始化函数
- 模块卸载函数,当rmmod命令卸载内核模块时候，会在内核执行模块的销毁函数
- 模块许可申明，许可证是用来描述内核模块的许可权限
- 模块的参数，模块参数是模块被加载时候可以传递参数，它对应的模块的全局变量
- 模块到处符号，内核模块可以导出的符号(symbol，对应的变量或者函数)，导出后其他的内核模块可以使用本模块的函数或者变量
- 模块作者申请

##### 内核模块的样例

```
#include <linux/init.h>
#include <linux/module.h>

static int __init first_init(void)
{
    printk(KERN_INFO "first kernel module init");
    return 0;
}
module_init(first_init);
static void __exit first_exit(void)
{
    printk(KERN_INFO "first kernel module exit");
    return 0;
}
module_init(first_exit);

MODULE_AUTHOR("perrynzhou@gmail.com");
MODULE_LICENSE("GPL V2");
MODULE_DESCRIPTION("a demo for kernel module");
MODILE_ALIAS("a sample module");

```
##### 下载安装最新release版本

```
git clone https://github.com/Open-CAS/open-cas-linux
cd open-cas-linux && git checkout v20.12.2
git submodule update --init
//编译安装opencas内核模块
make -j4 && make install
// 安装完毕后可以在/var/log/message查看如下的信息
6153  Mar  7 20:07:47 CentOS kernel: Open Cache Acceleration Software Linux Version 20.12.2.0444.release (4.18.0-240.15.1.el8_3.x86_64)::Module loaded successfully
  7141  Mar 14 09:28:31 CentOS kernel: [Open-CAS] Not found configuration for upgrade. Standard module initialization.
  7142  Mar 14 09:28:31 CentOS kernel: Open Cache Acceleration Software Linux Version 20.12.2.0444.release (4.18.0-240.15.1.el8_3.x86_64)::Module loaded successfully

```
##### 安装完后Opencas做了什么事情？
- 当opencas初始化完毕后会加载cas_cache和cas_disk这两个内核模块的init函数。
- cache_cache中的init函数会执行如下的函数调用核心函数，这个内核模块主要是通过module_init(cas_init_module)来完成对应的加载服务

```
static int __init cas_init_module(void)
{
	// 注册cas_disk导出的内核调用函数
	cas_casdisk_lookup_funtions();
	// opencas的版本检查
	casdisk_functions.casdsk_get_version();
	//ocf框架的初始化
	cas_initialize_context();
	//opencas的升级的配置文件
	cas_upgrade_get_configuration();
	//根据版本来进行升级
	cas_upgrade_finish();
	// opencas的服务的控制设备，这个主要是用casctl这个python来和它进行交互进行opencas的控制操作
	cas_ctrl_device_init();
  printk(KERN_INFO "%s Version %s (%s)::Module loaded successfully\n",
		OCF_PREFIX_LONG, CAS_VERSION, CAS_KERNEL);
}
```
- cache_disk的初始化通过module_init(casdsk_init_module)来进行，主要经历如下函数的调用
```
static int __init casdsk_init_module(void)
{
	// 内核函数导出对象的数据结构初始化
	casdsk_init_exp_objs();
	//按照cas名称来匹来注册设备
	casdsk_init_disks();
	//内核对象初始化，这里主要是加载 cas_disk.ko这个内核模块
	casdsk_init_kobjects();
	
  printk(CASDSK_KERN_INFO "%s Version %s (%s)::Module loaded successfully\n",
		CASDSK_LOGO, CAS_VERSION, CAS_KERNEL);
}
```