# Linux Command "show" to print lines

## Table of contents
* [About](#About)
* [Setup](#setup)
* [Synopsis](#Synopsis)
* [Examples](#Examples)
* [Credits](#Credits)


## About
**show** is a Linux command that presents information line by line (usually 20 lines iteration) from a command output. It also presents any number of lines using option with a number preceded by "-". It requires minimum 2 arguments to successfully run the command. A man-page is also included as a man1 (regular user command [^1]) entry.

[^1]: [What do the numbers in a man page mean?](https://unix.stackexchange.com/questions/3586/what-do-the-numbers-in-a-man-page-mean)

## Setup
Run `install.sh` to use **show** as a command.

Setup includes copying the command in `\bin` and a man-page in `/usr/share/man/man1` directory.

If not executable, run `chmod +x install.sh`.

If you don't want to use as a command, then run in the same directory using `./show`.

If `./show` doesn't work, compile the program again using `gcc show.c -o show`

## Synopsis
show [OPTION]... [COMMAND]... [COMMAND FLAG/FILE]...

## Examples
### Without flag
* show ls -al
* show cat /etc/group

### With flag
* show -10 ls -alg
* show -5 cat /etc/passwd
* show -2 tail /etc/passwd
* show -2 head -n 15 /etc/passwd

### Exception
* show 4 ls -al         [output: Bad Flag, see man page]
* show -3 lssss -a      [output: bad command, see man page]

### Help using man
* man show

## Credits
* [Stack Overflow](https://stackoverflow.com/)
* [Github](https://github.com)
* Other random tutorials
