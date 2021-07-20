# High-dimensional Sharpened Dimensionality Reduction (HD-SDR)
## Table of contents
* [About](#about)
* [Setup](#setup)
* [Experimental results](#experimental-results)
* [Supplemental materials](#supplemental-materials)


## About
Sharpened dimensionality reduction (SDR) sharpens the original data before dimensionality reduction to create visually segregrated sample clusters, which is targeted for user-guided labeling. 


## Setup
Code is written in C++ and is available [here](https://github.com/youngjookim/sdr).
```
https://github.com/youngjookim/sdr
```
\
To get the code running please include the following:\
a. [Nanoflann](https://github.com/jlblancoc/nanoflann) header file\
b. [Eigen 3.3.9](http://eigen.tuxfamily.org/) cmake required\
c. [Tapkee dimension reduction library](http://tapkee.lisitsyn.me/)

The sharpening filter is implemented in function 'filter_for_dr' in main.cpp.

### Build SDR on Linux

```
cd Code/LGCDR_v1/
mkdir build && cd build
cmake ..
```

## Experimental results
All the experimental results for this project are found [here](https://youngjookim.github.io/sdr/experiments/)


## Supplemental materials
Supplemental materials are found [here](https://youngjookim.github.io/sdr/supplemental_materials/)
