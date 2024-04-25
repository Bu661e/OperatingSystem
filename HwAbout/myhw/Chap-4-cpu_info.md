> **1．用以下标志运行程序：./process-run.py -l 5:100,5:100。CPU 利用率（CPU 使用时间 的百分比）应该是多少？为什么你知道这一点？利用 -c 标记查看你的答案是否正确。**

在创建进程时，`5:100`表示该进程有5个指令，且每个指令使用CPU的概率为100%。这里创建的两个进程都是如此。所以CPU利用率应该是100%

运行指令结果如下：

```bash
dinghaitong@dinghaitong-VirtualBox:~/os_ws/ostep-homework/cpu-4-intro$ ./process-run.py -l 5:100,5:100
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu
  cpu

Process 1
  cpu
  cpu
  cpu
  cpu
  cpu

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)

```

加上 `-c` 和 `-p`后进行验证，运行结果如下：

```bash
dinghaitong@dinghaitong-VirtualBox:~/os_ws/ostep-homework/cpu-4-intro$ ./process-run.py -l 5:100,5:100 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1  
  2        RUN:cpu         READY             1  
  3        RUN:cpu         READY             1  
  4        RUN:cpu         READY             1  
  5        RUN:cpu         READY             1  
  6           DONE       RUN:cpu             1  
  7           DONE       RUN:cpu             1  
  8           DONE       RUN:cpu             1  
  9           DONE       RUN:cpu             1  
 10           DONE       RUN:cpu             1  

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```

可以看到：10个时间周期中，CPU一直在运行，所以CPU利用率为100% 。说明答案正确。

---

> **2．现在用这些标志运行：./process-run.py -l 4:100,1:0。这些标志指定了一个包含 4 条指令的进程（都要使用 CPU），并且只是简单地发出 I/O 并等待它完成。完成这两个进程需要 多长时间？利用-c 检查你的答案是否正确。**

一共创建了2个进程。进程 0 包括 4 个指令，且每个指令使用CPU概率为100%；进程 1 包括 1 个指令，且每个指令使用CPU概率为0，意味着每个指令都会是IO指令。

对于IO指令，在README.md 文档中有 2 点注意事项：

* 进程执行 1 次IO指令分为 3 个阶段 `RUN:io `, `BLOCKED `, `RUN: io_end` 。其中前后两阶段要占用CPU各 1 个时钟周期，只用中间的阶段才会占用IOs 。
* `BLOCKED` 阶段的持续时间由参数 `-L IO_LENGTH` 决定，但本题并没有给出此参数。

下面给出我的猜测：进程0会的第一个指令时CPU指令，进程1的第一个指令第一时间也需要占用CPU，但是进程0的CPU使用概率为100%。所以一开始，进程0为 `RUNNING`，进程1为 `READY`，持续 4 个周期。进程0运行完所有指令，进入 `DONE` ，进程1进入 `RUN：io` ，持续 1 个周期 。进程0继续保持 `DONE` ，进程 1 进入 `BLOCK` ,此时没有进程需要CPU，由于没有给出持续时间，这里假设为 5 个周期 。最后进程 1 调用CPU进入 `RUN:io_done` ，持续 1 个周期 。共计 4 + 1 + 5 + 1 = 11个时钟周期。

```bash
dinghaitong@dinghaitong-VirtualBox:~/os_ws/ostep-homework/cpu-4-intro$ ./process-run.py -l 4:100,1:0
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu

Process 1
  io
  io_done

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```

加上 `-c` 和 `-p`后进行验证，运行结果如下：

```bash
dinghaitong@dinghaitong-VirtualBox:~/os_ws/ostep-homework/cpu-4-intro$ ./process-run.py -l 4:100,1:0 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5           DONE        RUN:io             1
  6           DONE       BLOCKED                           1
  7           DONE       BLOCKED                           1
  8           DONE       BLOCKED                           1
  9           DONE       BLOCKED                           1
 10           DONE       BLOCKED                           1
 11*          DONE   RUN:io_done             1

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```
