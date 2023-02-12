# Triangles
The task requires to output the numbers of intersected triangles received from stdin. To do it faster then with naive implementation with O(n^2) I use octree to divide space into 8 parts.

## Requirements 

The following applications have to be installed:

1. CMake 3.2 version (or higher)
2. GTest
3. g++


## Compiling 

To compile each of the cache you need to use сmake in the directory build:

``` cmd
$ mkdir build
$ cd build
$ сmake ..
$ make all
```

## Run the program:

You can find all binaries in dir build/bin


Main programm: 
``` cmd
$ ./bin/triangle
```
Test for triangles:

``` cmd
$ ./bin/perfect
```

## Algorithm

![](https://sun9-9.userapi.com/impg/eofJ6skc56YsE9eMaiW37jIqbR--eJE8oEYz_A/CyJQQM4wPmg.jpg?size=965x640&quality=96&sign=563de9d10b097abaf173844bd4749505&type=album)

## Useful materials:
* **Geometric Tools for Computer Graphics** by Philip Schneider and David Eberly [2003]