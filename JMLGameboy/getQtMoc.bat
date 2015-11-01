@ECHO OFF
FOR /R %%f in (.\src\Tools\Debugger\UI\*) DO moc.exe %%f -o MocFiles\moc_%%~nf.cpp