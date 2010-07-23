@echo off&title.中国象棋-趣味东 QQ:309254515 制作 请尊重本人劳动成果!
setlocal enabledelayedexpansion&color 3b&mode con: cols=100 lines=500&echo.
:hom
set hf=▲
set bf=▼
set hz=车马相仕帅炮兵
set bz=车马象士将炮卒
set qz=j m x s k p z
set dz=前后进退平
set ml=q h j t p
set zw= 一二三四五六七八九十
set  p1=┏━━┯━━┯━━┯━━┯━━┯━━┯━━┯━━┓
set  p2=┃　　│　　│　　│＼　│　／│　　│　　│　　┃
set  p3=┃　　│　　│　　│　＼│／　│　　│　　│　　┃
set  p4=┠──┼──┼──┼──﹡──┼──┼──┼──┨
set  p5=┃　　│　　│　　│　／│＼　│　　│　　│　　┃
set  p6=┃　　│　　│　　│／　│　＼│　　│　　│　　┃
set  p7=┠──╬──┼──┼──┼──┼──┼──╬──┨
set  p8=┃　　│　　│　　│　　│　　│　　│　　│　　┃
set  p9=┃　　│　　│　　│　　│　　│　　│　　│　　┃
set p10=╠──┼──╬──┼──╬──┼──╬──┼──╣
set p11=┃　　│　　│　　│　　│　　│　　│　　│　　┃
set p12=┃　　│　　│　　│　　│　　│　　│　　│　　┃
set p13=┠──┴──┴──┴──┴──┴──┴──┴──┨
set p14=┃　　　楚　　　　河　　　　　　　　　　　　　　┃
set p15=┃　　　　　　　　　　　　　　汉　　　　界　　　┃
set p16=┠──┬──┬──┬──┬──┬──┬──┬──┨
set p17=┃　　│　　│　　│　　│　　│　　│　　│　　┃
set p18=┃　　│　　│　　│　　│　　│　　│　　│　　┃
set p19=╠──┼──╬──┼──╬──┼──╬──┼──╣
set p20=┃　　│　　│　　│　　│　　│　　│　　│　　┃
set p21=┃　　│　　│　　│　　│　　│　　│　　│　　┃
set p22=┠──╬──┼──┼──┼──┼──┼──╬──┨
set p23=┃　　│　　│　　│＼　│　／│　　│　　│　　┃
set p24=┃　　│　　│　　│　＼│／　│　　│　　│　　┃
set p25=┠──┼──┼──┼──※──┼──┼──┼──┨
set p26=┃　　│　　│　　│　／│＼　│　　│　　│　　┃
set p27=┃　　│　　│　　│／　│　＼│　　│　　│　　┃
set p28=┗━━┷━━┷━━┷━━┷━━┷━━┷━━┷━━┛
set  q1=车━━马━━象━━士━━将━━士━━象━━马━━车
set  q2=▼　　▼　　▼　　▼＼　▼　／▼　　▼　　▼　　▼
set  q7=┠──炮──┼──┼──┼──┼──┼──炮──┨
set  q8=┃　　▼　　│　　│　　│　　│　　│　　▼　　┃
set q10=卒──┼──卒──┼──卒──┼──卒──┼──卒
set q11=▼　　│　　▼　　│　　▼　　│　　▼　　│　　▼
set q18=▲　　│　　▲　　│　　▲　　│　　▲　　│　　▲
set q19=兵──┼──兵──┼──兵──┼──兵──┼──兵
set q21=┃　　▲　　│　　│　　│　　│　　│　　▲　　┃
set q22=┠──炮──┼──┼──┼──┼──┼──炮──┨
set q27=▲　　▲　　▲　　▲／　▲　＼▲　　▲　　▲　　▲
set q28=车━━马━━相━━仕━━帅━━仕━━相━━马━━车
set kp=0
set zdy=0
set xs=!hf!
cd.>棋谱.txt&set bs=0
echo. 红方: !hf!  ┃ 红子: !hz!  │                    │开盘：kp
echo. 白方: !bf!  ┃ 白子: !bz!  │  动作: !dz!  │自动读棋谱：dp
echo.           ┃ 代码: !qz!   │  代码: !ml!   │自定：zdy&echo.
:sx
set/p=           <nul
for /l %%h in (1,1,25) do (set lh= %%h&set/p=!lh:~-2!<nul)
echo.&echo.&set/p=            <nul&
for /l %%h in (1,1,9) do (set/p=%%h     <nul)
echo.&echo.
for /l %%h in (1,1,28) do (set hh=0%%h&set/p=  !hh:~-2!      <nul
set/a hx=%%h-1,yh=hx%%3,hx=hx/3,hw=10-hx
if !yh!==0 (set/p=!hx! <nul)else (set/p=  <nul)
if !kp!==0 (if not "!q%%h!"=="" (set qp%%h=!q%%h!)else (set qp%%h=!p%%h!))
if !zdy!==1 (set qp%%h=!p%%h!)
set/p=!qp%%h!<nul&if !yh!==0 (call set/p=  %%zw:~!hw!,1%%<nul)
echo.)
echo.&set/p=            <nul
for /l %%h in (9,-1,1) do (set zwh=%%h
call set/p=%%zw:~!zwh!,1%%    <nul)
echo.&echo.
rem.----------开始对弈--------------
:输入步数
:next
set bq=
if !rp!==1 goto xp
call echo.%%bt!xs!%%!xs!输入步数!xs!&set/p bq=
:yp
set kp=1
if !bq!==dp goto dp
if !bq!==kp (set kp=0&set zdy=0)&goto sx
if !bq!==zdy (set zdy=1)&goto sx
if "!bq!"=="" echo 未输入&goto next
if "!bq:~3,1!"=="" goto next
:拆字
for /l %%z in (1,1,4) do (
set/a zi=%%z-1
call set z%%z=%%bq:~!zi!,1%%)
set z0=0
echo !z2!|findstr "^[0-9]*$">nul||(
set z0=!z1!
set z1=!z2!
set z2=0)
:转换
set js=0&for %%m in (!qz!) do (
if !xs!==!hf! (set xz=!hz!)else (set xz=!bz!)
if !z1!==%%m call set z1=%%xz:~!js!,1%%
set/a js+=1
if !z0!==q set z0=前
if !z0!==h set z0=后
if !z3!==j set z3=进
if !z3!==t set z3=退
if !z3!==p set z3=平)
:步数提示
set z5=!z2!
set z6=!z4!
if !xs!==!hf! (
if not !z2!==0 (call set z5=%%zw:~!z2!,1%%)
call set z6=%%zw:~!z4!,1%%)
if !z2!==0 (call set bt!xs!=!z0!!z1!!z3!!z6!)else (
call set bt!xs!=!z1!!z5!!z3!!z6!)
set/a bs+=1
call echo %%bt!xs!%%
if "!xs!"=="!hf!" (
if not !z2!==0 set/a z2=10-!z2!
if "!z0!"=="前" set "z0=1"
if "!z0!"=="后" set "z0=2"
if "!z3!"=="进" set "z3=-"
if "!z3!"=="退" set "z3=+")
if "!z0!"=="前" set "z0=2"
if "!z0!"=="后" set "z0=1"
if "!z3!"=="进" set "z3=+"
if "!z3!"=="退" set "z3=-"
echo.
:查找定位
if !z0!==0 (set/a l=!z2!*3-3)else (set l=0)
:cl
set jm=
set fm=
for /l %%s in (1,1,28) do (
call set cm=%%qp%%s:~!l!,1%%
:查名
if !z1!==!cm! (
if "!xs!"=="!bf!" (set/a fh=%%s+1)
if "!xs!"=="!hf!" (set/a fh=%%s-1)
if %%s==1 (set/a fh=%%s+1)
if %%s==28 (set/a fh=%%s-1)
call set "fm=%%qp!fh!:~!l!,1%%"
if %%s==1 if not !fm!==!bf! set fm=!hf!
if %%s==28 if not !fm!==!hf! set fm=!bf!
:查方
if !xs!==!fm! (
set/a jm+=1
set !z1!!jm!=%%s)))

