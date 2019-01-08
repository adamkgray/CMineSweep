# Mine Sweeper

Command line version of minesweeper written in C

### About

* The minefield is an array of 8-bit integers. Each int represents a mine, where the value (empty, mine, or # adjacent) is stored in the lower bits, and the status (covered, uncovered, flagged) is stored in the upper bits.
* The worst case runtime for any routine in this program is O(N)
* Keyboard input is implemented with a switch statement - which is compiled as a GOTO - allowing for O(1) time handling of controls.

### Executable

To compile, run `make` in the source directory. It will produce an executable called `minesweeper`

To run, do `./minesweeper`
