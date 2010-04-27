sc create ESSDServer binpath= "%~dp0\SDServer.exe --service -r" displayname= "ES Software Distribute Server"
sc description ESSDServer 天珣内网安全风险管理与审计系统软件分发服务进程
sc config ESSDServer start= auto
pause