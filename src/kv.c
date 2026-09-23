#include <kv.h>
#include <string.h>
#include <stdlib.h>

size_t hash(char* val, int capacity) {
    size_t hash = 0x13371337deadbeef;

    while(*val) {
        hash ^= *val;
        hash = hash << 8;
        hash += *val;

        val++;
    }

    return hash % capacity;
}

char* kv_get(kv_t* db, char* key) {
    if (!db || !key) return NULL;

    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity; i++) {
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t* entry = &db->entries[real_idx];

        if (entry->key == NULL) return NULL;
        if (
            entry->key && 
            entry->key != TOMBSTONE &&  
            !strcmp(entry->key, key)
        ) {
            return entry->value;
        }
    }

    return NULL;
}

int kv_put(kv_t* db, char* key, char* value) {
    if (!db || !key || !value) return -1;

    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity; i++) {

        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t* entry = &db->entries[real_idx];

        // place in empty
        if (!entry->key || entry->key == TOMBSTONE) {
            char* newkey = strdup(key);
            char* newval = strdup(value);
            if (!newval || !newkey) {
                free(newkey);
                free(newval);
                return -1;
            }
            entry->key = newkey;
            entry->value = newval;

            db->count++;
            return 0;
        }

        // append
        if (entry->key && entry->key != TOMBSTONE && !strcmp(entry->key, key)) {
            char *newval = strdup(value);
            if (!newval) return -1;

            free(entry->value);
            entry->value = newval;
            return 0;
        }

        // continue

    }

    return -1;
}

kv_t* kv_init(size_t capacity) {
    if (capacity == 0) return NULL;

    kv_t* table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    } 

    table->capacity = capacity;
    table->count = 0;

    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if (table->entries == NULL) {
        return NULL;
    }

    return table;
}
