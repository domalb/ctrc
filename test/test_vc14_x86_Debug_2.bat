%~d0
cd %~dp0

dir .. | ..\bin\vc14\x86\Debug\ctrc -c="yellow~<DIR>" -c=cyan~.md -c=magenta~bytes

pause