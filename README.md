# WASH - Shell Assignment for CSC 3350 Operating Systems
This project is intended for extra credit in CSC 3350 Operating Systems at Seattle Pacific University.

Included is the Washington shell and a helper command, `head_new`.

## Install Instructions
### wash
```
make
sudo make install
```
or
```
gcc wash.c -o wash
```
### head_new
```
gcc head_new.c -o head_new
```

## Examples
### wash help message
```
[user@host ~]$ ./wash -h
The Washington Shell is a simple shell that supports the following commands:

        -exit                   exits the shell
        -echo [string]          prints the arguments to the screen
        -cd [dir]               changes the current directory
        -pwd                    prints the current working directory
        -setpath <path>         sets the path to look for executables
        -help                   prints this help message

```
