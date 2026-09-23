#include <stdio.h>
#include <kv.h>

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
}