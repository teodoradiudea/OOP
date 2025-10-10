#pragma once
#include "service.h"

//admin ui
class AdminUI {
private:
    Service& service;

    void adminMenu();//admin menu

    //add remove update dog
    void addDog();
    void removeDog();
    void updateDog();

    void printAllDogs();//display dogs

public:
    AdminUI(Service& service);
    void run();
};


//user ui
class UserUI {
private:
    UserService& service;
    bool fileTypeSelected; // Flag to track if file type has been selected

    void userMenu();//user menu
    void viewDog();//dog menu
    void filterDog();//filter by breed and age
    void viewAdoptionList();//view adoption list
    void openAdoptionFile();//open adoption file
    void dogSlideshow(const Dog* dog);//display dog one by one

public:
    UserUI(UserService& service);
    void run();
};