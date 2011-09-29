@echo on 

echo 删除VC编译产生的临时文件

echo 删除开始...

for /r . %%c in (.) do @if exist "%%c\Debug" rd /S /Q "%%c\Debug"

for /r . %%c in (.) do @if exist "%%c\Release" rd /S /Q "%%c\Release"

for /r . %%c in (*.bsc *.aps *.clw *.ncb *.plg *.positions *.WW *.user) do del "%%c"

for /r . %%c in ("*VA_C__Documents and Settings_Administrator_Local Settings_Application Data_VisualAssist_vc6_history_") do del "%%c"

echo 删除完毕...

#gcode psw uH6Fj2ew7nB3
pause
