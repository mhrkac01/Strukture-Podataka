#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE 1024
#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _city;
typedef struct _city* cityPosition;
typedef struct _city {
    char cityName[MAX_LINE];
    int population;

    cityPosition left;
    cityPosition right;
} City;

struct _country;
typedef struct _country* countryPosition;
typedef struct _country {
    char countryName[MAX_LINE];
    cityPosition root;
    countryPosition next;
} Country;

int readCountries(FILE*, countryPosition);
int createCountryNode(countryPosition, char*, char*);
cityPosition readCityFile(cityPosition, char*);
cityPosition makeCityTree(cityPosition, char*, int);
void printCountries(countryPosition);
void findAndPrintCities(cityPosition, int);
int printCities(countryPosition, char*, int);
void freeCityTree(cityPosition);
void freeCountryList(countryPosition);

int main() {
    char searchCountry[MAX_LINE];
    int searchPopulation;

    FILE* countries = fopen("countries.txt", "r");
    if (countries == NULL) {
        printf("File could not be opened.\n");
        return FILE_OPEN_ERROR;
    }

    countryPosition countryHead = (countryPosition)malloc(sizeof(Country));
    if (countryHead == NULL) {
        printf("There is not enough memory!\n");
        fclose(countries);
        return MALLOC_ERROR;
    }

    strcpy(countryHead->countryName, "");
    countryHead->root = NULL;
    countryHead->next = NULL;

    if (readCountries(countries, countryHead) != EXIT_SUCCESS) {
        printf("Failed to read countries.\n");
        freeCountryList(countryHead);
        fclose(countries);
        return FILE_OPEN_ERROR;
    }

    printCountries(countryHead);

    printf("\nUnesite zeljenu drzavu:");
    if (scanf("%s", searchCountry) != 1) {
        printf("Invalid input for country name.\n");
        freeCountryList(countryHead);
        fclose(countries);
        return EXIT_FAILURE;
    }

    printf("\nUnesite broj stanovnika:");
    if (scanf("%d", &searchPopulation) != 1) {
        printf("Invalid input for population.\n");
        freeCountryList(countryHead);
        fclose(countries);
        return EXIT_FAILURE;
    }

    fclose(countries);

    printCities(countryHead, searchCountry, searchPopulation);

    freeCountryList(countryHead);
    return EXIT_SUCCESS;
}

int readCountries(FILE* countries, countryPosition head) {
    char tempCountryName[MAX_LINE];
    char tempFileName[MAX_LINE];
    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, countries) != NULL) {
        if (sscanf(line, "%[^,], %s", tempCountryName, tempFileName) != 2) {
            printf("Error parsing line: %s\n", line);
            return FILE_OPEN_ERROR;
        }
        if (createCountryNode(head, tempCountryName, tempFileName) != EXIT_SUCCESS) {
            return MALLOC_ERROR;
        }
    }

    return EXIT_SUCCESS;
}

int createCountryNode(countryPosition head, char* tempCountryName, char* tempFileName) {
    countryPosition newCountry = (countryPosition)malloc(sizeof(Country));
    if (newCountry == NULL) {
        printf("There is not enough memory!\n");
        return MALLOC_ERROR;
    }

    strcpy(newCountry->countryName, tempCountryName);
    newCountry->root = readCityFile(NULL, tempFileName);
    newCountry->next = NULL;

    countryPosition temp = head;
    while (temp->next != NULL && strcmp(temp->next->countryName, tempCountryName) < 0) {
        temp = temp->next;
    }

    newCountry->next = temp->next;
    temp->next = newCountry;

    return EXIT_SUCCESS;
}

cityPosition readCityFile(cityPosition root, char* tempFileName) {
    char tempCityName[MAX_LINE];
    int tempCityPopulation;

    FILE* cityFilePointer = fopen(tempFileName, "r");
    if (cityFilePointer == NULL) {
        printf("File %s could not be opened.\n", tempFileName);
        return NULL;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, cityFilePointer) != NULL) {
        if (sscanf(line, "%[^,], %d", tempCityName, &tempCityPopulation) != 2) {
            printf("Error parsing line: %s\n", line);
            continue;
        }
        root = makeCityTree(root, tempCityName, tempCityPopulation);
    }

    fclose(cityFilePointer);
    return root;
}

cityPosition makeCityTree(cityPosition root, char* cityName, int population) {
    if (root == NULL) {
        cityPosition newCity = (cityPosition)malloc(sizeof(City));
        if (newCity == NULL) {
            printf("There is not enough memory!\n");
            return NULL;
        }

        strcpy(newCity->cityName, cityName);
        newCity->population = population;
        newCity->left = NULL;
        newCity->right = NULL;

        return newCity;
    }

    if (population < root->population ||
        (population == root->population && strcmp(cityName, root->cityName) < 0)) {
        root->left = makeCityTree(root->left, cityName, population);
    }
    else {
        root->right = makeCityTree(root->right, cityName, population);
    }

    return root;
}

void printCountries(countryPosition head) {
    countryPosition temp = head->next;
    while (temp != NULL) {
        printf("%s\t", temp->countryName);
        temp = temp->next;
    }
}

void findAndPrintCities(cityPosition root, int population) {
    if (root == NULL)
        return;

    findAndPrintCities(root->right, population);

    if (root->population >= population) {
        printf("City: %s\tPopulation: %d\n", root->cityName, root->population);
    }

    findAndPrintCities(root->left, population);
}

int printCities(countryPosition head, char* country, int population) {
    countryPosition temp = head->next;
    countryPosition foundCountry = NULL;

    while (temp != NULL) {
        if (strcmp(temp->countryName, country) == 0) {
            foundCountry = temp;
            break;
        }
        temp = temp->next;
    }

    if (foundCountry == NULL) {
        printf("No such country was found.\n");
        return 1;
    }

    findAndPrintCities(foundCountry->root, population);
    return 0;
}

void freeCityTree(cityPosition root) {
    if (root == NULL)
        return;

    freeCityTree(root->left);
    freeCityTree(root->right);
    free(root);
}

void freeCountryList(countryPosition head) {
    countryPosition temp = head;
    while (temp != NULL) {
        countryPosition next = temp->next;
        freeCityTree(temp->root);
        free(temp);
        temp = next;
    }
}