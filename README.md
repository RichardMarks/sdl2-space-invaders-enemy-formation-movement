# README

This is a small example of how you can implement the enemy formation and movement from space invaders.

## Architecture

A very simple approach has been taken with an `Invader` class which implements the logic for a single enemy, and an `Invaders` class which implements the main functionality of the formation movement.

The demo entry point is the `main` function which has a basic SDL2 boilerplate setup.

You can click on an invader to kill it.

## Build and Run

On macOS with SDL2 installed using Homebrew, you can compile the code and run it using the following command:

```
clang++ -std=c++17 $(sdl2-config --cflags --libs) src/main.cpp -o gamebin && ./gamebin
```
