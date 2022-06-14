# gbOS
it's an operating system for the gameboy

import the .gb file on your personal gameboyTM device (lol emulation)
The rest is the main c code for gbOS (large_map.c), the tile map (map_map.c and map_map.h), and the tiles (map_tiles.c and map_tiles.h), along with the makefile and compile.bat so that you can check those if you want.
You can modify the tiles using the gbSpriter tools I also put on github https://github.com/qw33e/gbspriter

For now there isn't much but-

Move the mouse with the arrow keys, and click with a

Click on "code" on the bottom right to start coding. It's in brainfuck, so if you don't know how that works: https://esolangs.org/wiki/Brainfuck...  yeah it's not exactly a very serious language, but it is suited for this project and funny as shit and I love it
Coding works by pressing either a or b along with a directional key

a:
up +
down -
left <
right >

b:
up ,
down .
left [
right ]

Press enter to run. If you get a _ you can input a number by pressing
left -1
right +1
down -10
up +10
and subsequently pressing enter when satisfied.
As for editing... most emulators should have a rewind feature, that's the best I got. Yeah I'm sorry, I'm sorry and all. I do plan on adding saving in the future, along with other neat features to make it more like a normal operating system

Thanks
Copyright is whatever as usual
