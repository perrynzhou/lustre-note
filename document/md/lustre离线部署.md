## lustre-2.13部署

### chapter 1.lustre部署
#### 0.节点信息

| node                       | role    |
| -------------------------- | ------- |
| centos1(mgs_node/mds_node) | mgs/mds |
| centos2(oss_node)          | oss     |
| centos3(client_node)       | client  |

#### 1.kernel版本信息

```
[root@CentOS1 ~]# cat /etc/redhat-release 
CentOS Linux release 7.7.1908 (Core)
[root@CentOS1 ~]# uname -r
3.10.0-1062.el7.x86_64
```

#### 2.配置离线lustre 安装包安装源
```
// lustre-2.13.0 内核刚好匹配 kernel 3.10.0-1062.el7.x86_64
[root@CentOS1 lustre]# pwd
/root/lustre
[root@CentOS1 lustre]# ls
repo.conf
[root@CentOS1 lustre]# cat repo.conf 
[lustre-server]
name=lustre-server
baseurl=https://downloads.whamcloud.com/public/lustre/lustre-2.13.0/el7.7.1908/server
gpgcheck=0


[patchless-ldiskfs-server]
name=patchless-ldiskfs-server
baseurl=https://downloads.whamcloud.com/public/lustre/lustre-2.13.0/el7.7.1908/patchless-ldiskfs-server
gpgcheck=0

[lustre-client]
name=lustre-client
baseurl=https://downloads.whamcloud.com/public/lustre/lustre-2.13.0/el7.7.1908/client
gpgcheck=0

[e2fsprogs-wc]
name=e2fsprogs-wc
baseurl=https://downloads.whamcloud.com/public/e2fsprogs/latest/el7
gpgcheck=0
```

#### 3.下载lustre安装包到本地

```
[root@CentOS1 ~]# cd ~/lustre
[root@CentOS1 ~]# yum groupinstall “Development Tools” -y
[root@CentOS1 ~]# yum install epel-release quilt libselinux-devel python-docutils xmlto asciidoc elfutils-libelf-devel elfutils-devel zlib-devel rng-tools binutils-devel python-devel sg3_utils newt-devel perl-ExtUtils-Embed audit-libs-devel lsof hmaccalc -y
[root@CentOS1 ~]# systemctl stop firewalld.service
[root@CentOS1 ~]# systemctl disable firewalld.service
[root@CentOS1 ~]# yum install  yum-utils dkms-y
[root@CentOS1 ~]# reposync -c lustre-repo.conf -n \
-r lustre-server \
-r lustre-client \
-r patchless-ldiskfs-server \
-r e2fsprogs-wc
[root@CentOS1 ~]# cp ~/lustre/repo.conf /etc/yum.repos.d/lustre.repo
```

#### 4.安装lustre版本kernel

