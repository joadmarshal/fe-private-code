#coding:GBK
import SuperiorReportTest
def test(arg):
    xmlsdgfdh = SuperiorReportTest.SuperiorReportTest()
    xmlsdgfdh.SendRequst(arg);
    
if __name__ == '__main__':  
    print('socket发包开始\n');
    import thread
    xmlsdgfdh = SuperiorReportTest.SuperiorReportTest()
    arg=(1,)
    xmlsdgfdh.SendRequst(arg);
 #   for i in range(1,1000):
 #       arg=(i,)
 #       thread.start_new_thread(test,arg)
    import time
    time.sleep(3000)
      
    