# STC-DRIFT-TIME
Finding the velocity of the drift electrons in the STC for the ALEPH experiment.

All c++ code is located in `src/`, `py-test/` contains jupyter notebooks and python code that I used for testing ideas. No plots here where used in my final results, all of these came from the c++ code.

## Build C++
Prefered:
```make```
or alternitvely
``` g++ src/main.cpp src/event.cpp src/event-helpers.cpp -o STC -std=c++17 -Wall `root-config --cflags` `root-config --libs` ```

## Build LaTeX
``` source build-TeX.sh ```




## Notes (temp)
Use paralisation?
Check secant method

https://cp-algorithms.com/geometry/tangents-to-two-circles.html
https://math.stackexchange.com/questions/1481904/distance-between-line-and-circle#


https://en.wikipedia.org/wiki/Powell%27s_method <- optimisation method uses no derivatives

https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.minimize_scalar.html#scipy.optimize.minimize_scalar 