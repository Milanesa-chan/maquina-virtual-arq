@echo off
echo TRADUCCION
echo ==========
mvc.exe crearArbolEnES.asm codigo1.img
echo.
mvc.exe assembler.asm codigo2.img
mvc.exe assembler1.asm codigo3.img
::echo.
::mvc.exe assembler3.asm codigo3.img
echo.
echo ====================
echo TRADUCCION TERMINADA
echo EJECUCION
echo ====================
::mvx.exe aritmetica.img -d -a 
mvx.exe codigo1.img codigo2.img codigo3.img -b -d -a
echo.
pause