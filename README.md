# STC-DRIFT-TIME
Finding the velocity of the drift electrons in the STC for the ALEPH experiment.

All c++ code is located in `src/`, `py-test/` contains Jupyter notebooks and python code that I used for testing ideas. No plots here where used in my final results, all of these came from the c++ code.

## Build C++
Preferred:
```
$ make
```
or alternately
``` 
$ g++ src/main.cpp src/event.cpp src/event-helpers.cpp -o STC -std=c++17 -Wall `root-config --cflags` `root-config --libs`
```

## Running
``` 
$ ./STC
```
or
```
$ ./STC 100000      # plot every 100k events with an event display
```

## Build LaTeX
``` source build-TeX.sh ```

