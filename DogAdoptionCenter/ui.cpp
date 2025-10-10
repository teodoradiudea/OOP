#include <iostream>
#include <string.h>
#include <limits>
#include "ui.h"
#include <vector>

using namespace std;

AdminUI::AdminUI(Service& service) : service(service) {}

void AdminUI::run() {
    bool ok = true;

    while (ok) {
        adminMenu();
        char choice;

        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (choice) {
        case '1':
            printAllDogs();
            break;
        case '2':
            addDog();
            break;
        case '3':
            removeDog();
            break;
        case '4':
            updateDog();
            break;
        case 'x':
            ok = false;
            cout << "Exiting administrator mode..\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void AdminUI::adminMenu() {
    cout << "\n   Welcome to administrator mode!" << endl;
    cout << " Choose one from below" << endl;
    cout << "1. See all dogs from the shelter" << endl;
    cout << "2. Add new dog to the shelter" << endl;
    cout << "3. Remove info/availablity for adopted dogs" << endl;
    cout << "4. Update info about one of the dogs" << endl;
    cout << "x. exit mode" << endl;
}

void AdminUI::addDog() {
    cout << "\nAdd new dog\n";

    string name, breed, photo, age;
    cout << "Enter dog name: ";
    getline(cin, name);

    cout << "Enter dog breed: ";
    getline(cin, breed);

    cout << "Enter dog age: ";
    getline(cin, age);

    cout << "Enter dog image: ";
    getline(cin, photo);

    try
    {
        int a = service.validateInput(name, breed, age);
        bool success = service.addDog(name, breed, a, photo);
        if (success) {
            cout << "Dog added successfully!\n";
        }
        else {
            cout << "Error: Dog with this id already exists!\n";
        }
    }
    catch (exception& e)
    {
        cout << e.what();
    }
}

void AdminUI::removeDog() {
    cout << "\nRemove dog\n";

    int id;
    cout << "Enter dog id: ";
    while (!(cin >> id)) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    bool success = service.removeDog(id);

    if (success) {
        cout << "Dog removed successfully!\n";
    }
    else {
        cout << "Error: Dog with id " << id << " does not exist!\n";
    }
}

void AdminUI::updateDog() {
    cout << "\nUpdate dog info\n";
    string name, breed, photo;
    int id, age;

    cout << "Enter dog id: ";
    while (!(cin >> id)) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    cout << "Enter new dog name: ";
    getline(cin, name);

    cout << "Enter new dog breed: ";
    getline(cin, breed);

    cout << "Enter new dog age: ";
    while (!(cin >> age)) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    cout << "Enter new dog photo: ";
    getline(cin, photo);

    bool success = service.updateDog(id, name, breed, age, photo);

    if (success) {
        cout << "Dog information updated successfully!\n";
    }
    else {
        cout << "Error: Dog with id " << id << " does not exist!\n";
    }
}

void AdminUI::printAllDogs() {
    std::vector<Dog> allDogs = service.getAllDogs();

    if (allDogs.empty()) {
        cout << "No dogs in the shelter :( \n";
        return;
    }

    cout << "\ndogy dogs\n";
    for (size_t i = 0; i < allDogs.size(); i++) {
        Dog dog = allDogs[i];
        cout << "dog #" << (i + 1) << ":\n";
        cout << "id: " << dog.getId() << "\n";
        cout << "name: " << dog.getName() << "\n";
        cout << "breed: " << dog.getBreed() << "\n";
        cout << "age: " << dog.getAge() << " years\n";
        cout << "image: " << dog.getPhotograph() << "\n\n";
    }
}

UserUI::UserUI(UserService& service) : service(service), fileTypeSelected(false) {}

void UserUI::userMenu() {
    // Only ask for file type once when entering user mode
    if (!fileTypeSelected) {
        cout << "Choose file type for adoption list:\n";
        cout << "1. CSV file\n";
        cout << "2. HTML file\n";

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 2) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            cout << "Invalid choice. Please enter 1 or 2: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        // Map user choice 1->2 and 2->3 for internal file types
        // (since internally 2=CSV and 3=HTML)
        if (choice == 1) {
            service.setAdoptionFile("adoption.csv", 2); // CSV
        }
        else if (choice == 2) {
            service.setAdoptionFile("adoption.html", 3); // HTML
        }

        fileTypeSelected = true;
    }

    cout << "\n   Welcome to user mode!" << endl;
    cout << "Choose one from below" << endl;
    cout << "1. See available dogs to adopt" << endl;
    cout << "2. Filter dogs by breed and age" << endl;
    cout << "3. See adoption list" << endl;
    cout << "4. Open adoption file" << endl;
    cout << "x. exit mode" << endl;
}

void UserUI::run() {
    bool ok = true;

    while (ok) {
        userMenu();
        char choice;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (choice) {
        case '1':
            viewDog();
            break;
        case '2':
            filterDog();
            break;
        case '3':
            viewAdoptionList();
            break;
        case '4':
            openAdoptionFile();
            break;
        case 'x':
            ok = false;
            fileTypeSelected = false; // Reset for next time user mode is entered
            cout << "Exiting user mode..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
}

void UserUI::viewDog() {
    if (service.getDogCount() == 0) {
        cout << "No dogo available." << endl;
        return;
    }

    bool browsing = true;
    while (browsing) {
        const Dog* currentDog = service.getCurrentDog();
        if (!currentDog) {
            cout << "No dogo available." << endl;
            return;
        }

        dogSlideshow(currentDog);

        cout << endl << "Choose one:" << endl;
        cout << "1. Adopt this dog" << endl;
        cout << "2. Next" << endl;
        cout << "x. Back to menu" << endl;

        char choice;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (choice) {
        case '1':
            if (service.adoptCurrentDog()) {
                cout << "Ham adopted successfully!" << endl;
                // Automatically move to the next dog after adoption
                service.nextDog();
            }
            else {
                cout << "You've already adopted this dog." << endl;
            }
            break;
        case '2':
            service.nextDog();
            break;
        case 'x':
            browsing = false;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
}

void UserUI::filterDog() {
    cout << endl << "Filter Dogs" << endl;

    string breed;
    cout << "Enter breed (empty for all breeds): ";
    getline(cin, breed);

    int maxAge;
    cout << "Enter max age: ";
    while (!(cin >> maxAge)) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    service.filterDogs(breed, maxAge);

    if (service.getDogCount() == 0) {
        cout << "No filter like this." << endl;
        service.clearFilter();
    }
    else {
        cout << "Filter applied. " << service.getDogCount() << " dogs found." << endl;
        viewDog();
        service.clearFilter();
    }
}

void UserUI::viewAdoptionList() {
    const std::vector<Dog>& adoptedDogs = service.getAdoptionList();

    if (adoptedDogs.size() == 0) {
        cout << "Adoption list is empty." << endl;
        return;
    }

    cout << endl << " Adoption List" << endl;
    for (size_t i = 0; i < adoptedDogs.size(); i++) {
        const Dog& dog = adoptedDogs[i];
        cout << "hamham #" << (i + 1) << ":" << endl;
        cout << "  id: " << dog.getId() << "" << endl;
        cout << "  name: " << dog.getName() << "" << endl;
        cout << "  breed: " << dog.getBreed() << "" << endl;
        cout << "  age: " << dog.getAge() << " yrs" << endl;
        cout << "  image: " << dog.getPhotograph() << endl << "" << endl;
    }
}

void UserUI::openAdoptionFile() {
    string filename = service.getAdoptionFile();
    int fileType = service.getFileType();

    // Make sure the file is saved before opening
    service.saveAdoptionList();

    if (filename.empty()) {
        cout << "No adoption file available." << endl;
        return;
    }

    // On Windows, use the 'start' command to open the file with the default application
#ifdef _WIN32
    string command = "start " + filename;
#else
// On Unix-like systems, use 'xdg-open' or 'open' (macOS)
#ifdef __APPLE__
    string command = "open " + filename;
#else
    string command = "xdg-open " + filename;
#endif
#endif

    cout << "Opening " << filename << "..." << endl;
    system(command.c_str());
}

void UserUI::dogSlideshow(const Dog* dog) {
    cout << endl << "ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham * ham" << endl;
    cout << "name: " << dog->getName() << "" << endl;
    cout << "breed: " << dog->getBreed() << "" << endl;
    cout << "age: " << dog->getAge() << " years" << endl;
    cout << "image: " << dog->getPhotograph() << "" << endl;
}