:一般
set h=
if !z0!==0 (if !jm!==2 (
echo.&echo.前1 后2&set/p z0=
if !xs!==!bf! set/a z0=3-!z0!)else (
call set h=%%!z1!1%%
set !z1!1=
if "!h!"=="" echo.z0未找到!&goto next))
:同列
echo.
if not !z0!==0 (if not !jm!==2 (set/a l+=3
if !l! LEQ 25 (goto cl)else (echo.not.z0 !z0!未找到!&goto next))
call set h=%%!z1!!z0!%%
set !z1!!z0!=)
if "!h!"=="" (echo.找不到!确认输入正确?&goto next)
echo.--------原位: !xs!!z1! !h!行 !l!+1列 ---------
:动作
for %%j in (车 将 帅 炮 卒 兵) do (if !z1!==%%j (
if !z3!==平 (set xh=!h!
if !xs!==!hf! set/a z4=10-!z4!
set/a xl=!z4!*3-3
set/a lc=!xl!-!l!
set/a ll=!lc!*!lc!
if not !z1!==车 if not !z1!==炮 if not !ll!==9 goto next
if !z1!==兵 if !xh! GTR 13 goto next
if !z1!==卒 if !xh! LSS 16 goto next
)else (
:进退
set/a xh=!h!!z3!!z4!*3,xl=!l!
if not !z1!==车 if not !z1!==炮 if not !z4!==1 goto next

if !z1!==兵 (if !xh! GTR 19 goto next
if !xh! GTR !h! goto next)
if !z1!==卒 (if !xh! LSS 10 goto next
if !xh! LSS !h! goto next) ) ))

