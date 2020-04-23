
助手测试项目，以条不同条条组合。需要注意测试结果必须包涵：条状态下是否可以使用耳机MIC，助手声音从哪儿传输出来。
1、ANDROID & IOS
2、连接APP在手机上唤醒或在耳机唤醒
3、先启动系统助手或先启动APP助手    两者分别有在手机上启动或耳机端启动
测试：
1、测试一在ANDROID上，连接手机在手机上唤醒系统助手，耳机上是A2DP，不会使用耳机MIC
2、测试二在ANDROID上，连接手机在耳机唤醒系统助手，耳机上运行的是SCO通道，会使用耳机MIC,无HfpCallxxxActive
解决：
1、APP助手启动的情况下启动系统助手，先去停止APP助手



频繁拿出放入盒子；最后一次拿出盒子后，耳机不自动连接手机
    两只耳机同时拿出来，此种极限情况暂时不考虑，单独一只耳机快速出盒是可以连接的
    另外单耳使用时，两次快速出入盒（连接的情况下入盒-出盒-入盒-出盒）会出现不能连接的情况，调试是中因为最后
一次出盒时，耳机向手机发送连接请求，而因为上一次的连接没有断开，耳机会收到断开HFP的信息，然后导致不能再次连接了,也可能是手机端的影响 。



  PEER_SYNC_STATUS-->appSmHandlePeerSyncStatus-->appSmSetInitialCoreState-->appSmSetCoreState-->appSmSetCoreState
  ruleInCaseEnterDfu(这儿应该只需要进入一次）


Dormant进入方案：
   1、盒子在关盖的情况下，检测到耳机充电满时，会向耳机发送断开PLC的命令，然后就真的断开PLC并停止通信（耳机会检测到出盒）。
   2、耳机在检测到出盒时，查看是否接收到停止PLC通信标志，如果检测成功就立即进入dormant。
   3、如果出盒时，没有检测到PLC停止标志，设置30分钟的定时器，30分钟内没有入耳的情况下，自己进入dormant模式。

退出Dormant:
   1、用户操作开盒等启动PLC，耳机重新启动,检测到入盒信息。
   2、需要在启户拿出PLC之前，发送开盒信息。告诉耳机开盒了。实际没发送开盒也可以，耳机默认为开盒。


低功耗：

      appSmHandlePeerSyncStatus-->appSmSetInitialCoreState-->appSmSetCoreState
   -->appSmCalcCoreState-->not busy-->APP_STATE_OUT_OF_CASE_IDLE
   -->appEnterOutOfCaseIdle-->(appConfigIdleTimeoutMs)-->SM_INTERNAL_TIMEOUT_IDLE-->appSmHandleInternalTimeoutIdle
   -->appSetState(APP_STATE_OUT_OF_CASE_SOPORIFIC)-->appSmStateIsSleepy(previous_state) && appSmStateIsSleepy(new_state))
   -->appPowerClientAllowSleep-->appPowerSetState(POWER_STATE_SOPORIFIC_CLIENTS_NOTIFIED)-->SM(APP_POWER_SLEEP_PREPARE_IND)
   -->SM(APP_STATE_OUT_OF_CASE_SOPORIFIC_TERMINATING)-->appEnterSubStateTerminating()
   -->appSmInitiateLinkDisconnection()-->SM_INTERNAL_LINK_DISCONNECTION_COMPLETE/SM_INTERNAL_TIMEOUT_LINK_DISCONNECTION
         这个函数是断开当前的HANDSET&PEER，断开完成之后，才会去执行DISCONNECTION_COMPLETE。

    COMPLETE会执行appSmHandleInternalAllRequestedLinksDisconnected，此时会检测当前状态
      ->APP_STATE_OUT_OF_CASE_SOPORIFIC_TERMINATING 执行appPowerSleepPrepareResponse
      ->APP_SUBSTATE_TERMINATING-->appPowerShutdownPrepareResponse       这个为主动调用appPowerOffRequest

   -->appPowerSleepPrepareResponse()-->appPowerSetState(POWER_STATE_SOPORIFIC_CLIENTS_RESPONDED)-->appPowerEnterPowerStateSoporificClientsResponded()-->Dormant


