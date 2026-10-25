#include <stdio.h>
#include <string.h>

/* Function declarations */
void qsort_generic(void *v[], int left, int right, int (*comp)(void *, void *));
void swap(void *v[], int i, int j);

int main(void) 
{
    /* Array of generic pointers pointing to strings */
    char *planets[] = {
        "Mercury", "Venus", "Earth", "Mars", 
        "Jupiter", "Saturn", "Uranus", "Neptune"
    };
    int size = sizeof(planets) / sizeof(planets[0]);
    int i;

    printf("Original array:\n");
    for (i = 0; i < size; i++) {
        printf("%s ", planets[i]);
    }
    printf("\n\n");

    /* 
       Cast 'planets' to (void **) and pass the standard library 'strcmp' 
       as our comparison function pointer matching: int (*comp)(void *, void *)
    */
    qsort_generic((void **) planets, 0, size - 1, (int (*)(void *, void *)) strcmp);       // note how this is being written

    printf("Lexicographically sorted array:\n");
    for (i = 0; i < size; i++) {
        printf("%s ", planets[i]);
    }
    printf("\n");

    return 0;
}

/* qsort_generic: sort v[left]...v[right] into increasing order */
void qsort_generic(void *v[], int left, int right, int (*comp)(void *, void *))
{
    int i, last;

    if (left >= right)              /* Do nothing if array contains */
        return;                     /* fewer than two elements */

    swap(v, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++) {
        /* Call the function pointer 'comp' to compare arbitrary pointers genericly */
        if ((*comp)(v[i], v[left]) < 0) {
            swap(v, ++last, i);
        }
    }

    swap(v, left, last);
    qsort_generic(v, left, last - 1, comp);
    qsort_generic(v, last + 1, right, comp);
}

/* swap: interchange two generic pointers in the pointer array */
void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
