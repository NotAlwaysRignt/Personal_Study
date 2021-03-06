参考资料:
[Linux usb子系统(一) _写一个usb鼠标驱动](https://www.cnblogs.com/xiaojiang1025/p/6500574.html)
[厂商自定义USB设备类概述](https://www.bbsmax.com/A/B0zqvGxQJv/)
### USB 驱动编程目标
首先我们要明白作为开发者,我们要在 USB 设备驱动上做什么工作,操作系统已经为我们做了什么,硬件厂家又做了什么
USB 是一种传输机制,它有一套规范,遵循这套规范的设备,就可以和我们的主机通信
当前 USB 设备非常普遍,包括常见的鼠标,键盘等等.USB协会将常用具有相同/相似功能的设备归为一类，并制定了相关的设备类规范.这样就能保障只要依照同样的规范标准，即使不同的厂商开发的USB设备也可以使用同样的驱动程序，而且操作系统中无须为每种设备提供独立的设备驱动程序，也就是说设备端的开发中不需要花费开发软件驱动的精力。这些标准的设备类包括：音频设备、人机接口设备（HID）、存储设备、打印机设备等。
通过上面可以知道,通过 USB 规范,我们到市面上购买不同厂家生产的设备,比如鼠标,键盘时,基本上即插即用,不需要为它们去下载专门的驱动了.(鼠标,键盘等设备有通用的`usb_device_id`,由USB规范制定)

由上面可知,对于一些设备, USB 驱动甚至可以不再需要开发者开发, 它们早已经被做到内核里面去了.但是这些标准的设备规范并不能完全涵盖所有的电子设备，市场上也有很多非标准类型的USB设备，这时就要求厂商提供单独的客户端驱动程序，这种设备就厂商自定义设备。

厂商自定义设备的开发除了要完成设备硬件以及固件程序开发外，还需要开发操作系统端的设备驱动程序(内核态编程)，甚至还要开发设备应用程序(在应用层开发)。

**设备硬件的固件是厂家完成的,开发者的任务主要就是在主机端开发USB设备驱动程序,而这种开发也是有规范的,我们要在 Linux提供的USB设备模型下进行**

首先要理解Linux内核做了哪些工作,它定义了哪些概念,我们开发者又在哪一层进行开发
device->config->interface->endpoint
USB设备驱动是面向interface这一层次的信息的
USB设备不会主动向主机传递信息,USB设备连接主机时,主机会轮询到 USB 接口的使用,并不断地发送请求去获取这个设备的相关信息,设备收到主机的请求后会作应答,把信息告诉主机,这个过程叫做USB枚举(枚举是主机做的动作),设备通信遵循USB协议
枚举获取信息的过程USB设备驱动开发者不需要去实现,我们在开发时可以直接通过API去获取信息
