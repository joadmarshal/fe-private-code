CScript %windir%\system32\iisvdir.vbs /create "�쫑���÷���" delivery "%~dp0"
%windir%\system32\inetsrv\appcmd add app /site.name:"�쫑���÷���" /path:/delivery /physicalPath:"%~dp0
pause