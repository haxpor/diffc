# diffp4

Perforce's `p4` tool has no built-in support for its diff output.
So `diffp4` is created to solve that problem.

PS. I'm not interested in solving this problem with `P4DIFF` which *might* be able
to solve the problem.

# Build

## Linux/Unix

* `make`
* `sudo make install`

## Windows

* `make.bat`

> If you use WSL, then you can do that via `cmd.exe /c make.bat` but you need to make sure you are in Visual Studio environment in order for `cl.exe` to be picked up.

# Usage

The program itself reads the input from the standard input. So you send input
either via `|` (pipe), or reading from file.

* `p4 diff | diffp4 -t perforce`
* `p4 diff -du | diffp4 -t perforce`
* `p4 describe -S 1234567 | diffp4 -t perforce`
* `diffp4 < /somedir/anotherdir/p4_diff.diff`

# Support

## Perforce

* `p4 diff` with or without `-du`
* `p4 describe`

# License
MIT, Wasin Thonkaew
