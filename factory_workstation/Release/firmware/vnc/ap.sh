#!/bin/sh
MACADDR=`macgen`
rm /dev/android_adb
echo ssid=`env |grep SVN_NUM= |awk -F"SVN_NUM=" '{printf $NF;}'` >> /tmp/hostapd.conf
ifconfig wlan0 up hw ether $MACADDR
ifconfig wlan0 192.168.12.1
nohup /tmp/hostapd /tmp/hostapd.conf > /dev/null &
usleep 200000
killall adbd
