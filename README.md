# Ray Tracer

Assignment 3 for *Intros to Computer Graphics*

Based on "Skeleton Code" (the first submit 73aa6c449734ae26ad8a92af5e2c7d49fa88f15b).

***Author: Danyang Song (Arthur)***

*For more details about this project, see https://github.com/GreysTone/SFU-CMPT-000-ASGN-2.*

***All steps (all functions) are implemented in the assignment. Extra information is shown as below.***

***Bonus completed.***

******

## Compilation, Deployment & OS Environment
* Linux (**tested on Ubuntu 14.04.03 LTS in VMware 8**)
    * Dependency: glew, freeglut3, mesa (possibly others needed)
    * In an initial environment
        ~~~
        make && ./raychess [-u | -d | **bonus** | **fbonus**] step_max
        ~~~
    * Otherwise
        ~~~
        make clean
        make && ./raychess [-u | -d | **bonus** | **fbonus**] step_max <options>
        ~~~
* Option List
   * **+s** inclusion of shadows;
   * **+l** inclusion of reflections;
   * **+r** inclusion of refractions;
   * **+c** inclusion of chess board pattern;
   * **+f** enabling diffuse rendering using stochastic ray generations;
   * **+p** enabling super-sampling.
 
## Preview
* Default scene (./raychess -d 5)

  <img src="default.bmp" />

* All options enabled scene (./raychess -u 5 +s +l +r +f +c +p)

  <img src="mine.bmp" />

* Bonus(Accelerated) scene (./raychess fbonus 5)

  <img src="chess_scene.bmp" />

## Acceleration in Bonus Implementation
* Speed comparison (option 'bonus 5' & 'fbonus 5')
  * without any accelerations: **more than 1 hour**
  * with all accelerations activated: **less than 3 minutes**


* **Parallel programming** with *OpenMP*
  * *OpenMP* enables multithreaded scene processing. 4 threads are created to process the whole image. Each thread takes one-quarter of the workloads.
* **Object Boundary Box** accelerations
  * The virtual object **GT_Boundary** is loaded into the scene. It binds the chess and implements the interface for intersection and refraction. Internally, it contains an OctTree.
* A simple **OctTree** implementation
  * A simple OctTree separates the boundary box into 8 sub-areas in order to reduce the number of triangle meshes during the intersection.
* **Object Internal** intersection in refraction
  * The refraction process takes only the object itself into account, which reduces calculation between the ray and others.

## Features
* ***Parallel*** (with **'-fopenmp'** compile option)
* ***Object Boundary Box***
* Simple ***OctTree***
* ***STL*** in C++11 (with **'-std'** compile option)
* Using **Normal Distribution** in stochastic diffuse
* **'-g'** compile option for debugging
* **'-pg'** compile option for profiling
* **Polymorphism**

## Options Description
* Render bonus part using:
   ~~~
   ./raychess bonus step_max      # normal bonus rendering
   ./raychess fbonus step_max     # active all accelerations
   ~~~
* Since I enable the parallel features, the statistic number of intersection is ignored, while there is also an approximate number in **profiling report** (shown as below).
* **'bonus'** or **'fbonus'** will automatically tigger *shadow*, *refraction*, *recursive reflection*, *chessboard*, *stochastic diffuse* and *antialias*

## Profiling
* Valgrind(**callgrind.out.[id]**) & gprof(**gmon.out**)
* Report for no acceleration:

<img src="report.png" />

## MACRO Description
* global.h
  * **OCT_OUTPUT** shows OctTree's basic information
  * **OCT_REDUCE_MEM** reduces the cost of memory of building OctTree
  * **SHOW_PROGRESS** shows the progress on different threads
  * **GT_PARALLEL** enables parallel features


## Some Comments
* Parallel computation is not balanced, due to different scene data
* OctTree is only implemented for 1 level, but it still accelerates the rendering a lot
