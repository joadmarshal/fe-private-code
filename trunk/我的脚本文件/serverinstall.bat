sc create ESSDServer binpath= "%~dp0\SDServer.exe --service -r" displayname= "ES Software Distribute Server"
sc description ESSDServer �쫑������ȫ���չ��������ϵͳ����ַ��������
sc config ESSDServer start= auto
pause