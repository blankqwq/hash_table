﻿// hash_table.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

typedef struct {
	char* key;
	char* value;
} ht_item;

typedef struct {
	int size;
	int count;
	ht_item** items;
	int base_size;
}ht_hash_table;

void ht_insert(ht_hash_table* ht,const char* key,const char* value);
char* ht_search(ht_hash_table* ht,const char* key);
void ht_delete(ht_hash_table* h, const char* key);