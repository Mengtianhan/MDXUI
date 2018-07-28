@echo off
rem
rem Register.bat
ren Register file for AsioNetCom.dll
rem
echo on
regsvr32  AsioNetCom.dll
regsvr32  AsioNetComd.dll

