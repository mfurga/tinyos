#pragma once

void *kmalloc(size_t sz);

void *kzmalloc(size_t sz);

void kfree(void *p);
