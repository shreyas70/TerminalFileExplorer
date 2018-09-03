# TerminalFileExplorer
The objective is to develop terminal application to explore files in the system, without using any wrapper libraries.


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
delete_file <file_path>
snapshot <folder> <dumpfile>
Goto <path>
  
  
From normal mode, press : to enter to command mode. Esc to exit command mode.
  
  



