# Triangles
The task requires to output the numbers of intersected triangles received from stdin. To do it faster then with naive implementation with O(n^2) I use octree to divide space into 8 parts.

## Requirements 

The following applications have to be installed:

1. CMake 3.2 version (or higher)
2. GTest
3. c++ compiliter
4. Vulkan-Hpp
5. glslc compiller for shaders


## Compiling 

To compile each of the cache you need to use сmake in the directory build:

``` cmd
$ mkdir build
$ cd build
$ сmake ..
$ cmake --build
```

## Run the program:

You can find all binaries in dir build/bin


Main programm: 
``` cmd
$ ./bin/triangles
```
Test for triangles:

``` cmd
$ ./bin/testTriangle
```

## Algorithm

![](https://sun9-9.userapi.com/impg/eofJ6skc56YsE9eMaiW37jIqbR--eJE8oEYz_A/CyJQQM4wPmg.jpg?size=965x640&quality=96&sign=563de9d10b097abaf173844bd4749505&type=album)

## 3D Visualisation with Vulkan-Hpp

I used Vulkan-Hpp to visualize the intersection. The red triangles are the intersection, the rest are blue. You can use the standard W/A/S/D and E/Q to move the camera up and down, and the mouse or arrows to review. Also the O/P keys allow you to change the angle of your own rotation, and holding SHIFT allows you to use acceleration. 

![](https://sun9-74.userapi.com/impg/vGBLS0-YcvE2pEGZcDnZYroZ6AOzCxHf575YCQ/M_sJ3IxEj6s.jpg?size=986x993&quality=96&sign=6dd015feb7668ea3e5983a9b492884c0&type=album)

to view the text example, use
``` cmd
$ ./bin/triangles < ./bin/10000.1.txt
```

## Useful materials:
* **Geometric Tools for Computer Graphics** by Philip Schneider and David Eberly [2003]