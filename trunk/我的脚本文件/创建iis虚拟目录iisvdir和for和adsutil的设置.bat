for /f "skip=6 tokens=4" %%i in ('CScript %windir%\system32\iisvdir.vbs /create "Ìì«‘ÅäÖÃ·şÎñ" delivery66 "%~dps0"') do cscript Adsutil.vbs SET %%i\AccessFlags 513

pause