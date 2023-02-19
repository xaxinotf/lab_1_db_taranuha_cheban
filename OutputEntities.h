#pragma once
#include "Master.h"

void OutputDrugStore(struct DrugStore drugStore)
{
    printf("DrugStore's first name: %s\n", drugStore.Name);
    printf("DrugStore's last name: %s\n", drugStore.description);
    printf("DrugStore's districtName: %s\n", drugStore.districtName);
    printf("DrugStore's mobile number: %s\n", drugStore.administratorMobileNumber);
    printf("DrugStore's cityName: %s\n", drugStore.cityName);

}

void OutputProduct(struct Product product, struct DrugStore drugStore)
{
    printf("The drugStore id %d \n", drugStore.id);
    printf("DrugStore name and description : %s, %s \n", drugStore.Name, drugStore.description);
    printf("Product count of product: %d\n", product.countOfProduct);
    printf("Product available: %d\n", product.available);
    printf("Product deliver time: %s\n", product.date);
}
