#pragma once

void insertion_sort(void *arr, size_t sz, size_t len,
                    int (*cmp)(const void *a, const void *b));
