# Cat With An Attitude

A small C program that writes text files on screen.
Powered by the [z_](https://github.com/zakarouf/z_) library.

## Installation

First you'll to install the [z_](https://github.com/zakarouf/z_) library.
Then Just compile the `cwaa.c` file:
```sh
gcc -std=c99 -Wall -I/usr/local/include/ -L/usr/local/lib/ -lzkcollection cwaa.c -o cwaa
```
> libzkcollection is the name for the `z_` library.

## Usage

Cwaa Take one argument; The file-name. Thats it.
```c
./cwaa cwaa.c
```
Will read-out the file `cwaa.c`

---
