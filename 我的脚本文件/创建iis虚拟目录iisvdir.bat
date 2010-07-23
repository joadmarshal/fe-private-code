CScript %windir%\system32\iisvdir.vbs /create "天珣配置服务" delivery "%~dp0"
%windir%\system32\inetsrv\appcmd add app /site.name:"天珣配置服务" /path:/delivery /physicalPath:"%~dp0
pause