DEMO板的几个测试，电压4.0V
  BT不连接:              : 1.8mA --->1.4mA
  BT  连接:              : 2.9mA
  BT+BLE ：              : 3.1mA
  BT+BLE+关掉外设        ：1.72mA
  BT+BLE+接近光          ：1.74mA  左右
  BT+BLE+接近光+敲       ：1.74mA  左右
  BT+BLE+接近光+敲+APO   ：2.40mA  （刚开始 2.9mA)
  入盒之后：             : 540uA （最底200uA 最高6.4mA)
  入耳状态下不连接手机   : 800uA

  左右耳的配对中功耗很大 : 6mA左右（非常大)
  出盒连接手机(无BLE)    ：670uA（打开了一下BLE,反而变成500uA，加上BLE为什么反而变少？好象是经典蓝牙的心跳间隔更大了）
  PowerOFF模式           : 3.9uA
  Dormant模式            : 270uA (如果打开接近光及敲击则为420uA，仅撕开接近光350uA）


BLE发送命令：============================================================================
 00000x01   MIC录音选择   01:主 02:副  03:主+副
 0000xx02   播放音频音量  00...7F
 0000xx03   播放提示音频  xx表示提示音频序号
 00000x04   CVC MIC 选译  01:主PASS  02:副PASS 其它：CVC

 0000xx10   打开外设  bit0:接近光 1:敲击 2:PLC 3:APO    4:骨麦 5:UCS接近光
 0000xx11   关闭外设

 00000112   入盒
 00000212   出盒
 00000312   关盒
 00000412   开盒
 00000512   入盒然后关盒

 00000113   PowerOFF
 00000213   关闭DSP
 00000313   TEST enter dormant



20200318
 仪器BT2000连接我们的设备进行音频测试需要注意的问题：
 1、出现不能切换到HFP，需要两个条件
   必须是耳机在入耳状态（inear），可以虚拟入耳或接近光挡住。
   必须是左右耳机已经配对的状态，左右耳机配对中是不能切换的（实际内部是无法入耳）；也可以是耳使用模式
   appHfpHandleHfpAudioConnectIndication-->appSmIsInEar()检测是否入耳
   下一版本软件可以发命令来代替拿出或挡住接近光的操作。
 2、出现Mode:[Idle],而不能连接成功的状态（连接成功为A2DP）
   多次出现这个问题里， 需要重新启动BT2000，应该是它的自身问题。
 3、HFP多次切换，没有解决。
   进一步的测试，在A2DP状态，可以快速的连续点击3次“Switch Hfp",很大概率会再次切换到HFP状态

MIC无法切换到HFP测试的解决方法：
   1、使用fake longkey 12这个命令来配对为单耳使用 （仅需要使用一次）
   2、不校准耳机的接近光传感器。
   3、以上两步还有行的话,  耳机传感器校准写固定值 check wr_sensor 15000 1000 50
   4、还有问题的话，请先连接一次手机，拨打一个10086，然后再回来测试。

 记录一下耳机进入DUT模式前(蓝牙传导测试），需要先发disable application可以解决右耳无法进入DUT模式的问题。


20200309
 appA2dpSetDefaultAttributes 这个函数来设置设置的音乐默认音量，appConfigDefaultVolumedB 计算出来为98.
 META10手机好象不会修改这儿的值。


20200305
 先拿出左耳机后拿出右耳机，手机设置音量， 先拿出右耳机后拿出左耳机，手机设置音量。导致不同的音量效果的问题。
 apple 手机与耳机连接成功后，调用 appAvrcpHandleSetAbsoluteVolumeInd -->appAvInstanceHandleAvAvrcpSetVolumeInd->appAvVolumeSet
 来设置音量。因为两只耳机的蓝牙地址不一至，手机上分别保存了不同的音量，因此会设置不同的音量值。
 调试：
   1、无论哪只耳机先出来，手机改写音量时都会自动同步到两只耳机中去。
   2、耳机与手机连接成功，会先主动读取自己保存的音量值，但这时AVRCP还没有成功连接，因此不能同步到手机上去。
   3、AVRCP连接成功之后，就会同步手机的音量过来。
 需要解决问题：
   1、如何将耳机音量同步到手机中去。appAvAvrcpVolumeNotification。
   2、如何去掉手机的第一次同步音量


20200227
 板卡生产功耗问题,以下为刚生产的析卡
 1、单耳模式， 解决左右耳机相互查找问题
 2、耳机在出盒状态(并会打开接近光),没有连接过手机会（appConfigAutoHandsetPairingTimeout）连接失败
    我们可以在手机配对


20200225
 appConfigAutoPeerPairingTimeout 设定耳机配对时间。 默认为0会一直查找直到成功，设置100后会在
 100秒后停止，但会再次启动查找，因此没有停下来。
   1、如果需要停止需要修改appSmHandlePairingPeerPairConfirm -> appSetState(APP_STATE_STARTUP); 去掉
   2、问题如果去掉了这个配对，那么什么时候再启动配对呢？


20200224
 appPhyStateInit初始化涵数中，以下几个步骤会执行对应的函数来清除以下LOCK状态
 phy_state->lock = PHY_STATE_LOCK_EAR | PHY_STATE_LOCK_MOTION | PHY_STATE_LOCK_CASE;
 appPhyStateInit -->appChargerClientRegister        ->CHARGER_MESSAGE_DETACHED
                 -->appProximityClientRegister      ->PROXIMITY_MESSAGE_IN_PROXIMITY
                 -->appAccelerometerClientRegister  ->ACCELEROMETER_MESSAGE_IN_MOTION
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




