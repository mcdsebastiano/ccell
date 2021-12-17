# CCell Command Line Spreadsheet

Note: Currently only operates as a spreadsheet UI. 
The function logic is not yet implemented

----
| Command | Description |
| ---- | ---- |
| Ctrl+Q | Quit Application |
| Ctrl+O | Open CSV File* |
| Ctrl+S | Save Current Session* | 
| Ctrl+N | New Session* | 
| Ctrl+D | Deselect current selection |
| Arrow Keys | Move Cursor | 
| Ctrl + Arrow Keys | Modify Selection |
| Ctrl + ] | Scroll Viewport Left* |  
| Ctrl + [ | Scroll Viewport Right* |

* Not yet implemented

**Currently only supported on Windows.**

#### Instructions
1. Clone repo
```
git clone https://github.com/mcdsebastiano/ccell.git
```
2. Change into ccell direction & run the cl command.
```
cd ccell
cl /Feccell.exe /std:c11 *.c 
```
3. Run the program
```
ccell.exe
```


| TODO: |
|----|
| 1. Save/Open CSV File. |
| 2. Implement spreadsheet logic. |



