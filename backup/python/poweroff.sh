#!/bin/bash
#poweroff after 1 o'clock or $1
set -x
deadline=1
function guanji(){
	echo "poweroff"
	poweroff
	#return 1
}
function die(){
	echo "failed"
}
function timediff(){
	#$1 start
	#$2 end
	echo $(((`expr $2 - $1`)/3600))
}
main(){
	[ -n "$1" ]&& deadline=$1
	#echo $deadline
	now=`date +%H`
	up=`uptime -s`
	up_stamp=`date -d "$up" +%s`
	now_stamp=`date +%s`
	long=`timediff $up_stamp $now_stamp`
	#echo $long
	#echo $now
	while true;do
		[ $now -gt "$deadline" ] && [ $now -lt "07" ] && [ $long -gt "2" ] && guanji || die
		sleep 1800
	done
}
main $*
