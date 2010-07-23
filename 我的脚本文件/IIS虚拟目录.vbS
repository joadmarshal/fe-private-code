

Dim txsv
'txsv="IIS://localhost/W3SVC/2"
Set txsv = GetIISServerFromServerComment("天珣配置服务")
'txsv = GetIISServerAdsPathFromPort("8833")
If(IsNull(txsv)) Then
	WScript.Echo "没有找到服务"
Else 
	WScript.Echo txsv.ADsPath
End If

WScript.Echo "pause"


Function GetIISServerFromServerComment(Comment)
				findsv = Null		
				Set servsers = GetObject("IIS://localhost/W3SVC")
        For Each sv In servsers
     					If(sv.ServerComment=Comment) Then 
     						isFind = 1
     						Set findsv = sv
     						Exit For
     					End If
     		Next
     		Set GetIISServerFromServerComment = findsv
End Function