rem : ʹ��ע�⣺
rem :    1�����ݰ�װ·���޸�SDKDIR��ʵ��λ��
rem :    2��usb��112 ����ʵ����USB�˿ں��޸�,�����ڿ����������豸�������²鿴
rem :    3��LEF�ļ���������д�Ĺ̼���ͬһ�����ɵ�XUV��Ӧ��ELF�ļ�
rem :    4��ÿ���ύ��д�����ύ Flash_image.xuv  Earbud.elf �Լ������ļ� QCC512X_dfu_xxx
rem ��˫�����BAT����֮������ʾ�������룺 apps1.live_log()
rem : 

set SDKDIR="D:\QCC512X\ADK_QCC512X_ROM_V21_WIN_6.4.0.39"
%SDKDIR%\tools\python27\python.exe %SDKDIR%\apps\fw\tools\pydbg.py -d usb:112 -f apps1:firmware\earbud.elf

pause


rem f:/qtil/ADK_QCC512X_QCC302X_WIN_6.4.0.43/tools/python27/python.exe F:\qtil\ADK_QCC512X_QCC302X_WIN_6.4.0.43\apps\fw\tools\pydbg.py -d usb:103 -f apps1:C:\Users\Admin\Desktop\case\sink.elf