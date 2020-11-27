// HashTable.cpp: 定义应用程序的入口点。
//

#include "hash_table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HT_PRIME_1 1113
#define HT_PRIME_2 2225
#define HT_INITAL_BASE_SIZE 10
#define HT_INVALID_IDX -1
#define HT_MIN_MASK -2


static ht_item HT_DELETED_ITEM = { NULL,NULL };


static int hash_check_size(int size) {
	return size;
}

// 新创建一个基础数量size的hashTable
static ht_hash_table* ht_new_sized(const int base_size) {
	ht_hash_table* ht = (ht_hash_table*)malloc(sizeof(ht_hash_table));
	if (ht==NULL) {
		return NULL;
	}
	ht->base_size = base_size;
	ht->count = 0;
	ht->nNumUsed = 0;
	ht->nNumOfElements = 0;
	ht->nNextFreeElement = 0;
	ht->nTableSize = hash_check_size(base_size);
	ht->nTableMask = HT_MIN_MASK;
	ht->nInternalPointer = HT_INVALID_IDX;
	// 初始化tableBucket
	ht->items = calloc((size_t)ht->nTableSize, sizeof(ht_item*));
	return ht;
}


static void ht_del_item(ht_item* i) {
	free(i->key);
	free(i->value);
	free(i->h);
	free(i);
}


void ht_del_hash_table(ht_hash_table* ht) {
	for (int i = 0; i < ht->size; i++) {
		ht_item* item = ht->items[i];
		if (item != NULL) {
			ht_del_item(item);
		}
	}
	free(ht->items);
	free(ht);
}


// 重新定义大小
static void ht_resize(ht_hash_table* ht, const int base_size) {
	if (base_size < HT_INITAL_BASE_SIZE) {
		return;
	}
	ht_hash_table* new_ht = ht_new_sized(base_size);
	for (int i = 0; i < ht->size; i++) {
		ht_item* item = ht->items[i];
		if (item != NULL && item != &HT_DELETED_ITEM) {
			ht_insert(new_ht, item->key, item->value);
		}
	}
	ht->base_size = new_ht->base_size;
	ht->count = new_ht->count;

	const int tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = tmp_size;


	ht_item** tmp_items = ht->items;
	ht->items = new_ht->items;
	new_ht->items = tmp_items;
	ht_del_hash_table(new_ht);
}

// 创建一个默认基础大小的hash
ht_hash_table* ht_new() {
	return ht_new_sized(HT_INITAL_BASE_SIZE);
}


static ht_item* ht_new_item(const char* k, const char* v) {
	ht_item* i= (ht_item*)malloc(sizeof(ht_item));
	if (i != NULL) {
		i->key = strdup(k);
		i->value = strdup(v);
		return i;
	}
}




// calculate hash
static int ht_hash(const char* s, const int a, const int m) {
	long hash = 0;
	const int len_s = strlen(s);
	for (int i = 0; i < len_s; i++) {
		hash += (long)pow(a, len_s - (i + 1) * s[i]);
		hash = hash % m;
	}
	return (int)hash;
}

static void ht_resize_up(ht_hash_table* ht) {
	const int new_size = ht->base_size * 2;
	ht_resize(ht, new_size);
}
static void ht_resize_down(ht_hash_table* ht) {
	const int new_size = ht->base_size / 2;
	ht_resize(ht, new_size);
}

// 计算hash值
static int ht_get_hash(const char* s,const int num_buckets,const int attempt) {
	const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
	const int hash_b = ht_hash(s,HT_PRIME_2,num_buckets);
	return (hash_a + (attempt * (hash_b + 1)) % num_buckets);
}

// 插入对应值
void  ht_insert(ht_hash_table* ht, const char* key, const char* value) {
	const int load = ht->count * 100 / ht->size;
	if (load > 70) {
		ht_resize_up(ht);
	}
	ht_item* item = ht_new_item(key,value);
	int index = ht_get_hash(item->key,ht->size,0);
	ht_item* cur_item = ht->items[index];
	int i = 1;
	while ( cur_item!=NULL && cur_item != &HT_DELETED_ITEM)
	{
		// 解决hash冲突
		index = ht_get_hash(item->key, ht->size, i);
		cur_item = ht->items[index];
		i++;
	}
	ht->items[index] = item;
	ht->count++;
}

// 查找对应的值
char* ht_search(ht_hash_table* ht, const char* key) {
	const int load = ht->count * 100 / ht->size;
	if (load < 10) {
		ht_resize_up(ht);
	}
	int index = ht_get_hash(key, ht->size, 0);
	ht_item* item = ht->items[index];
	int i = 1;
	while (item != NULL)
	{
		if (item!=&HT_DELETED_ITEM) {
			if (strcmp(item->key, key)) {
				return item->value;
			}
		}
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}
	return NULL;
}




// 删除
void ht_delete(ht_hash_table* ht, const char* key) {
	int index = ht_get_hash(key, ht->size, 0);
	ht_item* item = ht->items[index];
	int i = 1;
	while (item != NULL)
	{
		if (strcmp(item->key, key)) {
			ht_del_item(item);
			ht->items[index] = &HT_DELETED_ITEM;
		}
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}
	ht->count--;
}





// 主函数
int main()
{
	printf("hello world");
	ht_hash_table* table = ht_new();
	ht_del_hash_table(table);
	return 0;
}