#include <stdio.h>

/* Function declarations */
void qsort(int v[], int left, int right);
void swap(int v[], int i, int j);

int main(void) 
{
  /* Test array with unsorted integers */
  int arr[] = {34, -10, 5, 0, 100, 2, 89, -5, 23};
  int size = sizeof(arr) / sizeof(arr[0]);
  int i;

  printf("Original array:\n");
  for (i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n\n");

  /* Call qsort: array indices range from 0 to size - 1 */
  qsort(arr, 0, size - 1);

  printf("Sorted array:\n");
  for (i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  return 0;
}

/* qsort: sort v[left]..v[right] into increasing order */
void qsort(int v[], int left, int right)
{
  int i, last;
  
  if (left >= right)                    /* do nothing if array contains */
    return;                             /* fewer than two elements */
  swap(v, left, (left + right) / 2);    /* move partition elem */
  last = left;                          /* to v[left] */
  for (i = left + 1; i <= right; i++)   /* partition */
    if (v[i] < v[left])
      swap(v, ++last, i);               /*Increment last and not left*/
  
  // now swap and recur:    
  swap(v, left, last);                  /* restore partition elem */
  qsort(v, left, last - 1);
  qsort(v, last + 1, right);
}

/* swap: interchange v[i] and v[j] */
void swap(int v[], int i, int j)
{
 int temp;

 temp = v[i];
 v[i] = v[j];
 v[j] = temp;
}
