#coding:GBK
import struct
import socket  
class SuperiorReportTest:
    "SuperiorReport test class"
    xmlRq0 = ('<root>'
               "<commandtype>0</commandtype>"
        "<commandoption>0</commandoption>"
        "<companyguid>155���Ӽ�</companyguid>"
        "<companyname>155���Ӽ�</companyname>"
        "<centerserverip>1233211</centerserverip>"
        "<linkstatus>0</linkstatus>"
        '</root>')
    xmlRq1 = ('<root>'
               "<commandtype>2</commandtype>"
        "<commandoption>2</commandoption>"
        "<companyguid>155���Ӽ�</companyguid>"
        "<companyname>155���Ӽ�</companyname>"
        "<centerserverip>1233211</centerserverip>"
        "<linkstatus>1</linkstatus>"
        "<companyinfo>"
        "<parentguid>6.155</parentguid>"
        "<parentname>6.155</parentname>"
        "<companyname>155���Ӽ�</companyname>"
        "<companydesc>155���Ӽ�</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ�</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�</companyname>"
        "<companydesc>155���Ӽ����Ӽ�</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�2</companyname>"
        "<companydesc>155���Ӽ����Ӽ�2</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�2</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�3</companyname>"
        "<companydesc>155���Ӽ����Ӽ�3</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�3</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�4</companyname>"
        "<companydesc>155���Ӽ����Ӽ�4</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�4</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�5</companyname>"
        "<companydesc>155���Ӽ����Ӽ�5</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�5</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�6</companyname>"
        "<companydesc>155���Ӽ����Ӽ�6</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�6</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�7</companyname>"
        "<companydesc>155���Ӽ����Ӽ�7</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�7</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�8</companyname>"
        "<companydesc>155���Ӽ����Ӽ�8</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�8</companyguid>"
        "</companyinfo>"
        
        "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�9</companyname>"
        "<companydesc>155���Ӽ����Ӽ�9</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�9</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�10</companyname>"
        "<companydesc>155���Ӽ����Ӽ�10</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�10</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�11</companyname>"
        "<companydesc>155���Ӽ����Ӽ�11</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�11</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�12</companyname>"
        "<companydesc>155���Ӽ����Ӽ�12</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�12</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�13</companyname>"
        "<companydesc>155���Ӽ����Ӽ�13</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�13</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�14</companyname>"
        "<companydesc>155���Ӽ����Ӽ�14</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�14</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�15</companyname>"
        "<companydesc>155���Ӽ����Ӽ�15</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�15</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155���Ӽ�</parentguid>"
        "<parentname>155���Ӽ�</parentname>"
        "<companyname>155���Ӽ����Ӽ�16</companyname>"
        "<companydesc>155���Ӽ����Ӽ�16</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155���Ӽ����Ӽ�16</companyguid>"
        "</companyinfo>"
        
        '</root>')
    xmlRq3 = ("<?xml version=\"1.0\" encoding=\"gb2312\"?>"
        "<root>"
        "<commandtype>3</commandtype>"
        "<commandoption>0</commandoption>"
        "<companyguid>22222222</companyguid>"
        "<linkstatus>1</linkstatus>"
        "<summarydatas>"
        "<item parentguid=\"llllllll\" parentname=\"����\" companyguid=\"22222222\" companyname=\"���\" companydesc=\"���\" datatype=\"�ն˷���\" dataitem=\"���ն���\" datavalue=\"10\" />"
        "</summarydatas>"
        "</root>")
    #head='\xE0\xE0\xE0\xE0' + struct.pack('i',len)
    def SendRequst(self,strId):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
        sock.connect(('172.25.6.155', 7890))
        xmlRq0=str(self.xmlRq0).replace("155���Ӽ�", "155���Ӽ�"+str(strId))
        xmlRq1=str(self.xmlRq1).replace("155���Ӽ�", "155���Ӽ�"+str(strId))
        xmlRq3=str(self.xmlRq3).replace("155���Ӽ�", "155���Ӽ�"+str(strId))
        headRq0='\xE0\xE0\xE0\xE0' + struct.pack('i',len(xmlRq0))
        headRq1='\xE0\xE0\xE0\xE0' + struct.pack('i',len(xmlRq1))
        headRq3='\xE0\xE0\xE0\xE0' + struct.pack('i',len(xmlRq3))
        dat = headRq0 + xmlRq0
        sock.send(headRq0)
        sock.send(xmlRq0)
        recvdata = sock.recv(8)
        datalen = struct.unpack('i',recvdata[4:])[0]
        print 'recvXmllen = ' + str(datalen)
        recvdata = sock.recv(datalen)
        print repr(recvdata)
        for i in range(1,2):
            print 'the ' + str(i) +' test!';
            dat = headRq1 + xmlRq1
            sock.send(dat)
            recvdata = sock.recv(8)
            datalen = struct.unpack('i',recvdata[4:])[0]
            print 'recvXmllen = ' + str(datalen)
            recvdata = sock.recv(datalen)
            print repr(recvdata)
        
        sock.close()