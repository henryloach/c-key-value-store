#include <stdio.h>
#include <kv.h>
#include <string.h>
#include <assert.h>

int main() {
    kv_t* table = kv_init(1024);

    printf("%p\n", table);

    printf("%ld\n", table->capacity);

    printf("foo\n");

    kv_put(table, "here", "I am");
    kv_put(table, "here", "You are");

    for (int i = 0; i < table->capacity; i++) {
        kv_entry_t entry = table->entries[i];
        if (entry.key) {
            printf("%d - %s: %s\n", i, entry.key, entry.value);
        }
    }

    char* gotString = kv_get(table, "here");
    printf("%s\n", gotString);

    kv_t *db = kv_init(16);

    kv_put(db, "name", "alice");
    assert(strcmp(kv_get(db, "name"), "alice") == 0);
    assert(kv_get(db, "missing") == NULL);
}