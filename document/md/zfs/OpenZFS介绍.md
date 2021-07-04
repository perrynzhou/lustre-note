## OpenZFS 简介

| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2020/12/01 |672152841 |

### ZFS是什么？

- ZFS是最早sun公司设计用于Solaris系统的文件系统，在OpenSolaris和FreeBSD下的稳定性非常高。相对于XFS文件系统，ZFS虽然是本地文件系统，但是集成了软件Raid(Raid0/Raid1/Raid Z1/Raid Z2/Raid Z3)、基于元数据的COW写任意位置的事务模型，自带Checksum等保证数据完整性，同时提供高效的快照和复制功能，还有支持高速盘的SSD读写缓存、压缩、去重等特性


### ZFS混合池架构概览

![hybrid-pool](../images/hybrid-pool.png)

- ARC是DRAM内存中的Cache,主要用于元数据和数据的读缓存工程
- L2ARC是ARC的二级缓存，ARC的扩展，一般采用高速盘比如NVME/Sata SSD
- ZIL(ZFS Intent Log,写日志是支持掉电保护的SSD写缓存，持久化写缓存，用于加速同步写性能和数据的异常情况下恢复



### OpenZFS 功能分层概览

![layer-view](../images/layer-view.png)

- Zvol(zfs volume),ZFS 提供的块设备方式，暴露一个块设备接口，类似与dataset（文件系统），zvol为block设备，可以被格式化，可以被iSCSI分享。并且Zvol具有ARC的缓存能力，和dedup结合下非常适合宿主机提供虚拟文件系统镜像的存储
- ZPL(ZFS Posix Layer),提供符合POSIX文件系统语义的抽象，包括文件、目录、软连接、socket套接字、inode相关属性的抽象。ZPL是ZFS中最复杂的一个子系统，也是作为ZFS文件系统最核心的部分
- Transactional Object Layer，这个子系统提供一个事务对象的语义层，对对象的修改 不会因为掉电的原因导致事务的不一致



### 源代码架构

- 代码实现功能层级

![source-view](../images/source-view.png)

- 代码实现掉调用关系

![code-arc-view](../images/code-arc.png)

