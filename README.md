Masala Game Engine
==================
Masala is an experimental game engine written in C++(14), Qt 5.x and OpenGL.  My
goal was to see if I could make a cross platform game engine where the developer
used ChaiScript to write all of the game logic.

Unfortunately, I ran into performance issues when I tried to create mass amounts
of objects inside of ChaiScript, and determined that the current implementation
of the language is a bottleneck for what I was trying to make.  I've decided to
release it so that others may learn from what I have done here, or if anyone
else wants to explore making a game engine with ChaiScript.  It's not a bad
language in my opinion, and makes scripting C++ applications super simple.


Features
--------
It's fairly basic engine, but so you know what it offers:
 * 2D
 * Cross platform (Built on Linux, tested on Windows & OS X)
 * ChaiScript oriented game programming
 * Built on Qt & OpenGL
 * Featureful sprite object (with animations!)
 * Tilemaps (Flare maps)
 * XNA/MonoGame like scripting structure
 * Basic sound effects
 * Keyboard Input
 * Interactive debugging terminal (see `masala.ini` below)


Requirements
------------
This requires Qt's Multimedia package to be installed (for QSoundEffect).  If
your installtion of Qt doesn't have it, you'll need to install it.

This uses ChaiScript 5.8.0.  This version of ChaiScript can be found in the
`third_party` directory.  ChaiScript is statically compiled into the game
engine.  If you want to dynamically link it you will need to compile a `.so`
file of the library, modify `src/masala.pro` and `src/masalaapp.cpp`.  As well
as include the .so as the same directory as `masala.bin`

This project also requires GNU Readline.  Version 6.x was used to make this.
It is not included in this repository.


Building & Running
------------------
Change into the `src/` directory.  Type `qmake` then `make` an wait a bit.  To
play a game, just run the `masala.bin` executable`.   By default, it is setup to
play a game called `ping`.


`masala.ini`
------------
This is the configuration file for `masala.bin`.  Here are the important
options:

    `terminal` -> boolean; `true` to turn on the interactive debugging terminal,
                  which is just an interface to the embedded ChaiScript VM.
                  you can access objects that exist in the game.

    `script_dir_path` -> string; where the script files are located for the
                         game.  Needs a trailing slash.

    `media_dir_path` -> string; where the media files are located for the game.
                        Needs a trailing slash.


Samples
-------
You can find some example games in the `example_games` direction.  Drop the
contents of any of those folders into the `scripts` folder located in `src`.
See the above section (`masala.ini`) for how to change out games.

`ping` is the default loaded game.  It's a clone of a classic atari game.  Use
the `A` & `Z` keys for the left player, and `Up` & `Down` arrow keys for the
right side.

`grab_n_dodge` is a little more complex.  You play as a green arrow that goes
around trying to pick up purple coins, while dodging yellow enemies.  Use the
arrow keys to move around.  Press `R` to reset the game once you've died.


Documentation
-------------
Most of the engine should be documented in the `*.cpp` files of the `src/`
folder, where they explain how each function works.  You should be able to also
run Doxygen or QDoc on them.  For what is bound to Chaiscript, read through the
files `mchaiqt.h`, `mchaiqt.cpp`, `mchai.h`, and `mchai.cpp`; not everything is
bound.

For learning how to program games for Masala, it would be best to check out the
sample games and see how things are done.

As of right now, I don't have much interest in furthering this project, so I'm
not that inclined to write documentation for it.  If others want to, I'll be
glad to look over pull requests.  This goes too for other sample games.


License
-------
In a nutshell, everything in this project is GPLv3 (see `LICENSE` for more
details).  The exception though is for ChaiScript which is found in the
`third_party` directory; see it's `LICENSE` file for details.

