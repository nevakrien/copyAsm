#include <stdint.h>
#include <x86intrin.h>
#include <string.h>
#include <assert.h>
#include<stdio.h>
#include <math.h>

const int  SIZE = 1000; 
#define TIMES  10000

static inline uint64_t read_time(void) {
    unsigned int aux;
    return __rdtscp(&aux);
}

static inline void uint64_t_move(uint64_t* input,uint64_t* output, int size){
    memcpy(output, input, size * sizeof(uint64_t));
}   

static inline void movsq_copy(uint64_t* input,uint64_t* output, int size){
	 __asm__ (
        "rep movsq"

        :
        : "D" (output), "S" (input), "c" (size)
        : "memory"
    );
}

static inline void four_copy_ymm(uint64_t* input, uint64_t* output, int size) {
    __asm__ (
        "1:\n"
        "vmovdqa (%[input]), %%ymm0\n"
        "vmovdqa %%ymm0, (%[output])\n"
        "add $32, %[input]\n"
        "add $32, %[output]\n"
        "sub $1, %[size]\n"
        "jnz 1b\n"
        :
        : [input] "r"(input), [output] "r"(output), [size] "r"(size/4)
        : "memory", "ymm0"
    );
}

#define CHECK_FUNC(func) \
    output = malloc(SIZE*sizeof(uint64_t));\
    if(!output){printf("OOM\n"); return 1;}\
    func(input,output,SIZE);\
    assert(memcmp(input,output,SIZE* sizeof(uint64_t)   )==0);\
    free(output); \

#define SET_TIME() 
    uint64_t times[TIMES];\
    uint64_t sum = 0; \
    double mean = 0.0, stddev = 0.0; \

#define TIME_FUNC(func, arr) \
    sum = 0;  mean = 0.0; stddev = 0.0; \
    for(int i = 0; i < TIMES; i++) { \
        uint64_t start = read_time(); \
        CHECK_FUNC(func) \
        arr[i] = read_time() - start; \
        sum += arr[i]; \
    } \
    mean = sum / (double)TIMES; \
    for(int i = 0; i < TIMES; i++) { \
        stddev += ((double)arr[i] - mean) * ((double)arr[i] - mean); \
    } \
    stddev = sqrt(stddev / (double)TIMES); \
    printf("Function: %s, Mean: %f, Stddev: %f\n", #func, mean, stddev);

int main(void) {
    uint64_t* input = malloc(SIZE * sizeof(uint64_t));
    if(!input) {
        printf("OOM\n");
        return 1;
    }

    for (uint64_t i = 0; i < SIZE; i++) {
        input[i] = i;
    }

    uint64_t* output = malloc(SIZE * sizeof(uint64_t));
    if(!output) {
        printf("OOM\n");
        free(input);
        return 1;
    }

    
    CHECK_FUNC(uint64_t_move)
    CHECK_FUNC(movsq_copy)
    CHECK_FUNC(four_copy_ymm)
    printf("pass\n\n" );

    SET_TIME()
    TIME_FUNC(uint64_t_move,times)
    TIME_FUNC(movsq_copy,times)
    TIME_FUNC(four_copy_ymm,times)

    free(input);
    printf("done\n" );
    
    return 0;
}