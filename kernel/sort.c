#include <tinyos/common/string.h>

void insertion_sort(void *arr, size_t sz, size_t len,
                    int (*cmp)(const void *a, const void *b)) {
  char key[sz];
  int j;

  for (size_t i = 1; i < len; i++) {
    memcpy(&key, arr + i * sz, sz);  /* key = arr[i] */

    for (j = i - 1; j >= 0 && cmp(arr + j * sz, &key) > 0; j--) {
      memcpy(arr + (j + 1) * sz, arr + j * sz, sz);  /* arr[j + 1] = arr[j] */
    }

    memcpy(arr + (j + 1) * sz, &key, sz);  /* arr[j + 1] = key */
  }
}
