Option Explicit
Dim d, wmi, disks, logicalDiskSet, count, partitions
Set wmi=GetObject("winmgmts:\\")
Set logicalDiskSet = wmi.InstancesOf("Win32_LogicalDisk")
Function FormatSize(size)
   Dim s : s = ""
   If size>1024 Then
      size=(size/1024)
      s = FormatNumber(size, 1) & " KB"
   End If
   If size>1024 Then
      size=(size/1024)
      s = FormatNumber(size, 1) & " MB"
   End If
   If size>1024 Then
      size=(size/1024)
      s = FormatNumber(size, 1) & " GB"
   End If
   FormatSize = s
End Function

Sub GetLogicalDisk()
   dim ld,str,rn,fN,f,rpath,N,fso
   For Each ld In logicalDiskSet 
       str = str & ld.Name & "  " & FormatSize(ld.Size) & Chr(32) & chr(13) & chr(10)
   Next
    set rn=WScript.CreateObject("Wscript.Shell")
    rpath="HKLM\System\CurrentControlSet\Control\ComputerName\ComputerName\ComputerName" 
    N=rn.RegRead(rpath) '取机名
    fN="E:\disk.txt" '写文件 指定好路径
    Set fso=Wscript.CreateObject("Scripting.FileSystemObject")
    Set f=fso.OpenTextFile(fN,8,True)
    f.WriteLine str 
    f.close
End Sub

GetLogicalDisk()