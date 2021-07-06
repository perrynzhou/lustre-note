

### moosefs 源码编译


##### moosefs源码编译

```shell
wget https://github.com/moosefs/moosefs/archive/refs/tags/v3.0.115.tar.gz
tar zxvf v3.0.115.tar.gz && cd moosefs-3.0.115
./configure --enable-debug && make -j8 && make install
```
##### mfs master

- 初始化配置
```shell
cd ~/moosefs-3.0.115/mfsdata/ && cp mfsmaster.cfg /usr/local/etc
cd ~/moosefs-3.0.115/mfsdata/ && cp mfsexports.cfg /usr/local/etc

// 修改 mfsexports.cfg 配置可以挂在该存储的权限
192.168.0.2.0/24   /   rw,alldirs,maproot=0


// 修改mfsmaster.cfg中的host和port
MASTER_HOST = 172.29.76.218
MASTER_PORT = 9419
```
- 启动
```
/usr/local/bin/mfsmaster -a start
```

##### mfs metalogger
- 初始化配置
```shell
cd ~/moosefs-3.0.115/mfsdata/ && cp mfsmetalogger.cfg /usr/local/etc
// 修改mfsmaster.cfg中的host和port
MASTER_HOST = 172.29.76.218
MASTER_PORT = 9419
```
- 启动
```
/usr/local/bin/mfsmetalogger start
```

##### mfs chunkserver

- 初始化配置
```
cd ~/moosefs-3.0.115/mfsdata/ && cp mfshdd.cfg usr/local/etc/mfshdd.cfg
cd ~/moosefs-3.0.115/mfsdata/ && cp mfschunkserver.cfg /usr/local/etc/mfschunkserver.cfg


// 在mfshdd中添加数据挂载盘作为chuckserver的数据盘
echo "/data/mfs-data1" >>/usr/local/etc/mfshdd.cfg
echo "/data/mfs-data2" >>/usr/local/etc/mfshdd.cfg


// 修改mfschunkserver.cfg中master的节点IP和端口
MASTER_HOST = 172.29.76.218
MASTER_PORT = 9420
```
- 启动
```
./mfschunkserver start
```

##### mfs 客户端挂在

```
./mfsmount  /mnt/mfs/ -H 172.29.76.218
```
