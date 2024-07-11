# 版本迭代

+ + 

# 设计需求

​	遥控器需要控制2个舵机，2个推杆，1个喷水开关，1条传送带， 2个电机。另外两个开关分别用于遥控使能（或者备用）、自动切换开关。

+ 2个舵机使用1个输入捕获
+ 2个推杆使用1个输入捕获
+ 2个电机使用2个输入捕获
+ 1个喷水开关，1个传送带直接选择开关。

输出方面，

+ 2个舵机使用2个pwm输出
+ 2个推杆使用2个pwm输出
+ 2个电机用2个pwm输出
+ 1个喷水开关使用1个pwm输出
+ 1个传送带使用1个pwm输出

# 外设选择

​	使用TIM2的4个通道`PA0,1,2,3`作为输入捕获。TIM4的4个通道`PD12,13,14,15`作为舵机、电机输出比较。

# STM32F4外设可用性

## TIM（F427与F407相同）

![image-20231010114705783](C:/study/Mycode/MyProject/MyProject_STM32F4/A%E9%A1%B9%E7%9B%AE%EF%BC%9A%E6%97%A0%E4%BA%BA%E8%89%87/A%E9%A1%B9%E7%9B%AE%EF%BC%9A%E6%97%A0%E4%BA%BA%E8%89%87-v1.01/readme_image/image-20231010114705783.png)

![image-20231010114713048](https://raw.githubusercontent.com/chenxuanjie/Typora/master/img/202311262032368.png)

![image-20231010114719348](https://raw.githubusercontent.com/chenxuanjie/Typora/master/img/202311262032407.png)