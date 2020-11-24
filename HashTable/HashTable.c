// HashTable.cpp: 定义应用程序的入口点。
//

#include "HashTable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HT_PRIME_1 1
#define HT_PRIME_2 2


static ht_item* ht_new_item(const char* k, const char* v) {
	ht_item* i= (ht_item*)malloc(sizeof(ht_item));
	if (i != NULL) {
		i->key = strdup(k);
		i->value = strdup(v);
		return i;
	}
}

static void ht_del_item(ht_item* i) {
	free(i->key);
	free(i->value);
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

ht_hash_table* ht_new() {
	ht_hash_table* ht = (ht_hash_table*)malloc(sizeof(ht_hash_table));
	if (ht != NULL) {
		ht->size = 53;
		ht->count = 0;
		ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
		return ht;
	}
}

int main()
{
	printf("hello world");
	ht_hash_table* table = ht_new();
	ht_del_hash_table(table);
	return 0;
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

static int ht_get_hash(const char* s,const int num_buckets,const int attempt) {
	const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
	const int hash_b = ht_hash(s,HT_PRIME_2,num_buckets);
	return (hash_a + (attempt * (hash_b + 1)) % num_buckets);
}

void  ht_insert(ht_hash_table* ht, const char* key, const char* value) {
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

char* ht_search(ht_hash_table* ht, const char* key) {
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

static ht_item HT_DELETED_ITEM = { NULL,NULL };

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