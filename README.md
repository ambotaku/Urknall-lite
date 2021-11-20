# Urknall-lite
Conway's Game of Life on a Raspberry Pi Pico with Oled Display [Pi Pico SDK Demonstration in C++]

This is mainly an example for using the Raspberry Pi Pico microcontroller with an attached SSD1306 controlled Oled-Display.
As hardware only a Pi Pico and an Oled display with 128 x 64 pixels are needed, the software is based on the Pico SDK, anything else is part of the example.

The code renders stages of a cellular automaton known as "Conway's Game of Life" using pixels as "cells". Starting from a random pattern you will see establishing stable and moving / "living" structures based on very simple rules.

I named it "Urknall" (big bang) since everything in nature (atoms, crystals, living cells, stars) need just some simple rules to grow to astonishing very complex and often expressive and beautiful stable structures - like the ornaments for this game.

I tried that with MicroPython first, but bit-wise graphics-handling gets to slow and memory consuming, since each pixel represents a living "cell or Python object which needs 28 bytes. 

The rules of the game are very simple:
- a cell dies if not having at least 2 neighbors
- a new cell is born when having exactly 3 neighbors
- a cell dies (is starving) having 4 or more (up to 8) neighbors

So the algorithm needs two 2-dimensial cell arrays where counting the neighbors of each in the first array will result in the pixels of the second array; after that those "frame buffers" get swapped and the result array will be transferred via i2c- interface to the Oled display.

Of course it would be stupid to use read-pixel functions and the pixels in such monochrome display are organized in 8-pixel groups (bytes) vertical or horizontal so you need a lot of shifting and modulo-calculations to build a framebuffer and a fast transfer of so organized buffers into the oled display.

So finally I coded that in C++. My first Micropython try needed more than 20 seconds for rendering one stage / generation, the C++ solution shown here did it in 88 ms per generation on a Raspberry Pi Pico, so i needed long sleep-times to watch the changing results.
