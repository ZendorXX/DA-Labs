#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEY_LENGH 256
#define MAX_PATH_LENGHT 260
#define MAX_CMD_LENGHT 1 + 1 + MAX_KEY_LENGH + 1 + 1 + 20 + 1

typedef struct {
    char key[MAX_KEY_LENGH + 1];
    uint64_t number;
} item;

typedef struct {
    item* array;
    uint64_t size;
    uint64_t capacity;
} dictionary;

void init(dictionary *d) {
    d->size = 0;
    d->capacity = 0;
    d->array = NULL;    
}

void add(dictionary *d, item word) {
    if (d->array == NULL) {
        d->capacity = 1;
        d->array = (item*)calloc(1, sizeof(item));
    }
    else if (d->size == d->capacity) {
        d->capacity *= 2;
        item* newArray = (item*)calloc(d->capacity, sizeof(item));
        for (size_t i = 0; i < d->size; ++i) {
            newArray[i] = d->array[i];
        }
        free(d->array);
        d->array = newArray;
    }
    d->array[d->size++] = word;
}

void delete(dictionary *d, char key_to_delete[MAX_KEY_LENGH + 1]) {
    for (size_t i = 0; i < d->size; ++i) {
        if (strcmp(d->array[i].key, key_to_delete) == 0) {
            for (size_t j = i; j < d->size - 1; ++j) {
                d->array[j] = d->array[j + 1];
            }
            d->size -= 1;
            break;
        }
    }    
}

int find(dictionary *d, char key_to_find[MAX_KEY_LENGH + 1]) {
    for (size_t i = 0; i < d->size; ++i) {
        if (strcmp(d->array[i].key, key_to_find) == 0) {
            return i + 1;
        }
    }
    return 0;
}

void save(dictionary *d, char filename[MAX_PATH_LENGHT]) {
    FILE *ptr = fopen(filename, "w");

    for (size_t i = 0; i < d->size; ++i) {
        fprintf(ptr, "%s %"PRIu64"\n", d->array[i].key, d->array[i].number);
    }

    fclose(ptr);
}

void load(dictionary *d, char filename[MAX_PATH_LENGHT]) {
    init(d);

    FILE *ptr = fopen(filename, "r");

    char tmp[MAX_KEY_LENGH + 1];
    uint64_t n;

    while(fscanf(ptr, "%s %"PRIu64, tmp, &n) > 0) {
        item word;
        word.number = n;
        strncpy(word.key, tmp, MAX_KEY_LENGH + 1);
        add(d, word);
    }

    fclose(ptr);
}

void print(dictionary *d) {
    if (d->array == NULL) {
        printf("Empty\n");
        return;
    }
    for (size_t i = 0; i < d->size; ++i) {
        printf("%s %"PRIu64"\n", d->array[i].key, d->array[i].number);
    }
}

void destroy(dictionary *d) {
    d->size = 0;
    d->capacity = 0;
    if (d->array == NULL) {
        return;
    }
    free(d->array);
    d->array = NULL;
}

int main() {
    dictionary d;   
    init(&d);

    char cmd[MAX_CMD_LENGHT];
    while (fgets(cmd, MAX_CMD_LENGHT, stdin) != NULL) {
        if (cmd[0] == '+') { // add
            char symbol;
            char word[MAX_KEY_LENGH + 1];
            uint64_t number;

            sscanf(cmd, "%c %s %"PRIu64, &symbol, word, &number);

            for (size_t i = 0; i < strlen(word); ++i) {
                if ('A' <= word[i] && word[i] <= 'Z') {
                    word[i] = (char)(word[i] + 32);
                }
            }

            item new_element;
            strncpy(new_element.key, word, MAX_KEY_LENGH + 1);
            new_element.key[256] = '\0';
            new_element.number = number;

            if (find(&d, word) == 0) {
                add(&d, new_element);
                printf("OK\n");
            }
            else {
                printf("Exist\n");
            }
        }
        else if (cmd[0] == '-') { // delete
            char symbol;
            char word[MAX_KEY_LENGH + 1];

            sscanf(cmd, "%c %s", &symbol, word);

             for (size_t i = 0; i < strlen(word); ++i) {
                if ('A' <= word[i] && word[i] <= 'Z') {
                    word[i] = (char)(word[i] + 32);
                }
            }

            if (find(&d, word) == 0) {
                printf("NoSuchWord\n");
            }
            else {
                delete(&d, word);
                printf("OK\n");
            }
        } 
        else if (isalpha(cmd[0])) { // search
            char word[MAX_KEY_LENGH + 1];

            sscanf(cmd, "%s", word);

             for (size_t i = 0; i < strlen(word); ++i) {
                if ('A' <= word[i] && word[i] <= 'Z') {
                    word[i] = (char)(word[i] + 32);
                }
            }

            size_t position = find(&d, word);

            if (position != 0) {
                printf("OK: %"PRIu64"\n", d.array[position - 1].number);
            } 
            else {
                printf("NoSuchWord\n");
            }
        } 
        else if (cmd[0] == '!') {
            char symbol;
            char word[4];
            char path[MAX_PATH_LENGHT];

            sscanf(cmd, "%c %s %s", &symbol, word, path);

            if (cmd[2] == 'L') {  // load
                load(&d, path);
                printf("OK\n");
            }
            else if (cmd[2] == 'S') { // save
                save(&d, path);
                printf("OK\n");
            }
        }
        else if (cmd[0] == '?') {
            print(&d);
        }
     }

    destroy(&d);

    return 0;
}