#coding:GBK
from os.path import walk, join, normpath
from macpath import normpath
import os 

import sys

def checkNetWorkLink(dspfile):
    file_object = open(dspfile)
    try:
        all_the_text = file_object.read()
        if all_the_text.find("\\\\") != -1:
            print dspfile
    finally:
        file_object.close()
def mydir(arg, dirname, names):#dirname目录名,names文件名序列
    for file in names:
        if file.find(".dsp")!=-1:
            checkNetWorkLink(join(dirname,file))
    #files=[normpath(join(dirname, file)) for file in names]
    #print "\n".join(files)

if __name__=="__main__":
    path='E:\\workspace_epol\\newest\\epol'
    walk(path, mydir, 0)#walk 枚举每个目录，运行mydir
    os.system('pause')