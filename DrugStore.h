#pragma once
#include <malloc.h>
#include "Master.h"
#include "Input.h"
#include "Check.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Product.h"

#define DrugStore_IND "drugStore.ind"
#define DrugStore_DATA "drugStore.fl"
#define DrugStore_GARBAGE "drugStore_garbage.txt"
#define INDEXER_SIZE sizeof(struct Indexer)
#define DrugStore_SIZE sizeof(struct DrugStore)
#define PRODUCT_SIZE sizeof(struct Product)

int PrintListOfClient(char *error);
void overwriteGarbageClientElement(int garbageCount, FILE* garbageZone, struct DrugStore* client) {
    int* deleteIdsFile = malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageZone, "%d", deleteIdsFile + i);
    }
    fclose(garbageZone);
    client->id = deleteIdsFile[0];
    fopen(DrugStore_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %d", deleteIdsFile[i]);
    }
    fclose(garbageZone);
    free(deleteIdsFile);
}
int insertDrugStore(struct DrugStore client){
    FILE* garbageZone = fopen(DrugStore_GARBAGE, "a+b");
    FILE* indexTable = fopen(DrugStore_IND, "a+b");
    FILE* database = fopen(DrugStore_DATA, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    struct Indexer indexer;
    if (garbageCount !=0 )
    {
        overwriteGarbageClientElement(garbageCount, garbageZone, &client);
        fclose(database);
        fclose(indexTable);
        indexTable = fopen(DrugStore_IND, "r+b");
        database = fopen(DrugStore_DATA, "r+b");
        fseek(indexTable, (client.id - 1) * INDEXER_SIZE, SEEK_SET);
        fread(&indexer, INDEXER_SIZE, 1, indexTable);
        fseek(database, indexer.address, SEEK_SET);
    }
    else {
        fseek(indexTable, 0, SEEK_END);
        if (ftell(indexTable) != 0) {
            fseek(indexTable, -(INDEXER_SIZE), SEEK_END);
            fread(&indexer, INDEXER_SIZE, 1, indexTable);
            client.id = indexer.id + 1;
        } else {
            client.id = 1;
        }
    }
    client.productFirstAddress = -1;
    client.countOfProduct = 0;
    fwrite(&client, DrugStore_SIZE, 1, database);
    indexer.id = client.id;
    indexer.exists = 1;
    indexer.address = (client.id - 1) * DrugStore_SIZE;
    printf("Your drugStore id is %d\n", client.id);
    fseek(indexTable, (client.id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(database);
    fclose(indexTable);
    return 1;
}
int getDrugStore(struct DrugStore* client, int id, char* error)
{
    FILE* indexTable = fopen(DrugStore_IND, "rb");
    FILE* database = fopen(DrugStore_DATA, "rb");
    if(checkFileExists(indexTable, database, error) == 0)
        return 0;
    struct Indexer indexer;
    if(checkIndexExists(indexTable, error, id) ==0)
        return 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if (checkRecordExists(indexer, error)==0)
        return 0;
    fseek(database, indexer.address, SEEK_SET);
    fread(client, DrugStore_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}

int updateClient(struct DrugStore client, char* error, int id) {
    FILE* indexTable = fopen(DrugStore_IND, "r+b");
    FILE* database = fopen(DrugStore_DATA, "r+b");
    struct Indexer indexer;
    if (checkFileExists(indexTable, database, error)==0) {
        return 0;
    }
    if (checkIndexExists(indexTable, error, id)==0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if(checkRecordExists(indexer, error)==0)
        return 0;
    client.id = id;
    fseek(database, indexer.address, SEEK_SET);
    fwrite(&client, DrugStore_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}
void listOfDeletedClient(int id)
{
    FILE* garbage = fopen(DrugStore_GARBAGE, "rb");
    if (garbage == NULL)
    {
        printf("Error occured while opening file \n");
        return;
    }
    int garbageCount;
    fscanf(garbage, "%d", &garbageCount);
    int* deleteIds = malloc(garbageCount * sizeof(int));

    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbage, "%d", deleteIds + i);
    }
    fclose(garbage);
    garbage = fopen(DrugStore_GARBAGE, "wb");
    fprintf(garbage, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbage, " %d", deleteIds[i]);
    }
    fprintf(garbage, " %d", id);
    free(deleteIds);
    fclose(garbage);
}
int deleteClient(int id, char* error) {
    FILE* indexTable = fopen(DrugStore_IND, "r+b");
    struct Indexer indexer;
    if (indexTable == NULL) {
        strcpy(error, "database files are not created yet");
        return 0;
    }
    if (checkIndexExists(indexTable, error, id)==0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if(checkRecordExists(indexer, error)==0)
        return 0;
    struct DrugStore client;
    getDrugStore(&client, id, error);
    indexer.exists = 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(indexTable);
    listOfDeletedClient(id);


    if (client.countOfProduct !=0) {
        FILE* crewDb = fopen(DrugStore_DATA, "r+b");
        struct Product order;
        fseek(crewDb, client.productFirstAddress, SEEK_SET);
        for (int i = 0; i < order.countOfProduct; i++) {
            fread(&order, PRODUCT_SIZE, 1, crewDb);
            fclose(crewDb);
            deleteProduct(client, order, error);
            crewDb = fopen(PRODUCT_DATA, "r+b");
            fseek(crewDb, order.nextAddress, SEEK_SET);
        }
        fclose(crewDb);
    }
    return 1;
}
int PrintListOfClient(char *error)
{
    FILE* database = fopen(DrugStore_DATA, "rb");
    FILE* indexTable = fopen(DrugStore_IND, "rb");
    struct DrugStore client;
    struct Indexer indexer;
    if (checkFileExists(indexTable, database, error)==0)
    {
        return 0;
    }
    fseek(indexTable, -INDEXER_SIZE, SEEK_END);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    rewind(indexTable);
    int endIndex =0;
    getDrugStore(&client, indexer.id, error);
    endIndex = indexer.id;
    int id = 0;
    int count =0;
    while(id < endIndex)
    {
        fseek(indexTable, id *INDEXER_SIZE, SEEK_SET);
        fread(&indexer, INDEXER_SIZE, 1, indexTable);
        if(indexer.exists)
        {
            printf("<=============================>\n");
            getDrugStore(&client, indexer.id, error);
            printf("Id %d\n", client.id);
            OutputDrugStore(client);
            count++;
        }
        id++;
    }
    if (count == 0)
    {
        printf("No drugStore( \n");
        return 1;
    }
    return 1;
}


