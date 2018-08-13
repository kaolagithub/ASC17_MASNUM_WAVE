# ASC
I have taken part in ASC  (ASC Student Supercomputer Challenge ), and these are what I done for the competition.

## ASC17
masnum_wave: run on the Sunway  Supercomputer (接近400倍加速)<br/>
masnum_wave(ori): the official original codes

### 主要工作：
 1. 减少线性插值函数progaget中的重复坐标计算，多次用到的坐标提前计算一次存好
 2. 删除多余内存拷贝和数值检查（主要对e和ee数组）
 3. 将Fortran源代码改为Fortran + C 混编。在神威的主从核结构上，计算能力基本由从核提供，从核需要主核调用执行，但是创建线程组需要数万个时钟周期，我呢吧只使用一次athread_spawn创建从核线程组，在程序最后用athread_join来等待从核线程组运行结束
 4. 计算向量化、逻辑判断指令优化（减少分支语句和使用SW26010的内置逻辑判断指令），循环展开
 5. DMA压缩、DMA异步
 6. 负载均衡优化（从贪心——>dp）
 7. 通信优化，能用非阻塞通信的用非阻塞通信，然后通过计算解耦尽量覆盖掉这些通信时间
