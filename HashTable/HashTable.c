// HashTable.cpp: 定义应用程序的入口点。
//

#include "HashTable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
