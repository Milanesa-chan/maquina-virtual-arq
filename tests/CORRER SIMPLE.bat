@echo off
echo TRADUCCION
echo ==========
mvc.exe assembler.asm codigo.img
echo.
echo ====================
echo TRADUCCION TERMINADA
echo EJECUCION
echo ====================
mvx.exe codigo.img
:mvx.exe codigo.img -d -b -c -a
echo.
pause