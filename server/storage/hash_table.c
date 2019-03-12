#include "hash_table.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint16_t get_hash(uint8_t *key, uint8_t len)
{
	uint16_t hash = 0;
	for (uint8_t i = 0; i < len; ++len)
		hash += key[i] * 10 + i + 1;
	return hash % PRIME_LENGTH;
}

hash_table *get_hash_table()
{
	hash_table *table = malloc(sizeof(hash_table));
	for (int i = 0; i < PRIME_LENGTH; ++i)
		table->elements[i] = NULL;
	table->count = 0;
	table->filled = 0;
	return table;
}

linked_container *get_linked_container()
{
	linked_container *c = malloc(sizeof(linked_container));
	c->next = NULL;
	c->prev = NULL;
	c->data = get_default_content();
	return c;
}

uint8_t *get_new_copy(uint8_t *source)
{
	uint32_t len = strlen((char *)source);
	uint8_t *new = calloc(len + 1, sizeof(uint8_t));
	memcpy(new, source, len);
	return new;
}

void linked_container_set_data(linked_container *c, uint8_t *data)
{
	c->data->data = get_new_copy(data);
	c->data->len = strlen((char *)data);
}

void linked_container_set_key(linked_container *c, uint8_t *key)
{
	c->data->key = get_new_copy(key);
}

void set_linked_container_content(linked_container *c, uint8_t *key, uint8_t *data)
{
	linked_container_set_key(c, key);
	linked_container_set_data(c, data);
}

linked_container *get_and_set_linked_container(uint8_t *key, uint8_t *data)
{
	linked_container *c = get_linked_container();
	set_linked_container_content(c, key, data);
	return c;
}

int hash_table_insert(hash_table *table, uint8_t *key, uint8_t *data)
{
	uint16_t hash = get_hash(key, strlen((char *)key));

	if (table->elements[hash] == NULL)
	{
		table->elements[hash] = get_and_set_linked_container(key, data);
		++table->filled;
		++table->count;
		return 0;
	}
	linked_container *c = table->elements[hash];
	for (;;)
	{
		if (!strcmp((char *)c->data->key, (char *)key))
		{
			free(c->data->data);
			linked_container_set_data(c, data);
			return 0;
		}
		if (c->next == NULL)
			break;
		c = c->next;
	}
	c->next = get_and_set_linked_container(key, data);
	c->next->prev = c;
	++table->count;
	return 0;
}

uint8_t *hash_table_get(hash_table *table, uint8_t *key)
{
	uint16_t hash = get_hash(key, strlen((char *)key));

	if (table->elements[hash] == NULL)
		return NULL;
	linked_container *c = table->elements[hash];
	for (;;)
	{
		if (!strcmp((char *)c->data->key, (char *)key))
		{
			return c->data->data;
		}
		if (c->next == NULL)
			break;
		c = c->next;
	}
	return NULL;
}

uint8_t *hash_table_pop(hash_table *table, uint8_t *key)
{
	uint16_t hash = get_hash(key, strlen((char *)key));
	if (table->elements[hash] == NULL)
		return NULL;
	linked_container *c = table->elements[hash];
	for (;;)
	{
		if (!strcmp((char *)c->data->key, (char *)key))
		{
			uint8_t *tmp = get_new_copy(c->data->data);
			if (c->prev == NULL)
			{
				linked_container *tmp_lc = c->next;
				free_linked_container(c);
				table->elements[hash] = tmp_lc;
				c = table->elements[hash];
				if (c != NULL)
					c->prev = NULL;
			}
			else if (c->next == NULL)
			{
				free_linked_container(c);
			}
			else
			{
				linked_container *tmp_lcn = c->next,
								 *tmp_lcp = c->prev;
				free_linked_container(c);
				tmp_lcp->next = tmp_lcn;
				tmp_lcn->prev = tmp_lcp;
			}
			return tmp;
		}
		if (c->next == NULL)
			break;
		c = c->next;
	}
	return NULL;
}

uint8_t **hash_table_get_keys(hash_table *t)
{
	uint8_t **keys = calloc(t->count, sizeof(uint8_t *));
	for (uint32_t i = 0, k = 0; i < PRIME_LENGTH; ++i)
	{
		if (t->elements[i] == NULL)
			continue;
		linked_container *c = t->elements[i];
		for (; c->next != NULL; ++k)
		{
			keys[k] = get_new_copy(c->data->key);
			c = c->next;
		}
		keys[k] = get_new_copy(c->data->key);
		++k;
	}
	return keys;
}

void hash_table_print(hash_table *table)
{
	for (uint32_t i = 0; i < PRIME_LENGTH; ++i)
	{
		if (table->elements[i] != NULL)
		{
			linked_container *c = table->elements[i];
			for (; c != NULL;)
			{
				printf("%s\t%s\n", c->data->key, c->data->data);
				c = c->next;
			}
		}
	}
	return;
}

void free_linked_container(linked_container *c)
{
	free(c->data->data);
	free(c->data->key);
	free(c->data);
	free(c);
}

void free_hash_table(hash_table *table)
{
	for (uint32_t i = 0; i < PRIME_LENGTH; ++i)
	{
		if (table->elements[i] == NULL)
			continue;
		linked_container *c = table->elements[i];
		for (linked_container *tmp = NULL; c != NULL;)
		{
			tmp = c->next;
			free_linked_container(c);
			c = tmp;
		}
	}
	free(table);
	return;
}
