#include "ui.h"
#include "service.h"
#include "repo.h"
#include <iostream>
#include <crtdbg.h>
using namespace std;

void principal_menu() {
    cout << "\n Keep calm and adopt a pet \n";
    cout << "Choose mode:\n";
    cout << "1. Admin\n";
    cout << "2. User\n";
    cout << "x. Exit\n";
}

int main() {
    {
        // Use file persistence
        Repository repo("dogs.txt");
        Service service(repo);

        // Only add sample dogs if the repository is empty
        if (repo.getSize() == 0) {
            service.sampleDogs();
        }

        AdminUI adminUI(service);
        UserService userService(repo);
        UserUI userUI(userService);

        while (true) {
            principal_menu();
            char choice;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case '1': {
                adminUI.run();
                break;
            }
            case '2': {
                userUI.run();
                break;
            }
            case 'x': {
                cout << "Goodbye...\n";
                return 0;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
    return 0;
}
