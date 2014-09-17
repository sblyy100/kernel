#!/bin/bash
#set -x
function mount_disk(){
    echo "mount---"
    mount /dev/sda5 ~/d
    mount /dev/sda6 ~/e
    mount /dev/sda7 ~/f
}
function umount_disk(){
    echo "umount---"
    umount /dev/sda5 
    umount /dev/sda6 
    umount /dev/sda7
}
echo ========$1===============
if [ -z $1 ];then
mount_disk
elif [ "$1" = "-m" ];then
mount_disk
else
umount_disk
fi

ping 10.66.90.128 -c1 && mount 10.66.90.128:/vol/S2/kvmauto /media/