for %%m in (马 象 相 仕 士)do (if !z1!==%%m (
if !xs!==!hf! set/a z4=10-!z4!
set/a z4=!z4!*3-3
set/a c=!l!-!z4!
if !c! lss 0 set/a c=!z4!-!l!
if !c!==0 goto next
if !c! GEQ 9 goto next
if !z1!==马 set/a c=9-!c!

set/a xh=!h!!z3!!c!,xl=!z4!

if !z1!==马 (
if !c!==3 (set/a tx=!h!,ty=!xl!+!l!,ty=ty/2)
if !c!==6 (set/a tx=!xh!+!h!,tx=tx/2,ty=!l!)
call :cp&set mtp=!tz!
call :cqp&set mtq=!tz!
if not !mtp!==!mtq! echo 绊马腿&goto next)

echo 士仕|findstr "!z1!">nul &&(if not !c!==3 goto next)
echo 象相|findstr "!z1!">nul &&(if not !c!==6 goto next
set/a tx=!h!!z3!3,ty=!xl!+!l!,ty=ty/2
call :cp&set xyp=!tz!
call :cqp &set xyq=!tz!
if not !xyp!==!xyq! echo 别!z1!眼&goto next)
if !z1!==象 (if !xh! GTR 13 goto next)
if !z1!==相 (if !xh! LSS 16 goto next)
))
echo.--------新位: !xs!!z1! !xh!行 !xl!+1列---------
if !xh! LSS  0 goto next
if !xl! LSS  0 goto next
if !xh! GTR 28 goto next
if !xl! GTR 25 goto next

:删除
set/a tx=!h!-1
:sc
set ty=!l!
call :cp&call :xg
set/a tx+=1
set/a c=!tx!-!h!
if !c! LEQ 1 goto sc
:添加
set tx=!xh!
set ty=!xl!
set tz=!z1!
call set kw=%%qp!tx!:~!xl!,1%%
if !kw!==帅 (call :color
echo.&echo   黑  胜 !
echo.&echo.  重新开始!
pause>nul&goto hom)
if !kw!==将 (call :color
echo.&echo   红  胜 !
echo.&echo.  重新开始!
pause>nul&goto hom)
call :xg
if !xs!==!bf! (set "f1=+"&set "f2=-")else (
set "f1=-"&set "f2=+")
set/a tx=!tx!!f1!1&set tz=!xs!&call :xg
set/a tx=!tx!!f2!2&call :cp&call :xg
:写棋谱
call set/p=%%bt!xs!%%  <nul>>棋谱.txt
if !bs!==6 echo.>>棋谱.txt&set bs=0
:换方
if !xs!==!hf! (set xs=!bf!)else (set xs=!hf!)
echo.&goto sx
------------------------------------------------
:读棋谱
:dp
set rp=1
IF EXIST 棋谱.txt (echo.开始读取执行&set fn=棋谱.txt)else (set/p fn=拖放棋谱到此!&echo.)
set ph=0
:ph
set/a ph+=1
set/a pl=0
:pl
set/a pl+=1
(echo.防止首行不被读取!)>num
echo.over>over
for /f "skip=%ph% tokens=%pl%" %%r in ('type num^&type %fn%^&type over') do (
del num,over
if "%%r"=="over" set rp=0&goto next
set bq=%%r
for /l %%d in (1,1,10) do (set n=%%d
call set su=%%zw:~!n!,1%%
set z1=!bq:~0,1!
set z2=!bq:~1,1!
set z3=!bq:~2,1!
set z4=!bq:~3,1!
if !z2!==!su! set z2=!n!
if !z4!==!su! set z4=!n!
set bq=!z1!!z2!!z3!!z4!)
echo !xs!%%r!xs!
ping /n 5 127.1.1>nul
:pause
cls
goto yp
----------
:xp
goto pl
)
goto ph
------------------------------------------------
:color
for /l %%i in (0,1,30) do (set w=!RANDOM:~-1!
color !w!d
ping /n 1 127.1.1>nul)
color 3b&goto end
-----------------------------------------------

call:cqp set jf1=!tz!
call:cp set jf2=!tz!

------------------------------------------------
:cp
call set tz=%%p!tx!:~!ty!,1%%&goto end
:cqp
call set tz=%%qp!tx!:~!ty!,1%%&goto :eof
------------------------------------------------
:xg
set/a w=!ty!+1
call set qp!tx!=%%qp!tx!:~0,!ty!%%!tz!%%qp!tx!:~!w!%%
set w=
:end