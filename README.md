# CMineSweep

Command line version of minesweeper written in C

### About

* The minefield is an array of 8-bit integers. Each int represents a mine, where the value (empty, mine, or # adjacent) is stored in the lower bits, and the status (covered, uncovered, flagged) is stored in the upper bits.

### Executable

To compile, run `make` in the source directory. It will produce an executable called `ms`

To run, do `./ms`

### Possible future features
* show number of moves made so far in status bar
* ask if the user wants to play again after game finishes (Play again? [Y/n])
* enable variable mine density through command line args (--high-density | --low-density | --medium-density)
