## MyRPC
### summary

#### 同步设计 todo
1、Connection

每个client对应一个connection的socket，其socket上的读写生成一个任务，放到任务队列，

**重点**

优先级队列：读、写、调用都是任务，调用的优先级最高

轮询的方式

2、thread_pool

构建线程池，取任务队列的任务进行处理，处理结束收回线程


#### 异步设计

1、一个线程

server:
async_accept -> async_read -> call function -> return -> async_write

2、多线程 todo

一个线程一个iocontext
