'iis相关函数

'用说明查找web服务器
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

'用端口查找web服务器(有bug)
Function GetIISServerFromPort(port)
				findsv = Null	
				Set servsers = GetObject("IIS://localhost/W3SVC")
        For Each sv In servsers
      			For Each svp In sv.ServerBindings
      				WScript.Echo svp
     					If(svp=port) Then 
     						isFind = 1
     						Set findsv = sv
     						Exit For
     					End If
     				Next
     				If(IsNull(findsv) <> True) Then
     					Exit For
     				End if		
     		Next
     		Set GetIISServerFromPort = findsv
End Function