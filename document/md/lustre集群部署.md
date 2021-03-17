## Lustre集群部署


| 作者 | 时间 |QQ群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2021/03/14 |672152841 |

### kernel版本信息

```
[root@CentOS1 ~]# cat /etc/redhat-release 
CentOS Linux release 7.7.1908 (Core)
[root@CentOS1 ~]# uname -r
3.10.0-1062.el7.x86_64
```

### 配置lustre安装源
```
// lustre-2.14.0 内核刚好匹配 kernel 3.10.0-1062.el7.x86_64
[root@CentOS1 lustre]# pwd
/root/lustre
[root@CentOS1 lustre]# ls
repo.conf
[root@CentOS1 lustre]# cat lustre.repo 
[lustre-server]
name=lustre-server
baseurl=https://downloads.whamcloud.com/public/lustre/lustre-2.14.0/el7.7.1908/server
gpgcheck=0


[patchless-ldiskfs-server]
name=patchless-ldiskfs-server
baseurl=https://downloads.whamcloud.com/public/lustre/lustre-2.14.0/el7.7.1908/patchless-ldiskfs-server
gpgcheck=0

[lustre-client]
name=lustre-client
baseurl=https://downloads.whamcloud.com/public/lustre/lustre-2.14.0/el7.7.1908/client
gpgcheck=0

[e2fsprogs-wc]
name=e2fsprogs-wc
baseurl=https://downloads.whamcloud.com/public/e2fsprogs/latest/el7
gpgcheck=0

[root@CentOS1 lustre]# cp  lustre.repo /etc/yum.repo/lustre.repo
```

- 预安装

```
[root@CentOS1 ~]# cd ~/lustre
[root@CentOS1 ~]# yum groupinstall “Development Tools” -y
[root@CentOS1 ~]# yum install epel-release quilt libselinux-devel python-docutils xmlto asciidoc elfutils-libelf-devel elfutils-devel zlib-devel rng-tools binutils-devel python-devel sg3_utils newt-devel perl-ExtUtils-Embed audit-libs-devel lsof hmaccalc -y
[root@CentOS1 ~]# systemctl stop firewalld.service
[root@CentOS1 ~]# systemctl disable firewalld.service
[root@CentOS1 lustre]# yum remove kernel-tools-libs kernel-tools kernel-headers kernel-debug-devel -y
[root@CentOS1 lustre]# yum --nogpgcheck --disablerepo=base,extras,updates \
--enablerepo=lustre-server install \
kernel \
kernel-devel \
kernel-headers \
kernel-tools \
kernel-tools-libs \
kernel-tools-libs-devel
```

### 安装lustre

```

[root@CentOS1 lustre]# yum install epel-release quilt libselinux-devel python-docutils xmlto asciidoc elfutils-libelf-devel elfutils-devel zlib-devel rng-tools binutils-devel python-devel sg3_utils newt-devel perl-ExtUtils-Embed audit-libs-devel lsof hmaccalc asciidoc audit-libs-devel automake bc binutils-devel bison device-mapper-devel elfutils-devel elfutils-libelf-devel expect flex gcc gcc-c++ git glib2 glib2-devel hmaccalc keyutils-libs-devel krb5-devel ksh ibattr-devel libblkid-devel libselinux-devel libtool libuuid-devel libyaml-devel lsscsi make ncurses-devel net-snmp-devel net-tools newt-devel numactl-devel parted patchutils pciutils-devel perl-ExtUtils-Embed pesign python-devel redhat-rpm-config rpm-build systemd-devel tcl tcl-devel tk tk-devel wget xmlto yum-utils zlib-devel linux-firmware  dkms -y 

[root@CentOS1 lustre]# yum --nogpgcheck --enablerepo=lustre-server install \
kmod-lustre-osd-ldiskfs \
lustre-dkms \
lustre-osd-ldiskfs-mount \
lustre-osd-zfs-mount \
lustre lustre-debuginfo  \
lustre-resource-agents \
zfs  zfs-debuginfo

[root@CentOS1 ~] reboot
```

### 验证lustre安装
```
[root@CentOS1 x86_64]# modprobe -v zfs
[root@CentOS1 x86_64]# modprobe -v lustre
[root@CentOS1 x86_64]# modprobe -v lnet
[root@CentOS1 x86_64]# lsmod  |grep zfs
[root@CentOS1 ~]# lsmod  |grep lustre
[root@CentOS1 ~]# lsmod  |grep lnet

```

