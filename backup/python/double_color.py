#!/usr/bin/python
#encoding=utf-8
import os,sys,time
import random
def choose_n_from_list(l,n):
	lofl = len(l)
	result = []
	for i in range(n):
		r = random.randint(0,lofl-1)
		result.append(l.pop(r))
		lofl = lofl - 1
	result.sort()
	return result
def choose_red():
	red = range(1,34)
	r_red = choose_n_from_list(red,6)
	return list_to_string(r_red)
def choose_blue():
	blue = range(1,17)
	r_blue = choose_n_from_list(blue,1)
	return list_to_string(r_blue)
def list_to_string(l):
	str = ""
	str += "["
	for i in l:
		str += " %s" %(i)
	str +="]"
	return str
if __name__ == "__main__":
	str = ""
	str += choose_red()
	str += choose_blue()
	print str
