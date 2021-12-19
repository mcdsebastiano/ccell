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
| Ctrl+G | Goto Cell* |
| Ctrl+E | Execute Command* |
| Ctrl+D | Deselect current selection |
| Arrow Keys | Move Cursor | 
| Ctrl + Arrow Keys | Modify Selection |

* * Not yet implemented

**Currently only supported on Windows.**

#### Instructions
1. Clone repo
```
git clone https://github.com/mcdsebastiano/ccell.git
```
2. Change into ccell direction & run the cl command.
```
cd ccell
cl /Feccell.exe /std:c11 src/*.c 
```
3. Run the program
```
ccell.exe
```


| TODO: |
|----|
| 1. Save/Open CSV File. |
| 2. Implement spreadsheet logic. |

#### References:
* [MSDN Console Virtual Terminal Sequences](https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences)
* [kilo](https://github.com/snaptoken/kilo-src)

