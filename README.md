# SDL2 Point Rendering Performance Demo

This project demonstrates the impact of draw-call optimization in SDL2. It compares a naive rendering approach against an optimized batch rendering method, visualizing the difference in performance when drawing thousands of points.

## Overview

- Normal Version `points.c`: Draws points individually using separate draw calls. This creates a bottleneck between the CPU and GPU.

![Falling Snow](images/snow.png)

- Optimized Version `pointsoptimized.c`: Batches point data to render everything in a single draw call. This significantly improves framerates and allows for a much higher particle count.

![Heavy Snow](images/heavy.png)

## Prerequisites

- GCC/Clang
- Make
- SDL2 library (libsdl2-dev)

## Build Instructions

You can compile the versions separately using the included makefile.
1. Standard Version
``` bash
make
./points
```

2. Optimized Version
``` bash
make optimized
./optimized
```

## Cleaning up
``` bash
make clean
```
