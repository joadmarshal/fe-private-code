#coding:GBK
import struct
import socket  
class SuperiorReportTest:
    "SuperiorReport test class"
    xml = ('<root>'
               "<commandtype>0</commandtype>"
        "<commandoption>0</commandoption>"
        "<companyguid>155的子级</companyguid>"
        "<companyname>155的子级</companyname>"
        "<centerserverip>1233211</centerserverip>"
        "<linkstatus>1</linkstatus>"
        '<root/>')
    len = len(xml)
    
    head='\xE0\xE0\xE0\xE0' + struct.pack('i',len)
    #head='\xE0\xE0\xE0\xE0' + struct.pack('i',len)
    def SendRequst(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
        sock.connect(('172.25.85.200', 7890))
        dat = self.head + self.xml
        sock.send(dat)
        #sock.send(self.head)
        #sock.send(self.xml)
        recvdata = sock.recv(1024)
        #updata = struct.unpack('4sis',recvdata)
        print recvdata
        sock.close()
        return self.xml