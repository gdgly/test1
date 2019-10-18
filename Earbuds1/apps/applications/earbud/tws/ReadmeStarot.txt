






20191015
  升级命令格式，主要就是命令GAIA_COMMAND_VM_UPGRADE_CONTROL(0x642) + 字命令（1byte UPGRADE_HOST_XXX）
  UPGRADE_HOST_SYNC + 长度(2Byte) + progID（4Byte)
  -->000a0642    13 0004 a0a51c09
  <--000a4003 12 14 0006 00 a0a51c09 03        进入UPGRADE_STATE_READY
  UPGRADE_HOST_START_REQ
  -->000a0642    01 0000
  <--000a4003 12 020003 000666                 进入UPGRADE_STATE_DATA_READY
  UPGRADE_HOST_START_DATA_REQ
  -->000a0642    15 0000
    注意接收到这条命令，会准备并擦除分区，是需要时间的，分区不正确会导致系统重启(PANIC 0xb6 4108 (PANIC_P0_PANICKED)。
    擦除成功会获取底层 MESSAGE_IMAGE_UPGRADE_ERASE_STATUS 消息。
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




