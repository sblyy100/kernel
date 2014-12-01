#!/usr/bin/python
#encoding=utf-8
import sys,os,time
import re
rules = [
	r"(\w+)\s",
	r"(\w+)\s"
]
#replace something
def post_line(line):
	if line[-1] in (";"):
		line = line[:-1]
	
	return line
def read_from_h(file):
	fp = open(file)
	return fp
def match_line(line):
	for r in rules:
		m = re.match(r, line)
		if m:
			return True
	return False
def write_to_cpp(cfile,line):
	fp = open(cfile,"a")
	fp.write(line)
	fp.write("{\n}\n")
	fp.close()
def main(hfile,cfile):
	fp = read_from_h(hfile)
	for line in fp.readlines():
		if match_line(line):
			line = post_line(line)
			write_to_cpp(cfile,line)
	fp.close()

if __name__ == "__main__":
	main("server.h","server.c")
