## 操作系统
操作系统课程学习中的一些练习代码，主要关于死锁和进程通信。
### 死锁
* `Banker.cpp`, `Deadlock Detect.cpp`：银行家算法实现循环检测资源请求、死锁检测。
* `Readers-Writers 1.cpp`, `Readers-Writers 2.cpp`：读者写者问题读者优先、写者优先实现。
* `Semaphore 1.cpp`, `Semaphore 2.cpp`：狒狒过谷问题不考虑饥饿问题、考虑饥饿问题实现。
### 进程通信
* `Pipe Test.cpp`：Linux 进程管道通信实现。
* `Shared Memory Test.cpp`：Linux 进程共享内存通信实现，并使用 SYSTEM V 信号量实现共享内存区的互斥访问。
* `Signal Test.cpp`：Linux 进程信号通信实现。

## Operating Systems
Practice codes about deadlock and pross communication.
### DeadLock
* `Banker.cpp`, `DeadLock Detect.cpp`: Use Banker's algorithm to detect deadlock and determines if the state is safe with a request.
* `Readers-Writers 1.cpp`, `Readers-Writers 2.cpp`: Readers-preference and writers-preference readers-writer problem implmentations.
* `Semaphore 1.cpp`, `Semaphore 2.cpp`: Use semaphore to prevent deadlock.
### Process Communication
* `Pipe Test.cpp`: Process communication practice.
* `Shared Memory Test.cpp`: Process communication practice used shared memory and SYSTEM V semophore.
* `Signal Test.cpp`: Process communication practice used signal.