### 三个节点分别创建MDS

- 节点172.16.84.43
```
//节点1，每台机器的mgt和mdt共享；有条件可以单独部署
wipefs -a /dev/sdg
mkdir -p /lustre/mdt
mkfs.lustre --fsname=bigfs --mgs --mdt --index=0   --backfstype=zfs --reformat mdspool/mdt /dev/sdg 
mount.lustre mdspool/mdt /lustre/mdt/

//和上面是等价的命令，可以针对zfs pool进行调整
mkdir -p /lustre/mdt
zpool create -O canmount=off    -o cachefile=none mdspool /dev/sdb
mkfs.lustre --fsname=bigfs --mgs --mdt --index=0 \
--backfstype=zfs --reformat mdspool/mdt 
mount.lustre mdspool/mdt /lustre/mdt/
```
- 节点172.16.84.44
```
//清理磁盘的文件系统信息
wipefs -a /dev/sdg
//创建该节点挂载mdt的目录
mkdir -p /lustre/mdt
// 使用zfs初始化后端的mdt,当前配置中mgs和mdt是共享一个磁盘，每个mdt的index必须是唯一的，否则服务启动会有问题，同时mdspool/mdt中,mdspool是zfs pool，mdt是提供给lustre使用
mkfs.lustre --fsname=bigfs --mgs --mdt --index=2   --backfstype=zfs --reformat mdspool/mdt /dev/sdg
//挂载时候就启动了该节点的lustre mds服务，元数据信息写入到了mdt中
mount.lustre mdspool/mdt /lustre/mdt/

//注意当执行unmount /lustre/mdt就是停止mds的服务
```
- 节点172.16.84.45
```
//节点3
wipefs -a /dev/sdg
mkdir -p /lustre/mdt
mkfs.lustre --fsname=bigfs --mgs --mdt --index=3   --backfstype=zfs --reformat mdspool/mdt /dev/sdg 
mount.lustre mdspool/mdt /lustre/mdt/
```
### 配置三个节点的ost(每台机器2个磁盘)

- 节点172.16.84.43
```
//该节点创建2个ost挂载目录
mkdir -p /lustre/ost0
mkdir -p /lustre/ost1
//清除该该节点的文件系统信息
wipefs -a /dev/sde
wipefs -a /dev/sdf

//每个ost的pool名称必须不同,比如ost0 的pool名称是osspool0/ost,ost1的名称是osspool/ost,其中osspool-{xx}必须不同，全卷的ost的index必须不同。
//创建ost时候必须先把整个集群节点的mds服务准备好,每个ost节点必须使用mgsnode来连接当前集群中所有的mds的所有节点

//创建index=0的ost,fsname为lustre文件系统名称;index为当前集群中的ost的索引值，必须全局唯一;msgnode指定当前所有mds所在节点;backfstype用于执行后端ost所使用的文件系统;osspool0/ost用于指定zfs pool和lustre内使用的名称ost
mkfs.lustre --ost --reformat --fsname bigfs --index 0 --mgsnode 172.16.84.43@tcp0  --mgsnode 172.16.84.44@tcp0  --mgsnode 
172.16.84.45@tcp0 --backfstype zfs osspool0/ost /dev/sde 

mkfs.lustre --ost --reformat --fsname bigfs --index 1 --mgsnode 172.16.84.43@tcp0  --mgsnode 172.16.84.44@tcp0  --mgsnode 172.16.84.45@tcp0  --backfstype zfs osspool1/ost /dev/sdf 

```

- 节点172.16.84.44

```

mkdir -p /lustre/ost2
mkdir -p /lustre/ost3
wipefs -a /dev/sde
wipefs -a /dev/sdf
mkfs.lustre --ost --reformat --fsname bigfs --index 2 --mgsnode 172.16.84.43@tcp0  --mgsnode 172.16.84.44@tcp0  --mgsnode 172.16.84.45@tcp0 --backfstype zfs osspool0/ost /dev/sde 

mkfs.lustre --ost --reformat --fsname bigfs --index 3 --mgsnode 172.16.84.43@tcp0  --mgsnode 172.16.84.44@tcp0  --mgsnode 172.16.84.45@tcp0  --backfstype zfs osspool1/ost /dev/sdf 

```

- 节点172.16.84.45

