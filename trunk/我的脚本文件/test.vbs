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
MimeTypesToAddArray = Array(".manifest", "application/manifest", ".xaml", _ 
    "application/xaml+xml", ".application", "application/x-ms-application", _ 
    ".deploy", "application/octet-stream", ".xbap", "application/x-ms-xbap", _ 
    ".xps", "application/vnd.ms-xpsdocument", _ 
    ".msp","application/microsoftpatch", _ 
    ".msu","application/microsoftupdate", _ 
    ".vsto", "application/x-ms-vsto")


' Create a Shell object 
Set WshShell = CreateObject("WScript.Shell") 
Dim ArgCount
Dim Args(999)
Dim ArgObj

Set ArgObj = WScript.Arguments

Set MimeMapObj = GetObject("IIS://localhost/W3SVC/2/ROOT/Download")
WScript.Echo MimeMapObj.PropertyCount
For count = 0 to  MimeMapObj.PropertyCount 
     'WScript.Echo MimeMapObj.PropertyCount 
next

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
    
     ' Add a new mapping. 
     i = UBound(MimeMapArray) + 1 
     Redim Preserve MimeMapArray(i) 
     Set MimeMapArray(i) = CreateObject("MimeMap") 
     MimeMapArray(i).Extension = Ext 
     MimeMapArray(i).MimeType = MType 
     MimeMapObj.PutEx ADS_PROPERTY_UPDATE, "MimeMap", MimeMapArray 
     MimeMapObj.SetInfo 
         
End Sub 
