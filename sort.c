#include <stdint.h>

typedef int bool;
#define false 0
#define true 1

// Basic function to test calling llvm from sort.rkt
// call from sort.rkt as (source:@foo (bv 3 32))
// returns (bv 8 32)
uint32_t foo(uint32_t x) {
  return x + 5;
}

// Another simple function to test passing arrays
uint32_t min(uint32_t* list, uint32_t len) {
  if (len == 0) return -1;
  uint32_t min_val = list[0];
  for (int i = 1; i < len; i++) {
    if (list[i] < min_val) min_val = list[i];
  }
  return min_val;
}

// -------------------
//  Quicksort Testing
// -------------------
// from https://www.geeksforgeeks.org/quick-sort/
// A utility function to swap two elements
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (int arr[], int low, int high) {
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++) {
        // If current element is smaller than the pivot
        if (arr[j] < pivot) {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

void test_quicksort_3(int32_t a0, int32_t a1, int32_t a2) {
  // currently times out before completing
  int32_t list[3];
  list[0] = a0;
  list[1] = a1;
  list[2] = a2;
  quick_sort(list, 0, 2);
}

// ------------------------
//  Insertion Sort Testing
// ------------------------
void insertion_sort(int arr[], int len) {
  // This implementation returns early if
  // the array is already sorted.
  int sorted = 1;
  for (int i = 0; i < len-1; i++) {
    if (arr[i] > arr[i+1]) {
      sorted = 0;
      break;
    }
  }
  if (sorted) return;

  // O(n^2) sorting algorithm
  for (int i = 0; i < len-1; i++) {
    int min_i = i;
    int min_val = arr[i];
    for (int j = i+1; j < len; j++) {
      if (arr[j] < min_val) {
        min_i = j;
        min_val = arr[j];
      }
    }
    swap(&arr[i], &arr[min_i]);
  }
}

void test_insertion_sort_3(int32_t a0, int32_t a1, int32_t a2) {
  // Make a list of length 3 and sort
  int32_t list[3];
  list[0] = a0;
  list[1] = a1;
  list[2] = a2;
  insertion_sort(list, 3);
}

// --------------------
//  Hash Table Testing
// --------------------
typedef struct {
  int is_valid;
  int key;
  int val;
} HashEntry;

void hash_init(HashEntry tbl[], int size) {
  for (int i = 0; i < size; i++) {
    tbl[i].is_valid = false;
  }
}

int hash_fcn(int key) {
  // return key;
  return key * key;
}

bool hash_put(HashEntry tbl[], int size, int key, int val) {
  int index = hash_fcn(key) % size;
  bool inserted = false;
  for (int offset = 0; offset < size; offset++) {
    int tbl_i = (index+offset)%size;
    if (tbl[tbl_i].is_valid) {
      if (tbl[tbl_i].key == key) {
        tbl[tbl_i].val = val;
        inserted = true;
        break;
      }
    } else {
      tbl[tbl_i].is_valid = true;
      tbl[tbl_i].key = key;
      tbl[tbl_i].val = val;
      inserted = true;
      break;
    }
  }
  return inserted;
}

// returns size if key not found in table
bool hash_get_index(HashEntry tbl[], int size, int32_t key) {
  int index = hash_fcn(key) % size;
  int tbl_index = size;
  for (int offset = 0; offset < size; offset++) {
    int tbl_i = (index+offset)%size;
    if (tbl[tbl_i].key == key) {
      tbl_index = tbl_i;
      break;
    }
  }
  return tbl_index;
}

void test_hash(int32_t k0, int32_t k1, int32_t k2) {
  /*
  For hash_fcn = key:
    Max solution: (model
     [a0 (bv #x00000004 32)]
     [a1 (bv #x00002004 32)]
     [a2 (bv #x00000024 32)])
    Min solution: (model
     [a0 (bv #x00000002 32)]
     [a1 (bv #x00000003 32)]
     [a2 (bv #x00000000 32)])

  For hash_fcn = key*key:
    Max solution: (model
     [a0 (bv #x00003200 32)]
     [a1 (bv #x00000000 32)]
     [a2 (bv #x00000004 32)])
    >>> k0=0x00003200
    >>> k1=0x0
    >>> k2=0x4
    >>> (k0*k0)%8
    0
    >>> (k1*k1)%8
    0
    >>> (k2*k2)%8
    0

    Min solution: (model
     [a0 (bv #xffffc74c 32)]
     [a1 (bv #x00005112 32)]
     [a2 (bv #x000007c3 32)])
    >>> k0=0xffffc74c
    >>> k1=0x00005112
    >>> k2=0x000007c3
    >>> (k0*k0)%8
    0
    >>> (k1*k1)%8
    4
    >>> (k2*k2)%8
    1
  */

  // Make the hash table
  int size = 8;
  HashEntry tbl[size];
  hash_init(tbl, size);

  // Insert values
  hash_put(tbl, size, k0, 100);
  hash_put(tbl, size, k1, 101);
  hash_put(tbl, size, k2, 102);

  // Get a value
  hash_get_index(tbl, size, k0);
}
