+++
date = "2021-01-01T21:05:33+05:30"
title = "About "
+++

## All contents from https://youngjookim.github.io/sdr/ were submitted to and accepted by the Information Visualization journal.

## High-dimensional Sharpened Dimensionality Reduction (HD-SDR)
High-dimensional Sharpened Dimensionality Reduction (HD-SDR) sharpens the original high-dimensional data before dimensionality reduction to create visually segregrated sample clusters, which is targeted for user-guided labeling.

### Setup
Code is written in C++ and is available [here](https://github.com/youngjookim/sdr).
```
https://github.com/youngjookim/sdr
```
\
To get the code running please include the following:\
a. [Nanoflann](https://github.com/jlblancoc/nanoflann) header file\
b. [Eigen 3.3.3](http://eigen.tuxfamily.org/)\
c. [Tapkee dimension reduction library](http://tapkee.lisitsyn.me/)

The sharpening filter is implemented in function 'sharpening_for_dr()' in main.cpp.

Input files should be located in the 'Data' folder and consist of one header file and one data file for each data set. Please run 'example_data_set_1'.

### Experimental results
All the experimental results for this project are found [here](https://youngjookim.github.io/sdr/).


### Supplemental materials
Supplemental materials are found [here](https://youngjookim.github.io/sdr/).

<!--
![Console](https://github.com/mrmierzejewski/hugo-theme-console/blob/master/images/preview.png?raw=true)
-->
<!--
See the [Hugo documentation](https://gohugo.io/themes/installing/) for more information.

### Configuration

Set theme parameter in your config file:

```
theme = "hugo-theme-console"
```

### License

Copyright © 2020 [Marcin Mierzejewski](https://mrmierzejewski.com/)

The theme is released under the MIT License. Check the [original theme license](https://github.com/panr/hugo-theme-terminal/blob/master/LICENSE.md) for additional licensing information.
-->