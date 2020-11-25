#### 1.磁盘及文件统计信息：df/du/tree

```shell
# df命令操作的是磁盘，可以指定当前磁盘，如果指定了当前目录，那么显示的也是本目录所在磁盘的信息
df [option[-i,-h]] # -i查看磁盘节点使用情况， -h以方便阅读的方式显示统计数据，直接使用df 不方便阅读数据
# 以下命令全部默认为当前目录，可以在后面直接加上要统计的目录
du
du -a # 查看全部信息，
du -h # 数据友好可读，
du -s # 只显示文件本身的大小
du -c # 显示统计信息，会比 du 单独使用多一行total的统计数据
du --max-depth=N # 显示的最大递归深度

tree # 显示指定目录的树状图
```

#### 2.查看系统内存和版本信息

```shell
cat /proc/cpuinfo  # 在linux /proc/cpuinfo 文件中有系统的全部信息
top # 可以查看当前cpu的动态信息，输入 1 可以切换查看多核的使用情况
dstat # 可以查看更加丰富的cpu的动态信息

cat /proc/meminfo  # 在linux /proc/meminfo 文件中有系统的内存信息
free -[k/m/g] # 查看内存使用情况，后面的k/m/g是指定显示的数字单位，方便阅读

uname # 显示linux版本
uname -r # 显示详linux版本信息
uname -a # 显示Linux版本信息，内核信息，架构信息
cat /etc/redhat-release # centos系统可用，查看发行版信息
```



