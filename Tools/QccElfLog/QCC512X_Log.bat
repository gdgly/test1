rem : 使用注意：
rem :    1、根据安装路径修改SDKDIR的实际位置
rem :    2、usb：112 根据实际在USB端口号修改,可以在开发环境或设备管理器下查看
rem :    3、LEF文件必须与烧写的固件是同一次生成的XUV对应的ELF文件
rem :    4、每次提交烧写，请提交 Flash_image.xuv  Earbud.elf 以及升级文件 QCC512X_dfu_xxx
rem ：双击这个BAT运行之后，在提示符下输入： apps1.live_log()
rem : 

set SDKDIR="D:\QCC512X\ADK_QCC512X_ROM_V21_WIN_6.4.0.39"
%SDKDIR%\tools\python27\python.exe %SDKDIR%\apps\fw\tools\pydbg.py -d usb:112 -f apps1:firmware\earbud.elf

pause


rem f:/qtil/ADK_QCC512X_QCC302X_WIN_6.4.0.43/tools/python27/python.exe F:\qtil\ADK_QCC512X_QCC302X_WIN_6.4.0.43\apps\fw\tools\pydbg.py -d usb:103 -f apps1:C:\Users\Admin\Desktop\case\sink.elf