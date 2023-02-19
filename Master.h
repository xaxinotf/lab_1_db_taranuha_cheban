#pragma once
#include "time.h"
struct DrugStore
{
    int id;
    char Name[10];
    char description[10];
    char administratorMobileNumber[20];
    char cityName[20];
    char districtName[10];
    int countOfProduct;
    long productFirstAddress;
};
struct Indexer
{
    int id;
    int address;
    int exists;
};
