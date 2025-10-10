#pragma warning(disable: 6031)
#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"

void printMenu() {
    printf("\n0. Display all countries\n");
    printf("1. Add Country\n");
    printf("2. Update Country Population\n");
    printf("3. Update Country Continent\n");
    printf("4. Remove Country\n");
    printf("5. Update in case of migration\n");
    printf("6. Display countries by name(sorted)\n");
    printf("7. Display countries by continent and minimum population(sorted)\n");
    printf("8. Undo\n");
    printf("9. Redo\n");
    printf("x. Exit\n");
    printf("Choose an option: ");
}


void start(Service* serv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    generate_countries(serv);
    printf("Welcome back to the world population monitoring organisation!\n");

    char choice, bait_char;
    char name[25], continent[25], migrate[25], str[25];
    long population;

    do {
        printMenu();
        scanf(" %c", &choice);
        while (getchar() != '\n');

        switch (choice) {
        case '0':
            if (serv->repo->size == 0)
                printf("No countries\n");
            else
                print_countries(serv);
            break;
        case '1':
            printf("Enter country name: ");
            fgets(name, 25, stdin);
            name[strlen(name) - 1] = '\0';

            printf("Enter continent: ");
            fgets(continent, 25, stdin);
            continent[strlen(continent) - 1] = '\0';

            printf("Enter population: ");
            while (scanf("%ld", &population) != 1) {
                while (getchar() != '\n');
                printf("Invalid population. Please enter a valid number: ");
            }

            if (AddCountry(serv, name, continent, population))
                printf("Country added successfully.\n");
            else
                printf("Failed to add country.\n");
            break;
        case '2':
            printf("Enter country name: ");
            fgets(name, 25, stdin);
            name[strlen(name) - 1] = '\0';

            printf("Enter new population: ");
            while (scanf("%ld", &population) != 1) {
                while (getchar() != '\n');
                printf("Invalid population. Please enter a valid number: ");
            }

            if (UpdateCountryPopulation(serv, name, population))
                printf("Population updated successfully.\n");
            else
                printf("Country not found.\n");
            break;

        case '3':
            printf("Enter country name: ");
            fgets(name, 25, stdin);
            name[strlen(name) - 1] = '\0';

            printf("Enter new continent: ");
            fgets(continent, 25, stdin);
            continent[strlen(continent) - 1] = '\0';

            if (UpdateCountryContinent(serv, name, continent))
                printf("Continent updated successfully.\n");
            else
                printf("Country not found.\n");
            break;

        case '4':
            printf("Enter country name: ");
            fgets(name, 25, stdin);
            name[strlen(name) - 1] = '\0';

            if (RemoveCountry(serv, name))
                printf("Country removed successfully.\n");
            else
                printf("Country not found.\n");
            break;

        case '5':
            printf("Enter the country where the migrants are from: ");
            fgets(name, 25, stdin);
            name[strlen(name) - 1] = '\0';
            printf("Enter the country where they emigrate: ");
            fgets(str, 25, stdin);
            str[strlen(str) - 1] = '\0';

            printf("Enter population: ");
            while (scanf("%ld", &population) != 1) {
                while (getchar() != '\n');
                printf("Invalid population. Please enter a valid number: ");
            }

            if (UpdateMigration(serv, name, str, population))
                printf("Population migrated!\n");
            else
                printf("Migration failed!\n");
            break;

        case '6':
            printf("Enter country name/partial name: ");
            fgets(str, 25, stdin);
            str[strlen(str) - 1] = '\0';

            DynamicArray* countries = DisplayByName(serv, str);
            if (countries) {
                print_all(countries);
                DestroyDynamicArray(countries);
            }
            else {
                printf("No countries found.\n");
            }
            break;

        case '7':
            printf("Enter continent name/partial name: ");
            fgets(str, 25, stdin);
            str[strlen(str) - 1] = '\0';

            printf("Enter the minimum population: ");
            while (scanf("%ld", &population) != 1) {
                while (getchar() != '\n');
                printf("Invalid population. Please enter a valid number: ");
            }

            DynamicArray* continents = DisplayByContinent(serv, str, population);
            if (continents) {
                print_all(continents);
                DestroyDynamicArray(continents);
            }
            else {
                printf("No countries found.\n");
            }
            break;

        case '8':
            if (Undo(serv))
                printf("Undo operation successful.\n");
            else
                printf("No operation to undo.\n");
            break;

        case '9':
            if (Redo(serv))
                printf("Redo operation successful.\n");
            else
                printf("No operation to redo.\n");
            break;

        case 'x':
            printf("Exiting...\n");
            DestroyService(serv);
            _CrtDumpMemoryLeaks();
            break;

        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (choice != 'x');
} 