# Learning-ITC
Learning Inter Thread Communication Programming
* * *

# 项目的缘起
此处是为了评估 ITC library 的可用性而建立。
* Inter Process Communication
* Inter Thread Communication

为了在 glibc space 中调用 bionic space 的函数，需要一个 ITC library，以便处理那些烦杂的编程细节。在 Linux 上面，可用于 ITC 的技术有不少。参见文档[1]。常用的有：
1. unnamed pipe: pipe() pipe2()
2. named pipe: mkfifo()
3. unix domain socket
4. TCP or UDP / IP

可用的代码库也不少：
* [libevent](http://libevent.org/)
* [libuv](http://libuv.org/)
* [nanomsg / nng](https://nanomsg.org/index.html)

评估的目标是代码快一点，小一点，用起来方便一点。

# 评估结果
评估下来让我看到了 Android 控制欲的另一面。

ITC 的常用技术在 Android 平台上的测试结果：
1. unnamed pipe: OK
2. named pipe: Only root user cans create fifo file.
3. unix domain socket: Only root user cans create filesystem socket.
   * 在 Android 中可以使用 unix domain socket。参见项目[3]与[4]。
   * 在 Android 中的 socket address 使用的是 Linux abstract namespace。使用 abstract namespace 能使文件系统干净一些，不会留下垃圾文件。但是安全性要差一些，需要自己检查资格证书(credentials)[5]。
   * 使用 Linux abstract namespace socket 有一个技巧，参见文档[2]。
4. TCP or UDP / IP: The root and the user belong to "inet(3003)" group can use inet.

为了快一点，只能使用 unnamed pipe 技术。还好 unnamed pipe 从设计之初就是人人可用，自家内部自己玩，倒是很符合我的要求。Android 对此没有设置权限障碍，否则 Google 就成了众矢之的了。但对于 unnamed pipe 技术，各个 IPC library 都没什么兴趣做高级一点的封装。只有自己来了。想偷懒一点都不成:-(。
* [libevent](http://libevent.org/) : 一切以 File Descriptor 为中心进行封装。看起来简单一点，就它了。
* [libuv](http://libuv.org/) : 一切以 Asynchronous Event 为中心进行封装。File Descriptor 只是异步事件来源的一种。还有其它很多异步事件来源的处理。稍微复杂了一些，多了些我不要的处理代码。放弃了。
* [nanomsg / nng](https://nanomsg.org/index.html) : "inproc://" 协议接口可用。"ipc://"协议接口不能使用，估计是没有使用 abstract namespace socket。"tcp://" 协议接口可以在 Android 环境中使用，前提是用户是 root 或属于 "inet(3003)" group。nanomsg 封装的概念真的很好，测试用的 demo 也最简单，但似乎是第一次调用有点慢。可惜我不知 "inproc://" 协议下面封装的是什么技术。想想以后要在 glibc space 与 bionic space 之间来回调用，都是非标准的技术，要是联不通，都不知道怎么调试。放弃了。

# 参考文件
1. [Inter-process communication](https://en.wikipedia.org/wiki/Inter-process_communication)
2. [Android AHardwareBuffer Shared Memory over Unix Domain Sockets](https://medium.com/@spencerfricke/android-ahardwarebuffer-shared-memory-over-unix-domain-sockets-7b27b1271b36)
3. [Shared Memory IPC](https://github.com/sjfricke/NDK-Socket-IPC)
4. [android-unix-socket-client-server](https://github.com/frogoscar/android-unix-socket-client-server)
5. [network/unixdomain](https://github.com/troydhanson/network/tree/master/unixdomain)


