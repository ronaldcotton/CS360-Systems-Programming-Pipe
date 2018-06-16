# WSU Vancouver - CS360 - Signals and Pipes
## Systems Programming - Assignment 4
## C/Network Program
### Abstract
Creating a pipe with connect, this program demonstrates the use of pipes, forks, signals, and kills.  This takes the stdout of the first program and redirects it to the stdout of the second program.  Piping is done with the colon character.

### Requirements
*GNU C and GNU make*

### Executing
```
    make
    ./connect <program1> : <program2>
```
*to clean the project when done:*
```sh
    make clean
```
