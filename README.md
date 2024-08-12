# copyAsm
benchmarking copy operation in raw assembly

# results

VERY weird results... 
at first ymm has a clear advantage. 

then around 1,000/10,000 its even maybe movsq have the advantage.
this seems to depend on the compiler??? very very weird.

after that we have it bounce back to almost even in a milion??? 
and the macro is also just straight up slower. (this also happens when we JUST the macro on a milion)
so seems that gcc and clang write suboptimal code for memcpy over a 8 milion bytes.

``` bash
(base) user@user-System-Product-Name:~/Desktop/assembly/copyAsm$ clang main.c -O3 -lm && ./a.out 
pass
size is 20

Function: uint64_t_move, Mean: 43.465200, Stddev: 4.956409
Function: movsq_copy, Mean: 60.688000, Stddev: 6.579746
Function: four_copy_ymm, Mean: 42.037400, Stddev: 4.528112
Function: macro_move, Mean: 48.854200, Stddev: 545.339417
done
pass
size is 1000

Function: uint64_t_move, Mean: 432.401000, Stddev: 493.065331
Function: movsq_copy, Mean: 445.770000, Stddev: 929.333412
Function: four_copy_ymm, Mean: 469.325400, Stddev: 865.315938
Function: macro_move, Mean: 432.175600, Stddev: 500.986472
done
pass
size is 10000

Function: uint64_t_move, Mean: 6655.022600, Stddev: 886.509472
Function: movsq_copy, Mean: 7093.549600, Stddev: 554.495178
Function: four_copy_ymm, Mean: 6993.212600, Stddev: 349.750747
Function: macro_move, Mean: 6909.765800, Stddev: 673.898129
done
pass
size is 100000

Function: uint64_t_move, Mean: 62210.530000, Stddev: 3616.429999
Function: movsq_copy, Mean: 61734.429000, Stddev: 2802.724694
Function: four_copy_ymm, Mean: 74542.392600, Stddev: 2685.091711
Function: macro_move, Mean: 63739.978800, Stddev: 5514.991995
done
pass
size is 1000000

Function: uint64_t_move, Mean: 1649607.202800, Stddev: 42166.321897
Function: movsq_copy, Mean: 1096197.004400, Stddev: 40797.295718
Function: four_copy_ymm, Mean: 1115785.862000, Stddev: 46435.857245
Function: macro_move, Mean: 1648043.325400, Stddev: 41801.583073
done

```


```bash
(base) user@user-System-Product-Name:~/Desktop/assembly/copyAsm$ gcc main.c -O3 -lm && ./a.out 
pass
size is 20

Function: uint64_t_move, Mean: 44.189400, Stddev: 4.637384
Function: movsq_copy, Mean: 80.168800, Stddev: 2017.024086
Function: four_copy_ymm, Mean: 40.431800, Stddev: 11.555966
Function: macro_move, Mean: 43.980600, Stddev: 129.069646
done
pass
size is 1000

Function: uint64_t_move, Mean: 367.226600, Stddev: 43.359177
Function: movsq_copy, Mean: 371.454400, Stddev: 44.129454
Function: four_copy_ymm, Mean: 374.256200, Stddev: 116.867109
Function: macro_move, Mean: 368.226400, Stddev: 24.066640
done
pass
size is 10000

Function: uint64_t_move, Mean: 6169.755200, Stddev: 688.001633
Function: movsq_copy, Mean: 6187.433200, Stddev: 152.241165
Function: four_copy_ymm, Mean: 6153.091200, Stddev: 217.196931
Function: macro_move, Mean: 6199.430200, Stddev: 208.986981
done
pass
size is 100000

Function: uint64_t_move, Mean: 71294.647800, Stddev: 2485.804377
Function: movsq_copy, Mean: 70526.168400, Stddev: 3050.915560
Function: four_copy_ymm, Mean: 72660.946400, Stddev: 2992.314802
Function: macro_move, Mean: 70923.789000, Stddev: 2599.480785
done
pass
size is 1000000

Function: uint64_t_move, Mean: 1694229.001000, Stddev: 45454.284730
Function: movsq_copy, Mean: 1136299.442800, Stddev: 70326.145586
Function: four_copy_ymm, Mean: 1123054.094600, Stddev: 55794.533532
Function: macro_move, Mean: 1716945.173200, Stddev: 78339.566210
done

```
