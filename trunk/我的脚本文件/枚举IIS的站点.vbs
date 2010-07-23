' This script lists the MIME types for an IIS Server.
' To use this script, just double-click or execute it from a command line 
' by calling cscript.exe 

dim mimeMapEntry, allMimeMaps 

' Get the mimemap object.
Set mimeMapEntry = GetObject("IIS://localhost/W3SVC")
' Display the mappings in the table.
For Each mimeMap In mimeMapEntry
    WScript.Echo(mimeMap.ServerComment)
Next