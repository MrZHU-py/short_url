# short_url

#### 介绍
基于Redis实现的短地址生成服务，以时间为随机数种子，每次生成随机短地址。

#### 软件架构
include文件夹包括所有头文件
src文件夹包括所有源文件
CMakeLists.txt

#### 使用
将仓库克隆到本地后进入build目录，将原有的文件删掉。
然后运行以下指令：

```shell
cmake ..
make
```
最后运行可执行文件即可

