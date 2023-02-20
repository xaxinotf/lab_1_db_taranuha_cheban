#pragma once
#include "Master.h"
#include "Slave.h"
#include "OutputEntities.h"
#define PRODUCT_DATA "product.fl"
#define PRODUCT_GARBAGE "product_garbage.txt"
int updateDrugStore(struct DrugStore client, char* error, int id);
struct Product FindLastAddress(FILE *database, struct DrugStore *drugStore, struct Product *previous)
{
    for (int i = 0; i < drugStore->countOfProduct; i++) {
        fread(previous, PRODUCT_SIZE, 1, database);
        fseek(database, (*previous).nextAddress, SEEK_SET);
    }
    return (*previous);
}
void NextAddress(FILE* database, struct DrugStore *drugStore, struct Product *product)
{
    fclose(database);
    database = fopen(PRODUCT_DATA, "r+b");
    struct Product previous;
    fseek(database, drugStore->productFirstAddress, SEEK_SET);
    previous = FindLastAddress(database, drugStore, &previous);
    previous.nextAddress = product->selfAddress;
    fwrite(&previous, PRODUCT_SIZE, 1, database);
}
void overwriteGarbageAddress(int garbageCount, FILE* garbageZone, struct Product* record) {
    int * deletedIds = malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%d", deletedIds + i);
    }

    record->selfAddress = deletedIds[0];
    record->nextAddress = deletedIds[0];

    fclose(garbageZone);
    fopen(PRODUCT_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);

    for (int i = 1; i < garbageCount; i++) {
        fprintf(garbageZone, " %d", deletedIds[i]);
    }

    free(deletedIds);
    fclose(garbageZone);
}
int insertProduct(struct DrugStore drugStore, struct Product product, char * error)
{
    FILE* database = fopen(PRODUCT_DATA, "a+b");
    FILE* garbageZone = fopen(PRODUCT_GARBAGE, "a+b");
    int garbageCount = 0;
    product.exists=1;
    fscanf(garbageZone, "%d", &garbageCount);
    struct Product product1;
    fseek(database, 0, SEEK_END);
    if (garbageCount) {
        overwriteGarbageAddress(garbageCount, garbageZone, &product);
        fclose(database);
        database = fopen(PRODUCT_DATA, "r+b");
        fseek(database, product.selfAddress, SEEK_SET);
    }
    else
    {
        if (ftell(database) != 0) {
            fseek(database, -(PRODUCT_SIZE), SEEK_END);
            fread(&product1, PRODUCT_SIZE, 1, database);
            product.Id = product1.Id + 1;
        } else {
            product.Id = 1;
        }
        int dbSize = ftell(database);
        product.selfAddress = dbSize;
        product.nextAddress = dbSize;
    }
    fseek(database, 0, SEEK_END);
    printf("Your product id is %d \n", product.Id);
    fwrite(&product, PRODUCT_SIZE, 1, database);
    if (!drugStore.countOfProduct) {
        drugStore.productFirstAddress = product.selfAddress;
    }
    else {
        NextAddress(database, &drugStore, &product);
    }
    fclose(database);
    int count = drugStore.countOfProduct + 1;
    drugStore.countOfProduct=count;
    updateDrugStore(drugStore, error, drugStore.id);
    return 1;
}
int getProduct(struct DrugStore drugStore, struct Product* product, int productId, char* error) {

    FILE* database = fopen(PRODUCT_DATA, "rb");
    fseek(database, drugStore.productFirstAddress, SEEK_SET);
    fread(product, PRODUCT_SIZE, 1, database);

    for (int i = 0; i < drugStore.countOfProduct; i++) {
        if (product->Id == productId)
        {
            fclose(database);
            return 1;
        }
        fseek(database, product->nextAddress, SEEK_SET);
        fread(product, PRODUCT_SIZE, 1, database);
    }
    fclose(database);
    return 0;
}
int updateProduct(struct Product product)
{
    FILE* database = fopen(PRODUCT_DATA, "r+b");
    fseek(database, product.selfAddress, SEEK_SET);
    fwrite(&product, PRODUCT_SIZE, 1, database);
    fclose(database);
    return 1;
}
void noteDeletedProduct(int address) {
    FILE* garbageZone = fopen(PRODUCT_GARBAGE, "rb");
    int garbageCount;
    fscanf(garbageZone, "%d", &garbageCount);
    int* deletedAddresses = malloc(garbageCount * sizeof(int));

    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%d", deletedAddresses + i);
    }

    fclose(garbageZone);
    garbageZone = fopen(PRODUCT_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount + 1);

    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbageZone, " %d", deletedAddresses[i]);
    }

    fprintf(garbageZone, " %d", address);
    free(deletedAddresses);
    fclose(garbageZone);
}
void relinkAddresses(FILE* database, struct Product previous, struct Product product, struct DrugStore* drugStore) {
    if (product.selfAddress == drugStore->productFirstAddress) {
        if (product.selfAddress == product.nextAddress) {
            drugStore->productFirstAddress = -1;
        } else {
            drugStore->productFirstAddress = product.nextAddress;
        }
    } else {
        if (product.selfAddress == product.nextAddress) {
            previous.nextAddress = previous.selfAddress;
        }
        else {
            previous.nextAddress = product.nextAddress;
        }
        fseek(database, previous.selfAddress, SEEK_SET);
        fwrite(&previous, PRODUCT_SIZE, 1, database);
    }
}

void deleteProduct(struct DrugStore drugStore, struct Product product, char* error)
{
    FILE* database = fopen(PRODUCT_DATA, "r+b");
    struct Product previous;
    fseek(database, drugStore.productFirstAddress, SEEK_SET);
    do {
        fread(&previous, PRODUCT_SIZE, 1, database);
        fseek(database, previous.nextAddress, SEEK_SET);
    }
    while (previous.nextAddress != product.selfAddress && product.selfAddress != drugStore.productFirstAddress);
    relinkAddresses(database, previous, product, &drugStore);
    noteDeletedProduct(product.selfAddress);
    product.exists = 0;

    fseek(database, product.selfAddress, SEEK_SET);
    fwrite(&product, PRODUCT_SIZE, 1, database);
    fclose(database);

    drugStore.countOfProduct--;
    updateDrugStore(drugStore, error, drugStore.id);
}
void PrintList(struct DrugStore drugStore)
{
    FILE* productDb = fopen(PRODUCT_DATA, "a+b");
    struct Product product;
    fseek(productDb, drugStore.productFirstAddress, SEEK_SET);
    for (int i = 0; i < drugStore.countOfProduct; i++) {
        fread(&product, PRODUCT_SIZE, 1, productDb);
        if (product.exists)
        {
            OutputProduct(product, drugStore);
            printf("<====================================>\n");
        }
        fseek(productDb, product.nextAddress, SEEK_SET);
    }
    fclose(productDb);
}