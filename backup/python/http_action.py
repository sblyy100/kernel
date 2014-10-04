#!/usr/bin/python
#encoding=utf-8
import os,sys,time
action_list = {
"/poweroff" : "poweroff",
"/reboot" : "reboot",
"/dhcp" : "dhclient &",
"/caipiao":"python double_color.py",
}
