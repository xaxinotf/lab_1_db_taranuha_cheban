#pragma once

#include "Slave.h"
#include <mbstring.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Master.h"
#include "DrugStore.h"
#include "Check.h"

#define INDEXER_SIZE sizeof(struct Indexer)
#define DrugStore_DATA "drugStore.fl"
#define DrugStore_SIZE sizeof(struct DrugStore)
#define PRODUCT_DATA "product.fl"
#define PRODUCT_SIZE sizeof(struct Product)
#define DrugStore_IND "drugStore.ind"

int getDrugStore(struct DrugStore *pStore, int i, char error[51]);

int checkFileExists(FILE* indexTable, FILE* database, char* error)
{
    if (indexTable == NULL || database == NULL) {
        strcpy(error, "DB file don`t exits");
        return 0;
    }
    return 1;
}
int checkIndexExists(FILE* indexTable, char* error, int id) {
    fseek(indexTable, 0, SEEK_END);
    int indexTableSize = ftell(indexTable);
    if (indexTableSize == 0 || id * INDEXER_SIZE > indexTableSize) {
        strcpy(error, "no this ID in the table");
        return 0;
    }
    return 1;
}
int checkRecordExists(struct Indexer indexer, char* error) {
    if (indexer.exists == 0 ) {
        strcpy(error, "the record was deleted");
        return 0;
    }
    return 1;
}
int checkIfRecordExist(struct DrugStore client, int orderId, char *error ) {
    FILE* ordersDb = fopen(PRODUCT_DATA, "a+b");
    struct Product product;
    fseek(ordersDb, client.productFirstAddress, SEEK_SET);
    for (int i = 0; i < client.countOfProduct; i++) {
        fread(&product, PRODUCT_SIZE, 1, ordersDb);
        fclose(ordersDb);
        if (product.Id == orderId)
        {
            if(product.exists)
                return 1;
            else
            {
                strcpy(error, "Product was deleted");
                return 0;
            }
        }
        ordersDb = fopen(PRODUCT_DATA, "r+b");
        fseek(ordersDb, product.nextAddress, SEEK_SET);
    }
    strcpy(error, "The client do not have this product, the product has been deleted or does not exist");
    fclose(ordersDb);
    return 0;
}


void info() {
    FILE* indexTable = fopen(DrugStore_IND, "rb");
    if (indexTable == NULL) {
        printf("Error: database files do not exist\n");
        return;
    }
    int drugStoreCount = 0;
    int productCount = 0;

    fseek(indexTable, 0, SEEK_END);
    int indAmount = ftell(indexTable) / sizeof(struct Indexer);
    struct DrugStore drugStore;
    char error[51];
    for (int i = 1; i <= indAmount; i++)
    {
        if (getDrugStore(&drugStore, i, error))
        {
            drugStoreCount++;
            productCount += drugStore.countOfProduct;
            printf("Drugstore #%d has %d products\n", i, drugStore.countOfProduct);
        }
    }
    fclose(indexTable);
    printf("Total drugStore: %d\n", drugStoreCount);
    printf("Total product: %d\n", productCount);
}

