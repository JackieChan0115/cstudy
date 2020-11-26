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
top # 可以查看当前cpu的动态信息，
		# 输入 1 可以切换查看多核的使用情况,
    # 输入shift+m可以按占用内存从大到小排序，
    # 输入shift+p可以按占用cpu从大到小排序
    
dstat # 可以查看更加丰富的cpu的动态信息

cat /proc/meminfo  # 在linux /proc/meminfo 文件中有系统的内存信息
free -[k/m/g] # 查看内存使用情况，后面的k/m/g是指定显示的数字单位，方便阅读

uname # 显示linux版本
uname -r # 显示详linux版本信息
uname -a # 显示Linux版本信息，内核信息，架构信息
cat /etc/redhat-release # centos系统可用，查看发行版信息
```

#### 3.查看Linux网络链接信息

```shell
netstat -anp # a 显示所有选项，n 拒绝显示别名，把一些别名转化成数字，p 显示相关程序名
top # 详细在标题2里面
ifconfig # 可以查看网卡信息和收发包的信息
```

#### 4.文件夹权限信息

```shell
ls -l # 查看当前目录下所有文件和目录的信息, 输出如下
----------------------------------------------------------
-rwxrwxr-x 1 root root  45776 Nov 22 08:43 book
drwxrwxr-x 1 root root  45776 Nov 22 08:43 bookdir
----------------------------------------------------------
其中 -rwxrwxr-x 共10个字符，第1个字符: 横杠 - 代表是文件；字母d 代表是目录;
	       后9个字符rwxrwxr-x，是权限信息，解释如下：
```



![image-20201126131521808](/Users/sytm/Library/Application Support/typora-user-images/image-20201126131521808.png)


```shell
umask # 查看默认文件配置权限的掩码，是真实权限信息取反转化成8进制
umask -S # 查看默认的文件权限信息，是每次创建文件时的真实权限信息

chmod #change mode , 更改文件的权限信息
chmod u+w [filename] # 为文件的所属用户添加可写权限
chmod g-r [filename] # 为文件的所属用户去掉可读权限

----------------------------------------------------------
使用方法：
chmod [augo] +/- [rwx] [filename]
chmod [0-7][0-7][0-7] [filename]
----------------------------------------------------------

chown # 改变文件和目录的所属用户，change owner
chown [new-username]  [filename]

chgrp # 改变文件和目录的所属组，change group
chgrp [new-username]  [filename]
```

#### 5.Linux 用户切换

```shell
su root # 从普通用户切换到root用户
sudo userame # 让username临时提权，使用root用户的权限
```

#### 6.grep命令

```shell
grep [pattern|string] [filename] # 从文件中搜索字符串或者某种模式，大小写敏感
grep -[i,n] [pattern|string] [filename] # -i选项可以让大小写不敏感,-n选项可以显示搜索到字符串的行号
grep -F [搜索的字符串可以包含正则表达式的符号] [filename]
grep -E "linux|Linux" test.txt # -E选项让扩展正则表达式生效，在test.txt文件中搜索linux和Linux
grep -[c,o] # -c 是统计搜索到的数量， -o是只显示搜索到的字符串，不显示相关行的完整信息
```

#### 7.find命令

```shell
find [path] -name filename  		# 在路径path下面搜索 name
find [path] -iname filename 		# 添加了i字符，表示忽略大小写
find [path] -empty          		# 查找大小为0即为空的文件
find [path] -size  8k						# 查找大小为8k的文件
find [path] -size  +2k      		# 查找文件大小大于2k的文件
find [path] -size  -6k.         # 查找文件大小小于6k的文件
find [path] -size +6k -size -8k	#查找文件大小在6k和8k之间的文件
```

#### 8.uniq命令

````shell
uniq [filename] # 可以将文件中的所有行在输出的时候，去掉重复行
uniq [sorucefilename] [dstfilename] # 可以将源文件中的所有行去重后放到目标文件中
uniq -c [filename] # 可以将文件中重复行统计出来，输出的时候会显示该行有多少重复行
uniq -u [filename] # 将文件中唯一的行输出来
````

#### 9.shopt指令

```shell
# 用于显示和设置shell中的行为选项，通过这些选项以增强shell易用性。shopt命令若不带任何参数选项，则可以显示所有可以设置的shell操作选项。
shopt [选项][参数]
-------------------------------------------------
-s：激活指定的shell行为选项；
-u：关闭指定的shell行为选项。
-------------------------------------------------
shopt -s cdspell # 开启cd拼写检查
shopt -u cdspell # 关闭cd拼写检查
```

