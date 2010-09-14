set str=%~dp0
set str=%str:~0,-1%
cacls "%str%" /t /e /p Users:f
del %0
