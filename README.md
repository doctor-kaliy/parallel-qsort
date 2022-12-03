# parallel-qsort

![image](https://user-images.githubusercontent.com/62459951/205453286-e1ef5a93-e94e-4a93-b79f-0873f6c7709d.png)

OpenCilk is used

It can be installed by executing shell script as desribed [here](https://www.opencilk.org/doc/users-guide/install/).

To compile:
``` /opt/opencilk/bin/clang -fopencilk -O3 qsort.c -o qsort ```

To launch:
``` CILK_NWORKERS=4 ./qsort ```
