











一命令列表说明 （pydbg)，可以同时执行两条命令（中间加分号）
    1、系统reboot
apps1.fw.call.appSmReboot()
    2、系统参数复位
apps1.fw.call.appSmFactoryReset()
    3、蓝牙地址配对 ---- 显示
apps1.fw.call.BtAddrChange(0)
    4、蓝牙地址配对 ---- 清除
apps1.fw.call.BtAddrChange(1)
    5、蓝牙地址配对 ---- 左耳机调用写右地址
apps1.fw.call.BtAddrChange(2)
    5、蓝牙地址配对 ---- 右耳机调用写左地址
apps1.fw.call.BtAddrChange(3)
    6、蓝牙地址配对 ---- 耳机单独使用
apps1.fw.call.BtAddrChange(4);apps1.fw.call.appSmReboot()






