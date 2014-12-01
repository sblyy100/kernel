#!/usr/bin/python
#encoding=utf-8
import os,sys,time
import getopt
from price import *
exchange_rate = 0.053
def usage(app):
	print "%s -w unit weight" %(app)
	print "%s -r exchange rate"  %(app)
	print "%s -m transfer mode: 0:plane 1:sal 2:ship ,default is ship" %(app)
	print "%s -h help" %(app)
#unit_weight
#num
#mode:PLANE:0,SAL:1,SHIP:2
def calc_price(unit_weight,num,mode):
	pl = PRICE[mode]
	w = unit_weight * num
	keys = pl.keys()
	keys.sort()
	keys.reverse()
	weight = 0
	while True:
		weight = keys.pop()
		if weight < w:
			continue
		else:
			break
	post_price = pl[weight]
	return post_price

if __name__ == "__main__":
	weight = 2000
	mode = 2
	rate = exchange_rate
	options,args = getopt.getopt(sys.argv[1:],"hw:r:m:",["help","weight","rate","mode"])
	#print options
	for k,v in options:
		if k in ("-h","--help"):
			usage(sys.argv[0])
			sys.exit(1)
		elif k in ("-w","--weight"):
			weight = int(v)
		elif k in ("-r","--rate"):
			exchange_rate = int(v)
		elif k in ("-m","--mode"):
			mode = int(v)
		else:
			usage(sys.argv[0])
			sys.exit(1)
	for num in (1,2,3,4,5,6,7,8,9,10):
		print "weight:%dkg num:%d price:%d" %(weight*num/1000, num, calc_price(weight,num,mode)*rate/num)

