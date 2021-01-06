+++
date = "2016-11-05T21:05:33+05:30"
title = "About"
+++

Sharpened dimensionality reduction (SDR) sharpens the original data before dimensionality reduction to create visually segregrated sample clusters, which is targeted for user-guided labeling. 
<!--
![Console](https://github.com/mrmierzejewski/hugo-theme-console/blob/master/images/preview.png?raw=true)
-->

## Abstract
One of the largest challenges of using dimensionality reduction (DR) on large, high-dimensional data sets is to distinguish the underlying high-dimensional data clusters in a 2D projection. We address this problem by first sharpening the clusters in the data prior to the DR step using Local Gradient Clustering (LGC). We then project the sharpened data by a user-selected DR method. The sharpening step helps this method to preserve cluster separation more easily in the resulting 2D projection. With our method, end-users can label each distinct cluster to further analyze an otherwise unlabeled data set. Our `High-Dimensional Sharpened DR' (HD-SDR) method, tested on both synthetic and real-world data sets, is favorable to DR methods with poor cluster separation and yields a better visual cluster separation than these DR methods with no sharpening. We show that our method achieves good quality (measured by quality metrics) and scales computationally well with large high-dimensional data. We further apply HD-SDR on a recent astronomical catalog.

## Code
Code is written in C++ and is available [here](https://github.com/youngjookim/fdr).
```
https://github.com/youngjookim/fdr
```
\
The filter is implemented in function 'filter_for_dr' in main.cpp.

To get the code running please include the following:\
a. [Nanoflann](https://github.com/jlblancoc/nanoflann) header file\
b. [Eigen 3.3.3](http://eigen.tuxfamily.org/)\
c. [Tapkee dimension reduction library](http://tapkee.lisitsyn.me/)

<!--
See the [Hugo documentation](https://gohugo.io/themes/installing/) for more information.

## Configuration

Set theme parameter in your config file:

```
theme = "hugo-theme-console"
```

## License

Copyright © 2020 [Marcin Mierzejewski](https://mrmierzejewski.com/)

The theme is released under the MIT License. Check the [original theme license](https://github.com/panr/hugo-theme-terminal/blob/master/LICENSE.md) for additional licensing information.
-->