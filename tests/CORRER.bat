@echo off
echo TRADUCCION
echo ==========
::mvc.exe 1-aritmetica.asm aritmetica.img
mvc.exe assembler1.asm codigo1.img
echo.
mvc.exe assembler2.asm codigo2.img
echo.
mvc.exe assembler3.asm codigo3.img
echo.
echo ====================
echo TRADUCCION TERMINADA
echo EJECUCION
echo ====================
::mvx.exe aritmetica.img -d
mvx.exe codigo1.img codigo2.img codigo3.img
echo.
pause