#include <stdio.h>
#include "Master.h"
#include "Input.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "DrugStore.h"
#include "Product.h"

#define DrugStore_IND "drugStore.ind"
#define INDEXER_SIZE sizeof(struct Indexer)
int main()
{
    struct DrugStore drugStore;
    struct Product product;
    while (1)
    {
        char choice[15];
        int id;
        char error[20];
        printf("Acceptable Options:\ninsert-m Insert DrugStore\nget-m Get DrugStore\nupd-m Update DrugStore\ndel-m Delete DrugStore\nut-m Print List of DrugStore\ninsert-s Insert Product\nget-s Get Product\nupd-s Update Product\ndel-s Delete Product \nut-s Output List of Products\ninfo Info about all \n exit Exit\n");
        scanf("%s", &choice);
        if (strcmp(choice, "insert-m") == 0) {
            inputDrugStore(&drugStore);
            insertDrugStore(drugStore);
        } else if (strcmp(choice, "get-m") == 0) {
            printf("Enter ID: \n");
            scanf("%d", &id);
            if (getDrugStore(&drugStore, id, error) != 0) {
                OutputDrugStore(drugStore);
            } else {
                printf("Error: %s\n", error);
            }
        } else if (strcmp(choice, "upd-m") == 0) {
            printf("Enter ID: \n");
            scanf("%d", &id);
            getDrugStore(&drugStore, id, error);
            inputDrugStore(&drugStore);
            if (updateDrugStore(drugStore, error, id) == 0) {
                printf("%s", error);
            } else {
                printf("Update was successfully\n");
            }
        } else if (strcmp(choice, "del-m") == 0) {
            printf("Enter ID: \n");
            scanf("%d", &id);
            if (deleteDrugStore(id, error) != 0) {

                printf("Deleted successfully\n");
            } else {
                printf("Error: %s\n", error);
            }
        } else if (strcmp(choice, "ut-m") ==0 ) {
            PrintListOfDrugStore(error);
        } else if (strcmp(choice, "insert-s") ==0 ) {
            printf("Enter the DrugStore id\n");
            scanf("%d", &id);
            if (getDrugStore(&drugStore, id, error) != 0) {
                product.DrugStoreId = drugStore.id;
                InputProduct(&product);
                insertProduct(drugStore, product, error);
                printf("Insert was successfully \n");
            } else {
                printf("Error %s\n", error);
            }
        } else if (strcmp(choice, "get-s") ==0 ) {
            printf("Enter the DrugStore id \n");
            scanf("%d", &id);
            if (getDrugStore(&drugStore, id, error)) {
                printf("Enter the id of product \n");
                scanf("%d", &id);
                if (checkIfRecordExist(drugStore, id, error)) {
                    getProduct(drugStore, &product, id, error);
                    OutputProduct(product, drugStore);
                } else {
                    printf("Error %s\n", error);
                }
            } else {
                printf("Error %s\n", error);
            }
        } else if (strcmp(choice, "upd-s") == 0) {
            printf("Enter the DrugStore id \n");
            scanf("%d", &id);
            if (getDrugStore(&drugStore, id, error)) {
                printf("Enter the id of product\n ");
                scanf("%d", &id);
                if (checkIfRecordExist(drugStore, id, error)) {
                    getProduct(drugStore, &product, id, error);
                    InputProduct(&product);
                    updateProduct(product);
                    printf("Updating was successfully \n ");
                } else {
                    printf("Error %s\n", error);
                }
            } else {
                printf("Error %s \n", error);
            }
        } else if (strcmp(choice, "del-s") == 0) {
            printf("Enter the DrugStore id \n");
            scanf("%d", &id);
            if (getDrugStore(&drugStore, id, error)) {
                printf("Enter the id of product \n");
                scanf("%d", &id);
                if (checkIfRecordExist(drugStore, id, error)) {
                    getProduct(drugStore, &product, id, error);
                    deleteProduct(drugStore, product, error);
                    printf("Deleted was successfully \n");
                } else {
                    printf("Error %s \n ", error);
                }
            } else {
                printf("Error %s \n ", error);
            }
        }if (strcmp(choice, "ut-s") == 0) {
            printf("Enter the DrugStore id \n");
            scanf("%d", &id);
            if (getDrugStore(&drugStore, id, error)) {
                if (drugStore.countOfProduct != 0) {
                    PrintList(drugStore);
                } else {
                    printf("This DrugStore has no products ((\n");
                }
            } else {
                printf("Error %s \n ", error);
            }
        } else if (strcmp(choice, "exit") == 0) {
            return 0;
        } if (strcmp(choice, "info") == 0) {
            info();
        }
        /*else {
            printf("Choice %s \n ", choice);
            printf("No command !!!!!!! \n");
        }*/
        printf("\n");
    }
}
