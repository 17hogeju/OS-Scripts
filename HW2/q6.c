/*
Write a multithreaded program that calculates various statistical values for a
list of numbers. This program will be passed a series of numbers on the command
line and will then create three separate worker threads. One thread will
determine the average of the numbers, the second will determine the maximum
value, and the third will determine the minimum value. For example, suppose 
your program is passed the integers
90 81 78 95 79 72 85
The program will report
The average value is 82
The minimum value is 72
The maximum value is 95
The variables representing the average, minimum, and maximum values will be
stored globally. The worker threads will set these values, and the parent
thread will output the values once the workers have exited.
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
#define BUFF_MAX 15

typedef struct {
    int *array;
    size_t used;
    size_t size;
} Array;

float avg;
int maximum;
int minimum;

void initArray(Array *a, size_t initial_size) {
    a->array = malloc(initial_size * sizeof(int));
    a->used = 0;
    a->size = initial_size;
}

void insertArray(Array *a, int element) {
    if (a->used == a->size) {
        a->size += 1;
        a->array = realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used] = element;
    a->used += 1;
}

void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->used = 0;
    a->size = 0;
}

void *averageValue(void *arg) {
    Array *a = arg;
    int sum = 0;
    if (a->used == 1) {
        sum = a->array[0];
    } else {
        for(int i = 0; i < a->used; i++) {
            sum += a->array[i];
        }
    }
    avg = sum/(float)a->used;
}

void *maximumValue(void *arg) {
    Array *a = (Array*)arg;
    maximum = a->array[0];

    for(int i = 1; i < a->used; i++) {
        if (a->array[i] > maximum) {
            maximum = a->array[i];
        }
    }
}

void *minimumValue(void *arg) {
    Array *a = (Array*)arg;
    minimum = a->array[0];

    for(int i = 1; i < a->used; i++) {
        if (a->array[i] < minimum) {
            minimum = a->array[i];
        }
    }
}

typedef void (*func)(void *);

int main(int argc, char *argv[]) {
    Array a;
    
    static void * (*func[3])(void *) = {averageValue, maximumValue, minimumValue};
    
    // If arguments have been supplied, add them to Array, a
    if (argc > 1) {
        initArray(&a, argc);
        for (int i = 1; i < argc; i++) {
            insertArray(&a, atoi(argv[i]));
        }

        printf("Arguments entered: ");
        for (int i = 0; i < a.used; i++){
            printf("%d ", a.array[i]);
        }
        printf("\n");
    } else {
        printf("You did not supply any arguments. At least one argument is "
               "expected. \n");
        exit(-1);
    }

    pthread_t threads[NUM_THREADS];
    int rcs1[NUM_THREADS];
    int rcs2[NUM_THREADS];

    // Create the three separate worker threads
    for (int i = 0; i < NUM_THREADS; i++){
        rcs1[i] = 0;
        rcs1[i] = pthread_create(&threads[i], NULL, func[i], &a);
        if (rcs1[i] != 0) {
            printf("Error: Unable to create thread, %d\n", rcs1[i]);
            exit(-1);
        }
    }

    for(int i = 0; i < NUM_THREADS; i++){
        rcs2[i] = 0;
        rcs2[i] = pthread_join(threads[i], NULL);
        if (rcs2[i]) {
            printf("Error: Unable to join thread, %d\n", rcs2[i]);
            exit(-1);
        }
    }

    freeArray(&a);

    printf("The average value is %f\n", avg);
    printf("The maximum value is %d\n", maximum);
    printf("The minimum value is %d\n", minimum);

    return 0;
}
