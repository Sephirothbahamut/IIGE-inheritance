FOR /d /r . %%d IN ("ipch") DO @IF EXIST "%%d" rd /s /q "%%d"
del /s /q *.obj
del /s /q *.pdb
del /s /q *.idb
del /s /q *.suo
del /s /q *.ncb
del /s /q *.sbr
del /s /q *.exe