```

//节点3
mkdir -p /lustre/ost4
mkdir -p /lustre/ost5
wipefs -a /dev/sde
wipefs -a /dev/sdf

mkfs.lustre --ost --reformat --fsname bigfs --index 4 --mgsnode 172.16.84.43@tcp0  --mgsnode 172.16.84.44@tcp0  --mgsnode 172.16.84.45@tcp0 --backfstype zfs osspool0/ost /dev/sde 
mkfs.lustre --ost --reformat --fsname bigfs --index 5 --mgsnode 172.16.84.43@tcp0  --mgsnode 172.16.84.44@tcp0  --mgsnode 172.16.84.45@tcp0  --backfstype zfs osspool1/ost /dev/sdf 

```
### 客户端节点挂载
```
//针对当前内核模块无损的方式来安装lustre客户端的kernel module
yum --nogpgcheck --enablerepo=lustre-client  install kmod-lustre-client lustre-client

//客户端加载lustre内核模块
modprobe -v lustre
//挂载lustre文件系统，挂载方式是 mount -t luster mds1@tcp0:mds2@tcp0:mds3@tcp0/{lustre_name} /mnt/lustre
mount -t lustre 172.16.84.43@tcp:172.16.84.44@tcp:172.16.84.45@tcp:/bigfs /mnt/lustre/

```

###  lustre process信息

```
//show luste mgs info
[root@CentOS1 ~]# ps -ef|grep mgs
[root@CentOS1 ~]# ps -ef|grep ost
[root@CentOS1 ~]# ps -ef|grep mdt
```

### lustre 参数调优入门

```
 // 可配参数查看
# lctl list_param ost.*
ost.OSS
# lctl list_param ost.OSS.*
ost.OSS.ost
ost.OSS.ost_create
ost.OSS.ost_io
ost.OSS.ost_out
ost.OSS.ost_seq
ost.OSS.uuid


# lctl list_param ost.OSS.ost.*
ost.OSS.ost.high_priority_ratio
ost.OSS.ost.threads_max
ost.OSS.ost.threads_min
ost.OSS.ost.threads_started
ost.OSS.ost.nrs_crrn_quantum
ost.OSS.ost.nrs_delay_max
ost.OSS.ost.nrs_delay_min
ost.OSS.ost.nrs_delay_pct
ost.OSS.ost.nrs_policies
ost.OSS.ost.nrs_tbf_rule
ost.OSS.ost.req_buffer_history_len
ost.OSS.ost.req_buffer_history_max
ost.OSS.ost.req_buffers_max
ost.OSS.ost.req_history
ost.OSS.ost.stats
ost.OSS.ost.timeouts

// 设置参数
# lctl set_param ost.OSS.ost.threads_max 64
# lctl set_param ost.OSS.ost.threads_min 64


# lctl list_param mdt.*
mdt.lustrefs-MDT0002
# lctl list_param mgs.*
mgs.MGS

// 获取参数配置
# lctl list_param mdt.lustrefs-MDT0002.*
mdt.lustrefs-MDT0002.client_cache_count
mdt.lustrefs-MDT0002.client_cache_seconds
# lctl get_param mdt.lustrefs-MDT0002.client_cache_count
mdt.lustrefs-MDT0002.client_cache_count=128
# lctl set_param mdt.lustrefs-MDT0002.client_cache_count 8192
mdt.lustrefs-MDT0002.client_cache_count=8192
```
### Lustre 日常的命令使用介绍
- 查看zfs pool的信息
```
zfs list -o name,used,avail,refer
```
- 查看ost服务的相关进程
```
ps -ef | awk '/ost/ && !/awk/'
```
- lctl查看能使用命令
```
lctl --list-commands
```
- 查看当前mds/mgs/ost的参数
```
//查看mgs的参数
lctl list_param -RF mgs
//查看mds的参数
lctl list_param -RF mds
//查看ost的参数
lctl list_param -RF ost
```
### Lustre和Glusterfs 存储写粗略带宽对比(仅仅是个粗略测试)

- gluster 采用哈希卷，按照4M分片;lustre采用默认,相同的磁盘的情况下，lustre的写带宽是glusterfs的3倍左右。
- glusterfs 哈希卷大文件写的带宽，持续观察发现glusterfs的写带宽不是很稳定，波动比较大（120MB/S ~370MB/S)

![gluster-write](../images/gluster-write.jpg)

- lustre 大文件写的平均带宽，相对比较稳定

![lustre_write](../images/lustre_write.jpg)
