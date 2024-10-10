#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 15

typedef struct Item {
    int id;
    char name[50];
    float price;
    int stock; 
    struct Item* next;
} Item;

unsigned int hash(const int id) {
    return id % TABLE_SIZE;
}

void insertItem(Item* hashTable[],int id,const char* name, float price, int stock) {
    unsigned int index = hash(id);

    while (hashTable[index] != NULL) {
        index = (index + 1) % TABLE_SIZE;
    }

    Item* newItem = (Item*)malloc(sizeof(Item));
    newItem->id = id;
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->stock = stock;
    newItem->next = (hashTable[index]);  // insert to the head
    hashTable[index] = newItem;
}

Item* findItem(Item* hashTable[],const int id) {
    unsigned int index = hash(id);
    Item* item = hashTable[index];
    while (item != NULL) {
        if (item->id == id) {
            return item;
        }
        item = item->next;
    }
    return NULL;
}

void deleteItem(Item* hashTable[],int id) {
    unsigned int index = hash(id);
    Item* item = hashTable[index];
    Item* prev = NULL;

    while (item != NULL) {
        if (item->id == id) {
            if (prev == NULL) {
                hashTable[index] = item->next;
            } else {
                prev->next = item->next;
            }
            free(item);
            return;
        }
        prev = item;
        item = item->next;
    }
    printf("Item with ID %d not found\n", id);
}

void printHashTable(Item* hashTable[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Item* item = hashTable[i];
        if (item != NULL) {
            printf("Index %d:\n", i);
            while (item != NULL) {
                printf("  ID: %d, Name: %s, Price: %.2f, Stock: %d\n", item->id, item->name, item->price, item->stock);
                item = item->next;
            }
        }
    }
}


int main() {
    Item* hashTable[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
//"biscoff","butter_cheese","cadina","crispiroll","hokkaido", "nothing","onion","scientist_noodles","snapu_cookies","soda"
    insertItem(hashTable,1, "biscoff", 3, 100);
    insertItem(hashTable,2, "butter_cheese", 5, 100);
    insertItem(hashTable,3, "cadina", 7, 100);
    insertItem(hashTable,4, "crispiroll",3, 100);
    insertItem(hashTable,5, "hokkaido", 5, 100);
    insertItem(hashTable,6, "nothing", 0, 100);
    insertItem(hashTable,7, "onion", 3, 100);
    insertItem(hashTable,8, "scientist_noodles", 3, 100);
    insertItem(hashTable,9, "snapu_cookies", 5, 100);
    insertItem(hashTable,10, "Soda", 5, 100);

    Item* item = findItem(hashTable,2);
    if (item != NULL) {
        printf("Found item: ID=%d, Name=%s, Price=%.2f, Stock=%d\n", item->id, item->name, item->price, item->stock);
    } else {
        printf("Item not found\n");
    }

    deleteItem(hashTable,1);

    printHashTable(hashTable);

    return 0;
}
