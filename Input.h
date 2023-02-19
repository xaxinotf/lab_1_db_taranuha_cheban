#pragma once
#include <stdio.h>
#include <string.h>
#include "Master.h"
#include "time.h"
#include "Slave.h"
void inputDrugStore(struct DrugStore *drugStore)
{
    char Name[10];
    char description[10];
    char administratorMobileNumber[20];
    char cityName[20];
    char districtName[20];

    printf("Enter drugStore's Name: ");
    scanf("%s", Name);
    strcpy(drugStore->Name, Name);

    printf("Enter drugStore's Description: ");
    scanf("%s", description);
    strcpy(drugStore->description, description);

    printf("Enter drugStore's administrator Number: ");
    scanf("%s", administratorMobileNumber);
    strcpy(drugStore->administratorMobileNumber, administratorMobileNumber);

    printf("Enter drugStore's city Name: ");
    scanf("%s", cityName);
    strcpy(drugStore->cityName, cityName);

    printf("Enter drugStore's district Name: ");
    scanf("%s", districtName);
    strcpy(drugStore->districtName, districtName);
}

void InputProduct(struct Product *product)
{
    int countOfProduct;
    int available;
    char date[12];

    printf("Enter Product's count: ");
    scanf("%d", &countOfProduct);
    product->countOfProduct = countOfProduct;

    printf("Enter available 's Product: ");
    scanf("%d", &available);
    product->available = available;

    printf("Enter product's date: ");
    scanf("%s", &date);
    strcpy(product->date, date);
}