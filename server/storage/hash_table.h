#include "data.h"

#define PRIME_LENGTH 1021

typedef struct linked_container linked_container;

struct linked_container
{
    linked_container *next,
        *prev;
    hash_table_content *data;
};

typedef struct
{

} hash_table_element;

typedef struct
{
    linked_container *elements[1021];
    uint32_t filled,
        count;
} hash_table;

hash_table *get_hash_table();
void free_hash_table();
linked_container *get_linked_container();
void free_linked_container();
uint16_t get_hash(uint8_t *, uint32_t);
int hash_table_insert(hash_table *, simple_string *, simple_string *);
simple_string *hash_table_get(hash_table *, simple_string *);
simple_string *hash_table_pop(hash_table *, simple_string *);
simple_string **hash_table_get_keys(hash_table *);
void hash_table_print(hash_table *);
