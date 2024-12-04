# Snake Game in C++
Terminal UI based snake game in C++ using ncurses

# Build

## build with cmake

change directory to root directory i.e. SnakeGame

```Shell
~$ cmake --build build
```

once, cmake is done and build directory created under root directory

```Shell
~$ cd build
~$ make 
```

you can also use, DEBUG flag for enabling debug logs namely `game.log`

```Shell
~$ make DEBUG=1
```

once make is done, `snake` executable will be created in build directory. Simply run it!

```
~$ ./snake
```