```
//after download all lustre package
[root@CentOS1 lustre]# ls
e2fsprogs-wc  lustre-client  lustre-server  patchless-ldiskfs-server  repo.conf

[root@CentOS1 lustre]# yum remove -y    kernel-tools-libs  kernel-tools kernel-headers  kernel-debug-devel  kernel-debuginfo

[root@CentOS1 lustre]# cd e2fsprogs-wc/RPMS/x86_64/ && pwd
/root/lustre/e2fsprogs-wc/RPMS/x86_64 && yum --nogpgcheck --disablerepo=* --enablerepo=e2fsprogs-wc install  e2fsprogs-1.45.2.wc1-0.el7.x86_64.rpm 

[root@CentOS1 lustre]# cd lustre-server/RPMS/x86_64/ && pwd 
/root/lustre/lustre-server/RPMS/x86_64
yum --nogpgcheck --disablerepo=* --enablerepo=e2fsprogs-wc install  e2fsprogs-1.45.2.wc1-0.el7.x86_64.rpm


yum --nogpgcheck --disablerepo=* --enablerepo=lustre-server install  kernel-devel-3.10.0-1062.1.1.el7_lustre.x86_64.rpm  kernel-headers-3.10.0-1062.1.1.el7_lustre.x86_64.rpm kernel-tools-libs-3.10.0-1062.1.1.el7_lustre.x86_64.rpm 
[root@CentOS1 ~] reboot
```
#### 5.安装zfs
```
[root@CentOS1 lustre]# yum install epel-release quilt libselinux-devel python-docutils xmlto asciidoc elfutils-libelf-devel elfutils-devel zlib-devel rng-tools binutils-devel python-devel sg3_utils newt-devel perl-ExtUtils-Embed audit-libs-devel lsof hmaccalc asciidoc audit-libs-devel automake bc binutils-devel bison device-mapper-devel elfutils-devel elfutils-libelf-devel expect flex gcc gcc-c++ git glib2 glib2-devel hmaccalc keyutils-libs-devel krb5-devel ksh ibattr-devel libblkid-devel libselinux-devel libtool libuuid-devel libyaml-devel lsscsi make ncurses-devel net-snmp-devel net-tools newt-devel numactl-devel parted patchutils pciutils-devel perl-ExtUtils-Embed pesign python-devel redhat-rpm-config rpm-build systemd-devel tcl tcl-devel tk tk-devel wget xmlto yum-utils zlib-devel linux-firmware  dkms -y

[root@CentOS1 lustre]# cd lustre-server/RPMS/x86_64/ && pwd 
/root/lustre/lustre-server/RPMS/x86_64

[root@CentOS1 x86_64]# yum --nogpgcheck --disablerepo=* --enablerepo=lustre-server install zfs-0.7.13-1.el7.x86_64.rpm  zfs-dkms-0.7.13-1.el7.noarch.rpm  zfs-dracut-0.7.13-1.el7.x86_64.rpm

[root@CentOS1 x86_64]# reboot
[root@CentOS1 x86_64]# modprobe  zfs
[root@CentOS1 x86_64]# lsmod  |grep zfs
zfs                  3564425  3 
zunicode              331170  1 zfs
zavl                   15236  1 zfs
icp                   270148  1 zfs
zcommon                73440  1 zfs
znvpair                89131  2 zfs,zcommon
spl                   102412  4 icp,zfs,zcommon,znvpair
```
#### 6.安装lustre server 核心包 
```
[root@CentOS1 ~] yum --nogpgcheck --disablerepo=*  --enablerepo=lustre-server install kmod-lustre-2.13.0-1.el7.x86_64.rpm kmod-lustre-osd-ldiskfs-2.13.0-1.el7.x86_64.rpm lustre-2.13.0-1.el7.x86_64.rpm lustre-osd-ldiskfs-mount-2.13.0-1.el7.x86_64.rpm lustre-osd-zfs-mount-2.13.0-1.el7.x86_64.rpm lustre-resource-agents-2.13.0-1.el7.x86_64.rpm

[root@CentOS1 ~]# reboot
[root@CentOS1 ~]# modprobe  lustre
[root@CentOS1 ~]# lsmod  |grep lustre
lustre                875887  0 
lmv                   191957  1 lustre
mdc                   247683  1 lustre
lov                   320485  1 lustre
ptlrpc               2287996  7 fid,fld,lmv,mdc,lov,osc,lustre
obdclass             2649116  8 fid,fld,lmv,mdc,lov,osc,lustre,ptlrpc
lnet                  595547  6 lmv,osc,lustre,obdclass,ptlrpc,ksocklnd
libcfs                388506  11 fid,fld,lmv,mdc,lov,osc,lnet,lustre,obdclass,ptlrpc,ksocklnd

[root@CentOS1 ~]# modprobe  lnet
[root@CentOS1 ~]# lsmod  |grep lnet
lnet                  595547  6 lmv,osc,lustre,obdclass,ptlrpc,ksocklnd
libcfs                388506  11 fid,fld,lmv,mdc,lov,osc,lnet,lustre,obdclass,ptlrpc,ksocklnd
```

#### 7.安装lustre client核心包(客户端节点)
```
//lustre server和lustre client版本版本冲突，客户端必须在其他节点安装
[root@CentOS3 ~]# yum install epel-release -y
[root@CentOS3 ~]# yum install dkms -y
[root@CentOS3 ~]# cd lustre/
[root@CentOS3 lustre]# cd lustre-client/RPMS/x86_64/
[root@CentOS3 x86_64]# ls
kmod-lustre-client-2.13.0-1.el7.x86_64.rpm        lustre-client-debuginfo-2.13.0-1.el7.x86_64.rpm  lustre-iokit-2.13.0-1.el7.x86_64.rpm
kmod-lustre-client-tests-2.13.0-1.el7.x86_64.rpm  lustre-client-dkms-2.13.0-1.el7.noarch.rpm
lustre-client-2.13.0-1.el7.x86_64.rpm             lustre-client-tests-2.13.0-1.el7.x86_64.rpm
[root@CentOS3 x86_64]# yum --nogpgcheck --disablerepo=*  --enablerepo=lustre-client install  kmod-lustre-client-2.13.0-1.el7.x86_64.rpm  lustre-client-2.13.0-1.el7.x86_64.rpm  lustre-client-dkms-2.13.0-1.el7.noarch.rpm
```


#### 8.deploy msg/mst

