// HashTable.h: 标准系统包含文件的包含文件
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
}ht_hash_table;

