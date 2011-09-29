#coding:GBK
import struct
import socket  
class SuperiorReportTest:
    "SuperiorReport test class"
    xmlRq0 = ('<root>'
               "<commandtype>0</commandtype>"
        "<commandoption>0</commandoption>"
        "<companyguid>155的子级</companyguid>"
        "<companyname>155的子级</companyname>"
        "<centerserverip>1233211</centerserverip>"
        "<linkstatus>0</linkstatus>"
        '</root>')
    xmlRq1 = ('<root>'
               "<commandtype>2</commandtype>"
        "<commandoption>2</commandoption>"
        "<companyguid>155的子级</companyguid>"
        "<companyname>155的子级</companyname>"
        "<centerserverip>1233211</centerserverip>"
        "<linkstatus>1</linkstatus>"
        "<companyinfo>"
        "<parentguid>6.155</parentguid>"
        "<parentname>6.155</parentname>"
        "<companyname>155的子级</companyname>"
        "<companydesc>155的子级</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级</companyname>"
        "<companydesc>155的子级的子级</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级2</companyname>"
        "<companydesc>155的子级的子级2</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级2</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级3</companyname>"
        "<companydesc>155的子级的子级3</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级3</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级4</companyname>"
        "<companydesc>155的子级的子级4</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级4</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级5</companyname>"
        "<companydesc>155的子级的子级5</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级5</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级6</companyname>"
        "<companydesc>155的子级的子级6</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级6</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级7</companyname>"
        "<companydesc>155的子级的子级7</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级7</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级8</companyname>"
        "<companydesc>155的子级的子级8</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级8</companyguid>"
        "</companyinfo>"
        
        "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级9</companyname>"
        "<companydesc>155的子级的子级9</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级9</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级10</companyname>"
        "<companydesc>155的子级的子级10</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级10</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级11</companyname>"
        "<companydesc>155的子级的子级11</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级11</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级12</companyname>"
        "<companydesc>155的子级的子级12</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级12</companyguid>"
        "</companyinfo>"
        "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级13</companyname>"
        "<companydesc>155的子级的子级13</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级13</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级14</companyname>"
        "<companydesc>155的子级的子级14</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级14</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级15</companyname>"
        "<companydesc>155的子级的子级15</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级15</companyguid>"
        "</companyinfo>"
                "<companyinfo>"
        "<parentguid>155的子级</parentguid>"
        "<parentname>155的子级</parentname>"
        "<companyname>155的子级的子级16</companyname>"
        "<companydesc>155的子级的子级16</companydesc>"
        "<centerserverip>0</centerserverip>"
        "<companyguid>155的子级的子级16</companyguid>"
        "</companyinfo>"
        
        '</root>')
    xmlRq3 = ("<?xml version=\"1.0\" encoding=\"gb2312\"?>"
        "<root>"
        "<commandtype>3</commandtype>"
        "<commandoption>0</commandoption>"
        "<companyguid>22222222</companyguid>"
        "<linkstatus>1</linkstatus>"
        "<summarydatas>"
        "<item parentguid=\"llllllll\" parentname=\"广州\" companyguid=\"22222222\" companyname=\"天河\" companydesc=\"天河\" datatype=\"终端发现\" dataitem=\"总终端数\" datavalue=\"10\" />"
        "</summarydatas>"
        "</root>")
    #head='\xE0\xE0\xE0\xE0' + struct.pack('i',len)
    def SendRequst(self,strId):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
        sock.connect(('172.25.6.155', 7890))
        xmlRq0=str(self.xmlRq0).replace("155的子级", "155的子级"+str(strId))
        xmlRq1=str(self.xmlRq1).replace("155的子级", "155的子级"+str(strId))
        xmlRq3=str(self.xmlRq3).replace("155的子级", "155的子级"+str(strId))
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