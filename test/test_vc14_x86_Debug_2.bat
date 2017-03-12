%~d0
cd %~dp0

dir .. | ..\bin\vc14\x86\Debug\ctrc -v -t="yellow~<DIR>" -t=cyan~.md -t=magenta~bytes

pause