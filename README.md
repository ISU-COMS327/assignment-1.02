# Assignment 1.02: Dungeon Load/Save

## Description

In this project, we added the ability to load and save dungeons.
All dugneons will be saved in the file $HOME/.rlg327/dungeon. `dungeon`
is a binary file.

## Usage

* You can build this program by calling `make`.

* You can save a file by running `./generate_dungeon --save`

* You can load the file that exists in $HOME/.rlg327/dungeon by calling
`./generate_dungeon --load`

* If you want to load a dungeon and then re-write it, you may call
`./generate_dungeon --save --load`

* If you are generating a new dungeon, you may specify the number of rooms
by passing in the `--rooms` paramater like this: `./generate_dungeon --rooms=20`

* For usage information in your shell, you may call `./generate_dungeon --help`