```
[root@CentOS1 ~]# fdisk -l|grep sd
Disk /dev/sda: 68.7 GB, 68719476736 bytes, 134217728 sectors
/dev/sda1   *        2048     2099199     1048576   83  Linux
/dev/sda2         2099200   134217727    66059264   8e  Linux LVM
Disk /dev/sdc: 25.8 GB, 25769803776 bytes, 50331648 sectors
Disk /dev/sdb: 25.8 GB, 25769803776 bytes, 50331648 sectors
[root@CentOS1 ~]# mkfs.lustre --mgs  /dev/sdb
[root@CentOS1 ~]# mkdir /mgt
[root@CentOS1 ~]# mount.lustre  /dev/sdb /mgt/
[root@CentOS1 ~]# df -h
Filesystem               Size  Used Avail Use% Mounted on
devtmpfs                 905M     0  905M   0% /dev
tmpfs                    917M     0  917M   0% /dev/shm
tmpfs                    917M  9.0M  908M   1% /run
tmpfs                    917M     0  917M   0% /sys/fs/cgroup
/dev/mapper/centos-root   41G  2.9G   39G   7% /
/dev/sda1               1014M  193M  822M  20% /boot
/dev/mapper/centos-home   20G   33M   20G   1% /home
tmpfs                    184M     0  184M   0% /run/user/0
/dev/sdb                  24G   46M   23G   1% /mgt
```
#### 9.deploy mds/mdt

```
//mkfs.lustre --fsname=lustrefs --mgsnode=msg_node@tcp0  --mdt --index=0 /dev/sdc
[root@CentOS1 ~]# mkfs.lustre --fsname=lustrefs --mgsnode=10.211.55.3@tcp0  --mdt --index=0 /dev/sdc

   Permanent disk data:
Target:     lustrefs:MDT0000
Index:      0
Lustre FS:  lustrefs
Mount type: ldiskfs
Flags:      0x61
              (MDT first_time update )
Persistent mount opts: user_xattr,errors=remount-ro
Parameters: mgsnode=10.211.55.3@tcp

checking for existing Lustre data: not found
device size = 24576MB
formatting backing filesystem ldiskfs on /dev/sdc
        target name   lustrefs:MDT0000
        kilobytes     25165824
        options        -J size=983 -I 1024 -i 2560 -q -O dirdata,uninit_bg,^extents,dir_nlink,quota,huge_file,ea_inode,flex_bg -E lazy_journal_init -F
mkfs_cmd = mke2fs -j -b 4096 -L lustrefs:MDT0000  -J size=983 -I 1024 -i 2560 -q -O dirdata,uninit_bg,^extents,dir_nlink,quota,huge_file,ea_inode,flex_bg -E lazy_journal_init -F /dev/sdc 25165824k
Writing CONFIGS/mountdata
[root@CentOS1 ~]# mkdir /mdt
[root@CentOS1 ~]# mount.lustre /dev/sdc  /mdt
```

#### 10.deploy oss/ost

```
//mkfs.lustre --ost --fsname=lustrefs --mgsnode=mgs_node@tcp0 --index=1 /dev/sdb

[root@CentOS2 ~]# mkfs.lustre --ost --fsname=lustrefs --mgsnode=10.211.55.3@tcp0 --index=1 /dev/sdb

   Permanent disk data:
Target:     lustrefs:OST0001
Index:      1
Lustre FS:  lustrefs
Mount type: ldiskfs
Flags:      0x62
              (OST first_time update )
Persistent mount opts: ,errors=remount-ro
Parameters: mgsnode=10.211.55.3@tcp

checking for existing Lustre data: not found
device size = 24576MB
formatting backing filesystem ldiskfs on /dev/sdb
        target name   lustrefs:OST0001
        kilobytes     25165824
        options        -J size=983 -I 512 -i 69905 -q -O extents,uninit_bg,dir_nlink,quota,huge_file,flex_bg -G 256 -E resize="4290772992",lazy_journal_init -F
mkfs_cmd = mke2fs -j -b 4096 -L lustrefs:OST0001  -J size=983 -I 512 -i 69905 -q -O extents,uninit_bg,dir_nlink,quota,huge_file,flex_bg -G 256 -E resize="4290772992",lazy_journal_init -F /dev/sdb 25165824k
Writing CONFIGS/mountdata
[root@CentOS2 ~]# mkdir /ost
[root@CentOS2 ~]# mount.lustre  /dev/sdb /ost/
```

#### 11.mount on client node

```
[root@CentOS3 ~]# mkdir /mnt/lustrefs
[root@CentOS3 ~]# mount -t lustre 10.211.55.3@tcp0:/lustrefs /mnt/lustrefs
[root@CentOS3 ~]# df -h
Filesystem                 Size  Used Avail Use% Mounted on
devtmpfs                   906M     0  906M   0% /dev
tmpfs                      917M     0  917M   0% /dev/shm
tmpfs                      917M  9.0M  908M   1% /run
tmpfs                      917M     0  917M   0% /sys/fs/cgroup
/dev/mapper/centos-root     41G  2.8G   39G   7% /
/dev/sda1                 1014M  149M  866M  15% /boot
/dev/mapper/centos-home     20G   33M   20G   1% /home
tmpfs                      184M     0  184M   0% /run/user/0
10.211.55.3@tcp:/lustrefs   23G   46M   22G   1% /mnt/lustrefs
```

