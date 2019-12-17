
20191212
充电电流控制
 1、0度以下           10mA
 2、0-15度            20mA
 3、15-45度           60mA
 4、45度以上          20mA

20191211
PowerSave模式说明
1、几种状态：盒子中 盒盖打开 空中 耳中
2、可开关设备：接近光 APO2 TAP LIS25(当前无） PLC总线（不能/不需要关闭）
3、状态变化时需要一定的延时;
4、PLC通信的过程中，不能进入DeepSleep模式，否则通信不正常
5、APO打开时，当前需要BIAS电压，当前BIAS压电需要DSP打开
6、敲击传感器需要在耳朵中才能打开
                  接近光    APO2        TAP     LIS25(当前无）    DSP&BT
     盒子中        OFF       OFF         OFF
     盒盖打开      OFF       OFF         OFF
     空中          ON        OFF         OFF
     耳朵中        ON        ON/SET      ON

20191210
  SBC压缩，有一个使用sbc_encoder的程序可以指定码流、
  实际测试默认代码码流为128KBPS， 而我们可以设置为64/32都能使用。16K也可以压纹，但在QCC5126上
会出现播放成功（正确），但无返回的状态（不能获取播放结束），无法播放下一个提示音了.
  我们当前选用64K先测试着。


20191121
单MIC及双MIC的使用
uint8 g_appConfigSocMic1 = 0, g_appConfigSocMic2 = NO_MIC;      // 设置为 NO_MIC，就是不使用这个MIC（使用单MIC）
1、经典蓝牙正常通话时， 使用双MIC                                       g_appConfigSocMic1 = 0,      g_appConfigSocMic2 = 1
2、BLE录音助手时，      使用单MIC, 选择主MIC                            g_appConfigSocMic1 = 0,      g_appConfigSocMic2 = NO_MIC
3、经典蓝牙生产测试时， 使用单MIC，选择主MIC                            g_appConfigSocMic1 = 0,      g_appConfigSocMic2 = NO_MIC
                        使用单MIC，选择副MIC                            g_appConfigSocMic1 = NO_MIC, g_appConfigSocMic2 = 1
4、USB传输生产测试时，  使用单MIC，选择主MIC                            g_appConfigSocMic1 = 0,      g_appConfigSocMic2 = NO_MIC
                        使用单MIC，选择副MIC                            g_appConfigSocMic1 = NO_MIC, g_appConfigSocMic2 = 1

20191104
  ConnectionEnterDutMode();



20191015
  升级命令格式，主要就是命令GAIA_COMMAND_VM_UPGRADE_CONTROL(0x642) + 字命令（1byte UPGRADE_HOST_XXX）
  UPGRADE_HOST_SYNC(state=1) + 长度(2Byte) + progID（4Byte)
  -->000a0642    13 0004 a0a51c09
  <--000a4003 12 14 0006 00 a0a51c09 03        进入UPGRADE_STATE_READY
  UPGRADE_HOST_START_REQ(state=2)
  -->000a0642    01 0000
  <--000a4003 12 020003 000666                 进入UPGRADE_STATE_DATA_READY
  UPGRADE_HOST_START_DATA_REQ(state=6)
  -->000a0642    15 0000
    0.注意接收到这条命令，会准备并擦除分区，是需要时间的，分区不正确会导致系统重启(PANIC 0xb6 4108 (PANIC_P0_PANICKED)。
      擦除成功会获取底层 MESSAGE_IMAGE_UPGRADE_ERASE_STATUS 消息。
      UpgradePartitionDataInit-->UpgradePartitionDataRequestData 设置了需要块数据的大小
    1.UpgradeSendStartUpgradeDataInd 会向上层升级处发送UPGRADE_START_DATA_IND appUpgradeMessageHandler
    2.系统会接收到底层发送过来的 Flash擦除完成消息 MESSAGE_IMAGE_UPGRADE_ERASE_STATUS appUpgradeMessageHandler,
    3.进入UpgradeSMEraseStatus, 设置需要接收的数据位置及大小
  <--            03 0008 size offset           进入UPGRADE_STATE_DATA_TRANSFER
  UPGRADE_HOST_DATA
  -->000a0642    04                            写数据，再次


一、命令列表说明 （pydbg)，可以同时执行两条命令（中间加分号）
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


二、串口命令说明
    1、为了方便调试，增加串口命令，将并串口波特率设置为115200
    2、


/////////////////////////////////////////////////////////////////////////////////////
//     scofwd_nb.chain  修改后的内容
/////////////////////////////////////////////////////////////////////////////////////
<?xml version="1.0" encoding="UTF-8"?>
<!--Copyright (c) 2018 Qualcomm Technologies International, Ltd.-->
<!--Chain for HFP (narrowband SCO), used for forwarding SCO to other earbud -->

<chain name="CHAIN_SCOFWD_NB" id="1" generate_operator_roles_enum="False" generate_endpoint_roles_enum="False">

    <include_header name="../av_headset_chain_roles.h"/>

    <operator name="OPR_SCO_RECEIVE" id="CAP_ID_SCO_RCV" priority="medium">
        <sink name="SCO_IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_SCO_SEND" id="CAP_ID_SCO_SEND" priority="high">
        <sink name="IN" terminal="0"/>
        <source name="SCO_OUT" terminal="0"/>
    </operator>

    <operator name="OPR_CVC_RECEIVE" id="CAP_ID_CVC_RECEIVE_NB">
        <sink name="IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_SCOFWD_SPLITTER" id="CAP_ID_SPLITTER">
        <sink name="IN" terminal="0"/>
        <source name="OUT1" terminal="0"/>
        <source name="OUT2" terminal="1"/>
    </operator>

    <operator name="OPR_SWITCHED_PASSTHROUGH_CONSUMER" id="EB_CAP_ID_SWITCHED_PASSTHROUGH">
        <sink name="IN" terminal="0"/>
        <source name="FORWARD" terminal="0"/>
    </operator>

    <operator name="OPR_SCOFWD_SEND" id="EB_CAP_ID_ASYNC_WBS_ENC">
        <sink name="IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_CVC_SEND" id="CAP_ID_CVCHS1MIC_SEND_NB" >
        <sink name="REFERENCE" terminal="0"/>
        <sink name="IN1" terminal="1"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_SCO_UP_SAMPLE" id="CAP_ID_IIR_RESAMPLER">
        <sink name="IN8K" terminal="0"/>
        <source name="OUT16K" terminal="0"/>
    </operator>

    <operator name="OPR_SOURCE_SYNC" id="CAP_ID_SOURCE_SYNC">
        <sink name="IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_VOLUME_CONTROL" id="CAP_ID_VOL_CTRL_VOL">
        <sink name="MAIN_IN" terminal="0"/>
        <sink name="AUX_IN" terminal="1"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_SCO_AEC" id="EB_CAP_ID_AEC_REF">
        <sink name="INPUT1" terminal="0"/>
        <sink name="MIC1" terminal="2"/>
        <source name="REFERENCE" terminal="0"/>
        <source name="SPEAKER1" terminal="1"/>
        <source name="OUTPUT1" terminal="3"/>
    </operator>

    <operator name="OPR_CUSTOM_SCO_SPLITER" id="CAP_ID_SPLITTER">
        <sink name="IN" terminal="0"/>
        <source name="OUT_AUDIO" terminal="0"/>
        <source name="OUT_APP" terminal="1"/>
    </operator>

    <operator name="OPR_CUSTOM_MIC_SPLITER" id="CAP_ID_SPLITTER">
        <sink name="IN" terminal="0"/>
        <source name="OUT_AUDIO" terminal="0"/>
        <source name="OUT_APP" terminal="1"/>
    </operator>

    <operator name="OPR_CUSTOM_SCO_PASSTHROUGH" id="CAP_ID_DOWNLOAD_G722CODEC">
        <sink name="IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_CUSTOM_MIC_PASSTHROUGH" id="CAP_ID_DOWNLOAD_G722CODEC">
        <sink name="IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>


    <input sink="OPR_SCO_RECEIVE.SCO_IN" role="EPR_SCO_FROM_AIR"/>
    <input sink="OPR_SCO_AEC.MIC1" role="EPR_SCO_MIC1"/>
    <input sink="OPR_VOLUME_CONTROL.AUX_IN" role="EPR_VOLUME_AUX"/>
    <output source="OPR_SCO_AEC.SPEAKER1" role="EPR_SCO_SPEAKER"/>
    <output source="OPR_SCO_SEND.SCO_OUT" role="EPR_SCO_TO_AIR"/>
    <output source="OPR_SWITCHED_PASSTHROUGH_CONSUMER.FORWARD" role="EPR_SCOFWD_TX_OTA"/>
    <output source="OPR_CUSTOM_SCO_PASSTHROUGH.OUT" role="EPR_AUDIO_SCOFWD_OUT"/>
    <output source="OPR_CUSTOM_MIC_PASSTHROUGH.OUT" role="EPR_AUDIO_MICFWD_OUT"/>

    <connection source="OPR_SCO_RECEIVE.OUT" sink="OPR_CVC_RECEIVE.IN"/>
    <connection source="OPR_CVC_RECEIVE.OUT" sink="OPR_SOURCE_SYNC.IN"/>
    <connection source="OPR_SOURCE_SYNC.OUT" sink="OPR_SCOFWD_SPLITTER.IN"/>
    <connection source="OPR_VOLUME_CONTROL.OUT"  sink="OPR_SCO_AEC.INPUT1"/>
    <!--connection source="OPR_SCOFWD_SPLITTER.OUT1"  sink="OPR_VOLUME_CONTROL.MAIN_IN"/-->
    <connection source="OPR_SCOFWD_SPLITTER.OUT1"  sink="OPR_CUSTOM_SCO_SPLITER.IN"/>
    <connection source="OPR_CUSTOM_SCO_SPLITER.OUT_AUDIO"  sink="OPR_VOLUME_CONTROL.MAIN_IN"/>
    <connection source="OPR_CUSTOM_SCO_SPLITER.OUT_APP"  sink="OPR_CUSTOM_SCO_PASSTHROUGH.IN"/>

    <connection source="OPR_SCOFWD_SPLITTER.OUT2"  sink="OPR_SCO_UP_SAMPLE.IN8K"/>
    <connection source="OPR_SCO_UP_SAMPLE.OUT16K"  sink="OPR_SCOFWD_SEND.IN"/>
    <connection source="OPR_SCOFWD_SEND.OUT"  sink="OPR_SWITCHED_PASSTHROUGH_CONSUMER.IN"/>
    <connection source="OPR_SCO_AEC.OUTPUT1" sink="OPR_CVC_SEND.IN1"/>
    <connection source="OPR_CVC_SEND.OUT" sink="OPR_CUSTOM_MIC_SPLITER.IN"/>
    <connection source="OPR_CUSTOM_MIC_SPLITER.OUT_AUDIO"  sink="OPR_SCO_SEND.IN"/>
    <connection source="OPR_CUSTOM_MIC_SPLITER.OUT_APP"  sink="OPR_CUSTOM_MIC_PASSTHROUGH.IN"/>
    <connection source="OPR_SCO_AEC.REFERENCE" sink="OPR_CVC_SEND.REFERENCE"/>

</chain>

/////////////////////////////////////////////////////////////////////////////////////
//     scofwd_nb.chain  原始
/////////////////////////////////////////////////////////////////////////////////////
<?xml version="1.0" encoding="UTF-8"?>
<!--Copyright (c) 2018 Qualcomm Technologies International, Ltd.-->
<!--Chain for HFP (narrowband SCO), used for forwarding SCO to other earbud -->

<chain name="CHAIN_SCOFWD_NB" id="1" generate_operator_roles_enum="False" generate_endpoint_roles_enum="False">

    <include_header name="../av_headset_chain_roles.h"/>

    <operator name="OPR_SCO_RECEIVE" id="CAP_ID_SCO_RCV" priority="medium">
        <sink name="SCO_IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_SCO_SEND" id="CAP_ID_SCO_SEND" priority="high">
        <sink name="IN" terminal="0"/>
        <source name="SCO_OUT" terminal="0"/>
    </operator>

    <operator name="OPR_CVC_RECEIVE" id="CAP_ID_CVC_RECEIVE_NB">
        <sink name="IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_SCOFWD_SPLITTER" id="CAP_ID_SPLITTER">
        <sink name="IN" terminal="0"/>
        <source name="OUT1" terminal="0"/>
        <source name="OUT2" terminal="1"/>
    </operator>

    <operator name="OPR_SWITCHED_PASSTHROUGH_CONSUMER" id="EB_CAP_ID_SWITCHED_PASSTHROUGH">
        <sink name="IN" terminal="0"/>
        <source name="FORWARD" terminal="0"/>
    </operator>

    <operator name="OPR_SCOFWD_SEND" id="EB_CAP_ID_ASYNC_WBS_ENC">
        <sink name="IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_CVC_SEND" id="CAP_ID_CVCHS1MIC_SEND_NB" >
        <sink name="REFERENCE" terminal="0"/>
        <sink name="IN1" terminal="1"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_SCO_UP_SAMPLE" id="CAP_ID_IIR_RESAMPLER">
        <sink name="IN8K" terminal="0"/>
        <source name="OUT16K" terminal="0"/>
    </operator>

    <operator name="OPR_SOURCE_SYNC" id="CAP_ID_SOURCE_SYNC">
        <sink name="IN" terminal="0"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_VOLUME_CONTROL" id="CAP_ID_VOL_CTRL_VOL">
        <sink name="MAIN_IN" terminal="0"/>
        <sink name="AUX_IN" terminal="1"/>
        <source name="OUT" terminal="0"/>
    </operator>

    <operator name="OPR_SCO_AEC" id="EB_CAP_ID_AEC_REF">
        <sink name="INPUT1" terminal="0"/>
        <sink name="MIC1" terminal="2"/>
        <source name="REFERENCE" terminal="0"/>
        <source name="SPEAKER1" terminal="1"/>
        <source name="OUTPUT1" terminal="3"/>
    </operator>

    <input sink="OPR_SCO_RECEIVE.SCO_IN" role="EPR_SCO_FROM_AIR"/>
    <input sink="OPR_SCO_AEC.MIC1" role="EPR_SCO_MIC1"/>
    <input sink="OPR_VOLUME_CONTROL.AUX_IN" role="EPR_VOLUME_AUX"/>
    <output source="OPR_SCO_AEC.SPEAKER1" role="EPR_SCO_SPEAKER"/>
    <output source="OPR_SCO_SEND.SCO_OUT" role="EPR_SCO_TO_AIR"/>
    <output source="OPR_SWITCHED_PASSTHROUGH_CONSUMER.FORWARD" role="EPR_SCOFWD_TX_OTA"/>

    <connection source="OPR_SCO_RECEIVE.OUT" sink="OPR_CVC_RECEIVE.IN"/>
    <connection source="OPR_CVC_RECEIVE.OUT" sink="OPR_SOURCE_SYNC.IN"/>
    <connection source="OPR_SOURCE_SYNC.OUT" sink="OPR_SCOFWD_SPLITTER.IN"/>
    <connection source="OPR_VOLUME_CONTROL.OUT"  sink="OPR_SCO_AEC.INPUT1"/>
    <connection source="OPR_SCOFWD_SPLITTER.OUT1"  sink="OPR_VOLUME_CONTROL.MAIN_IN"/>
    <connection source="OPR_SCOFWD_SPLITTER.OUT2"  sink="OPR_SCO_UP_SAMPLE.IN8K"/>
    <connection source="OPR_SCO_UP_SAMPLE.OUT16K"  sink="OPR_SCOFWD_SEND.IN"/>
    <connection source="OPR_SCOFWD_SEND.OUT"  sink="OPR_SWITCHED_PASSTHROUGH_CONSUMER.IN"/>
    <connection source="OPR_SCO_AEC.OUTPUT1" sink="OPR_CVC_SEND.IN1"/>
    <connection source="OPR_CVC_SEND.OUT" sink="OPR_SCO_SEND.IN"/>
    <connection source="OPR_SCO_AEC.REFERENCE" sink="OPR_CVC_SEND.REFERENCE"/>

</chain>




