# TerminalFileExplorer
This application is a terminal-based file explorer. It is implemented strictly using low level system calls (POSIX standard calls). No wrapper functions used. The applications considers directory in which it is run as the root directory. The application operates in 2 modes - normal mode and command mode. In normal mode the user can nagitave the directories through arrows keys and enter key. In command mode, the user can type commands and perform operations like move, copy, create, delete and rename (file as well as directories).

## Normal mode
Traverse using arrow keys and enter.
Can open files too. Default application based on the file will launch
H for home and backspace to go back to previously visited directory

## Command mode 
Copy ‘:copy <source_file(s)> <destination_directory>’ 

Move ‘:move <source_file(s)> <destination_directory>’ 

Rename ‘:rename <old_filename> <new_filename>’

Create file ‘:create_file <file_name> <destination_path>’

Create directory ‘:create_dir <dir_name> <destination_path>’

delete_file file_path

goto path

snapshot folder
 
From normal mode, press : to enter to command mode. Esc to exit command mode.
  
  



