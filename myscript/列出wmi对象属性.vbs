strComputer = "."
Set objWMIService=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")

For Each objclass in objWMIService.SubclassesOf()
intCounter=0
If Left(objClass.Path_.Class,5) = "Win32" Then
For Each Qualifier in objClass.Qualifiers_
If UCase(Trim(Qualifier.Name)) = "ASSOCIATION" Then
intCounter = 1
End If
Next
If x = 0 Then
strComputer = "."
Set objWMIService = GetObject _
("winmgmts:{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set strClass = objWMIService.Get(objClass.Path_.Class)
Wscript.Echo "PROPERTIES:"
For each strItem in strClass.properties_
Wscript.Echo objClass.Path_.Class & vbTab & strItem.name 
Next
Wscript.Echo "METHODS:"
For Each strItem in strClass.methods_
Wscript.Echo objClass.Path_.Class & vbTab & strItem.name 
Next
End If
End If
Next