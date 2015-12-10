#include "zipmap.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ZM_END 0xff
#define ZM_BIGLEN 0xfd

uint32_t
zipmap_entry_len_size(const char *entry)
{
    if ((uint8_t)entry[0] < ZM_BIGLEN) {
        return 1;
    } else if ((uint8_t)entry[0] == ZM_BIGLEN) {
        return 5;
    }
    return 0;
}

uint32_t
zipmap_entry_strlen (const char *entry)
{
    uint32_t entry_len_size;
    uint32_t slen;

    entry_len_size = zipmap_entry_len_size(entry);
    if (entry_len_size == 1) {
        return (uint8_t)entry[0];
    } else if (entry_len_size == 5) {
        memcpy(&slen, entry + 1, 4);
        return slen;
    } else {
        return 0;
    }
}

uint32_t
zipmap_entry_len (const char *entry)
{
    return  zipmap_entry_len_size(entry) + zipmap_entry_strlen(entry);
}

void
zipmap_dump(const char *zm)
{
    int i, len, klen, vlen;
    char *key, *val;

    len = (uint8_t) zm[0];
    ++zm;

    for ( i =0; i < len; i++ ) {
        // key
        klen = zipmap_entry_strlen(zm);
        key = malloc(klen + 1);
        memcpy(key, zm + zipmap_entry_len_size(zm), klen);
        zm += zipmap_entry_len(zm);
        // value
        vlen = zipmap_entry_strlen(zm);
        val = malloc(vlen + 1);
        memcpy(val, zm + zipmap_entry_len_size(zm) + 1, vlen);
        zm += zipmap_entry_len(zm) + 1;
        printf(" key is %s, value is %s\n", key, val);
        free(key);
        free(val);
    }
}