@ECHO OFF

set OUT_EXE=ld47

set TARGET=x64

set FLAGS=/std:c++latest /W4 /FC /MP /EHsc /permissive-
set LD_FLAGS=

if "%1" == "run" (
  pushd build
  .\%OUT_EXE%.exe
  popd

  exit
)

if "%1" == "release" (
  echo [Release build]
  @REM set FLAGS=%FLAGS% /O2 /DNDEBUG /MT
  set FLAGS=%FLAGS% /O2 /DNDEBUG
) else (
  echo [Debug build]
  @REM set FLAGS=%FLAGS% /Zi /MDd /Od
  set FLAGS=%FLAGS% /Od /Zi
  set LD_FLAGS=%LD_FLAGS% /DEBUG
)
echo[

set DEFS=

set DEP_LIB=dep\lib
set DEP_INC=dep\inc
set DEP_SRC=dep\src

REM SFML
set SFML_LIB=%DEP_LIB%\SFML\%TARGET%
set SFML_INC=%DEP_INC%\SFML

REM ImGUI
REM set IMGUI_SRC=%DEP_SRC%\imgui
REM set IMGUI_INC=%DEP_INC%\imgui

set LIBS=^
  /libpath:%SFML_LIB% ^
  sfml-window.lib ^
  sfml-graphics.lib ^
  sfml-system.lib ^
  sfml-audio.lib

set INCS=^
  /I%DEP_INC%

set DEP_SRCS=

set SRCS=%DEP_SRCS% ^
  src\*.cpp

set FOLDERS=^
  assets

set FILES=^
  %SFML_LIB%\*.dll

set OUT_DIR=build
set OBJ_DIR=%OUT_DIR%\obj
set SYM_DIR=%OUT_DIR%\sym

REM timer
for /f "tokens=*" %%a in ('.\scripts\timeutil.py') do set LAST_TIME=%%a

rmdir /s /q %OUT_DIR%

mkdir %OUT_DIR%
mkdir %OBJ_DIR%

for %%I in (%FOLDERS%) do xcopy /sei %%I %OUT_DIR%\%%I > nul
for %%I in (%FILES%) do xcopy %%I %OUT_DIR% > nul

REM timer
for /f "tokens=*" %%a in ('.\scripts\timeutil.py %LAST_TIME%') do set COPY_DELTA_TIME=%%a
for /f "tokens=*" %%a in ('.\scripts\timeutil.py') do set LAST_TIME=%%a

rem cl /nologo %FLAGS% %INCS% %SRCS% %DEFS% /Fe%OUT_DIR%\%OUT_EXE%.exe /Fo%OBJ_DIR%\ /Fd%OUT_DIR%\ /link %LIBS%
cl /nologo /c %FLAGS% %INCS% %SRCS% %DEFS% /Fo%OBJ_DIR%\ /Fd%OUT_DIR%\

REM timer
for /f "tokens=*" %%a in ('.\scripts\timeutil.py %LAST_TIME%') do set COMPILE_DELTA_TIME=%%a
for /f "tokens=*" %%a in ('.\scripts\timeutil.py') do set LAST_TIME=%%a

link %OBJ_DIR%/*.obj %LIBS% /OUT:%OUT_DIR%\%OUT_EXE%.exe /INCREMENTAL:NO /subsystem:console /CGTHREADS:8 /NOLOGO %LD_FLAGS%

REM timer
for /f "tokens=*" %%a in ('.\scripts\timeutil.py %LAST_TIME%') do set LINK_DELTA_TIME=%%a

echo[
echo [Stats]
echo File copy:   %COPY_DELTA_TIME%
echo Compilation: %COMPILE_DELTA_TIME%
echo Linking:     %LINK_DELTA_TIME%
echo[

pushd %OUT_DIR%
echo [Execution]
.\%OUT_EXE%.exe
popd