### chapter 2. lustre process

```
//show luste mgs info
[root@CentOS1 ~]# ps -ef|grep mgs
root      2275     2  0 13:29 ?        00:00:00 [mgs_params_noti]
root      2276     2  0 13:29 ?        00:00:00 [ll_mgs_0000]
root      2277     2  0 13:29 ?        00:00:00 [ll_mgs_0001]
root      2278     2  0 13:29 ?        00:00:00 [ll_mgs_0002]
root      2375     2  0 13:31 ?        00:00:00 [mgs_lustrefs_no]
root      2548  2105  0 13:49 pts/0    00:00:00 grep --color=auto mgs

//show lustre mdt process
[root@CentOS1 ~]# ps -ef|grep mdt
root      2357     2  0 13:31 ?        00:00:00 [mdt00_000]
root      2358     2  0 13:31 ?        00:00:00 [mdt00_001]
root      2359     2  0 13:31 ?        00:00:01 [mdt00_002]
root      2360     2  0 13:31 ?        00:00:00 [mdt_rdpg00_000]
root      2361     2  0 13:31 ?        00:00:00 [mdt_rdpg00_001]
root      2362     2  0 13:31 ?        00:00:00 [mdt_attr00_000]
root      2363     2  0 13:31 ?        00:00:00 [mdt_attr00_001]
root      2364     2  0 13:31 ?        00:00:00 [mdt_out00_000]
root      2365     2  0 13:31 ?        00:00:00 [mdt_out00_001]
root      2366     2  0 13:31 ?        00:00:00 [mdt_seqs_0000]
root      2367     2  0 13:31 ?        00:00:00 [mdt_seqs_0001]
root      2368     2  0 13:31 ?        00:00:00 [mdt_seqm_0000]
root      2369     2  0 13:31 ?        00:00:00 [mdt_seqm_0001]
root      2370     2  0 13:31 ?        00:00:00 [mdt_fld_0000]
root      2371     2  0 13:31 ?        00:00:00 [mdt_fld_0001]
root      2372     2  0 13:31 ?        00:00:00 [mdt_io00_000]
root      2373     2  0 13:31 ?        00:00:00 [mdt_io00_001]
root      2374     2  0 13:31 ?        00:00:00 [mdt_io00_002]
root      2540     2  0 13:48 ?        00:00:00 [mdt00_003]
root      2552  2105  0 13:49 pts/0    00:00:00 grep --color=auto mdt

//show ost process
[root@CentOS2 ~]# ps -ef|grep ost
root      1854     1  0 13:09 ?        00:00:00 /usr/libexec/postfix/master -w
postfix   1859  1854  0 13:09 ?        00:00:00 pickup -l -t unix -u
postfix   1860  1854  0 13:09 ?        00:00:00 qmgr -l -t unix -u
root      2431     2  0 13:47 ?        00:00:00 [ll_ost00_000]
root      2432     2  0 13:47 ?        00:00:00 [ll_ost00_001]
root      2433     2  0 13:47 ?        00:00:00 [ll_ost00_002]
root      2434     2  0 13:47 ?        00:00:00 [ll_ost_create00]
root      2435     2  0 13:47 ?        00:00:00 [ll_ost_create00]
root      2436     2  0 13:47 ?        00:00:00 [ll_ost_io00_000]
root      2437     2  0 13:47 ?        00:00:00 [ll_ost_io00_001]
root      2438     2  0 13:47 ?        00:00:00 [ll_ost_io00_002]
root      2439     2  0 13:47 ?        00:00:00 [ll_ost_seq00_00]
root      2440     2  0 13:47 ?        00:00:00 [ll_ost_seq00_00]
root      2441     2  0 13:47 ?        00:00:00 [ll_ost_out00_00]
root      2442     2  0 13:47 ?        00:00:00 [ll_ost_out00_00]
root      2473  2111  0 13:49 pts/0    00:00:00 grep --color=auto ost
```

### chapter 3.script

- start mgs/mdt

```
modprobe  zfs
modprobe  lustre
modprobe  lnet
//execute on mgs/mdt node
mount.lustre /dev/sdc  /mdt
mount.lustre  /dev/sdb /mgt/
```
-  start ost

```
modprobe  zfs
modprobe  lustre
modprobe  lnet
mount.lustre  /dev/sdb /ost/
```
-  mount 

```
mount -t lustre 10.211.55.3@tcp0:/lustrefs /mnt/lustrefs
```
