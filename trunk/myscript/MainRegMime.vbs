'hfy 
'使用说明 
'只需调用时传入,想要加入的IIS adsi路径作为参数 即可 
'格式是:IIS://localhost/W3SVC/3/ROOT/Download 
'
'要想增加加入的类型,可以修改下面的类型表
'汪意
'不能对同样路径执行两次!!!!!//参数 : adspath

Dim MimeMapObj, MimeMapArray,virdirobj, MimeTypesToAddArray, WshShell, oExec,ChildObject


Const ADS_PROPERTY_UPDATE = 2    
' 设定添加的类型
MimeTypesToAddArray = Array(".msu","application/microsoftupdate", _ 
    ".msp","application/microsoftpatch")


' Create a Shell object 
Set WshShell = CreateObject("WScript.Shell") 
Dim ArgCount

Set MimeMapObj = GetObject("IIS://localhost/MimeMap") 
For counter = 0 to UBound(MimeTypesToAddArray) Step 2 
     AddMimeType MimeTypesToAddArray(counter),MimeTypesToAddArray(counter+1)
next
' 重启IIS才有效 
'Set oExec = WshShell.Exec("net stop w3svc") 
'Do While oExec.Status = 0 
'     WScript.Sleep 100 
'Loop 
    
'Set oExec = WshShell.Exec("net start w3svc") 
'Do While oExec.Status = 0 
'     WScript.Sleep 100 
'Loop 
    
'Set oExec = Nothing 
  
' 添加类型的过程
Sub AddMimeType (Ext, MType) 
    
     ' Get the mappings from the MimeMap property. 
     MimeMapArray = MimeMapObj.GetEx("MimeMap")
     If(-1=FindMineIndex(MimeMapArray,Ext)) Then 
     ' Add a new mapping. 
	     i = UBound(MimeMapArray) + 1 
	     Redim Preserve MimeMapArray(i) 
	     Set MimeMapArray(i) = CreateObject("MimeMap") 
	     MimeMapArray(i).Extension = Ext 
	     MimeMapArray(i).MimeType = MType 
	     MimeMapObj.PutEx ADS_PROPERTY_UPDATE, "MimeMap", MimeMapArray 
	     MimeMapObj.SetInfo 
	   End If
End Sub 

Function FindMineIndex(MMArray,fExt)
				isFind=0
        For ci = 0 to UBound(MMArray) Step 1
     					If(fExt=MMArray(ci).Extension) Then 
     						isFind = 1
     						FindMineIndex = i
     						Exit For
     					End If
     		Next
     		If(0 = isFind) Then 
     			FindMineIndex = -1
     		End If
End Function
