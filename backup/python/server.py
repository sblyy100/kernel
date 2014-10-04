#!/usr/bin/python
#encoding=utf-8
import os,sys,time
import socket
import SocketServer
import http_data
from http_action import *
debug = 0
M_GET = 0
M_POST = 1
M_UNKNOWN = -1
V_09 = 0
V_10 = 1
V_11 = 2
V_UNKNOWN = -1
def main(argv):
	# help(SocketServer.TCPServer)
	print "start..."
	if len(argv) > 1:
		print argv
		port = int(argv[1])
	else:
		port = 80
	ts = SocketServer.TCPServer(("0.0.0.0",port),request_handler,True)
	ts.serve_forever()
	
class request_handler(SocketServer.BaseRequestHandler):
	#def __init__(self):
	#	self.methpd = None
	#	self.uri = None
	#	self.version = None
	#	self.host = None
	def handle(self):
		self.recv_buf = ""
		self.send_buf = "hello"
		while(True):
			buf = self.request.recv(1024)
			if len(buf) > 0:
				self.recv_buf += buf
			if len(buf) < 1024:
				break
		#self.decode(self.recv_buf,self.client_address)
		#res = self.gen_response()
		req=Http_request(self.recv_buf)
		req.decode()
		#a.show()
		res = Http_response(req)
		res.gen_response_data()
		self.response(res.data)#todo
		self.request.close()
	
	def response(self,data):
		#write_log("send data %s" %data)
		self.request.sendall(data)
class Http_request():
	def __init__(self,data):
		self.raw_data = data
		self.request = "" #请求体
		self.request_line = ""#请求行
		self.request_headers_data = ""#请求头部数据
		self.post_data = ""#post data
		self.method = M_GET #请求方法，
		self.uri = ""	#uri
		self.version = ""
		self.host = ""
		self.headers = {}
	def show(self):
		print "========================="
		print "method:",self.method
		print "uri:",self.uri
		print "version:",self.version
		print "host:",self.host
		print "headers",self.headers
		print "========================="
	def decode(self):
		request = self.raw_data.split("\r\n\r\n",1)
		if len(request) < 1:
			write_log("decode request raw data failed")
			return -1
		elif len(request) == 1:
			self.request = request[0]
		elif len(request) == 2: 
			self.request,self.post_data = request
		self.decode_request()
		self.decode_data()
	def decode_request(self):
		res = self.request.split("\r\n",1)
		if len(res) == 2:
			self.request_line,self.request_headers_data = res
		else:
			write_log("decode request lien, headers failed")
			return -1
		self.decode_request_line()
		self.decode_headers()
	def decode_request_line(self):
		res = self.request_line.split(" ")
		if len(res) == 3:
			write_log("start decode request line %s" %str(res))
			method,uri,version = res
			self.decode_method(method)
			self.decode_uri(uri)
			self.decode_version(version)
	def decode_method(self,method):
		if method == "GET":
			self.method = M_GET
		elif method == "POST":
			self.method = M_POST
		else:
			self.method = M_UNKNOWN
	def decode_uri(self,uri):
		self.uri = uri
	def decode_version(self,version):
		if version == "HTTP/0.9":
			self.version = V_09
		elif version == "HTTP/1.0":
			self.version = V_10
		elif version == "HTTP/1.1":
			self.version = V_11
		else:
			self.version = V_UNKNOWN
	def decode_headers(self):
		for header in self.request_headers_data.splitlines():
			k,v = header.split(":",1)
			self.headers[k] = v
			if k.lower() == "host":
				self.host = v
	def decode_data(self):
		pass
class Http_response():
	def __init__(self,request):
		self.request = request
		self.data = "" #最终data
		self.version = V_11
		self.status = 404
		self.date = ""
		self.body = ""
		self.server = "httpd/sbl"
		self.content_type = "text/html"
	def gen_response_data(self):
		self.action()
		self.append_version()
		self.append_status()
		self.append_headers()
		self.append_body()
	def append_version(self,version=V_11):
		self.data+="HTTP/1.1 "
	def append_status(self,status=404):
		self.data+="200 OK\r\n"
	def append_headers(self,headers=None):
		self.data+="Server:%s\r\n" %(self.server)
		self.data+="Content-type:%s\r\n" %(self.content_type)
		self.data+="\r\n"
	def append_body(self,data=None):
		if self.body != "":
			self.data += self.body
		else:
			self.data+=http_data.welcome
	def action(self):
		act = Http_action(self.request)
		self.body = act.run()
class Http_action():
	def __init__(self,request):
		self.request = request
		self.uri = ""
		self.action = ""
		self.cmd = ""
		self.ret = "" #return data
	def parse_action(self):
		self.uri = self.request.uri
		print self.uri
		if action_list.has_key(self.uri):
			self.action = action_list[self.uri]
		else:
			self.action=self.cmd = ""
		self.cmd = self.action
		self.data = self.action
	def run(self):
		self.parse_action()
		if self.cmd != "" and not debug:
			self.data = os.popen(self.cmd).read()
		return self.data
def write_log(buf):
	print time.ctime()+''+buf+"\n"
	#pass
if __name__ == '__main__':
	main(sys.argv)
