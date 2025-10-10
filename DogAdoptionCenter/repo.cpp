#include "repo.h"
#include <algorithm>
#include <fstream>
#include <sstream>

// Repository constructors
Repository::Repository() : useFile(false) {}

Repository::Repository(const std::string& filename) : filename(filename), useFile(true) {
    loadFromFile();
}

// File operations
void Repository::loadFromFile() {
    if (!useFile) return;

    std::ifstream file(filename);
    if (!file.is_open()) return; // File doesn't exist yet

    dogs.clear();
    Dog dog;

    while (file >> dog) {
        dogs.push_back(dog);
    }

    file.close();
}

void Repository::saveToFile() const {
    if (!useFile) return;

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    for (const auto& dog : dogs) {
        file << dog << std::endl;
    }

    file.close();
}

bool Repository::addDog(const Dog& dog) {
    if (exists(dog.getId()))
        return false;
    dogs.push_back(dog);

    if (useFile) {
        saveToFile();
    }

    return true;
}

bool Repository::removeDog(int id) {
    for (size_t i = 0; i < dogs.size(); i++) {
        if (dogs[i].getId() == id) {
            dogs.erase(dogs.begin() + i);

            if (useFile) {
                saveToFile();
            }

            return true;
        }
    }
    return false;
}

bool Repository::updateDog(int id, const Dog& newDogData) {
    Dog* dog = findDogById(id);
    if (!dog) return false;
    *dog = newDogData;

    if (useFile) {
        saveToFile();
    }

    return true;
}

const std::vector<Dog>& Repository::getAllDogs() const {
    return dogs;
}

Dog* Repository::findDogById(int id) {
    for (size_t i = 0; i < dogs.size(); i++) {
        if (dogs[i].getId() == id)
            return &dogs[i];
    }
    return nullptr;
}

const Dog* Repository::findDogById(int id) const {
    for (size_t i = 0; i < dogs.size(); i++) {
        if (dogs[i].getId() == id)
            return &dogs[i];
    }
    return nullptr;
}

bool Repository::exists(int id) const {
    return findDogById(id) != nullptr;
}

int Repository::getSize() const {
    return static_cast<int>(dogs.size());
}


//user repo
UserRepository::UserRepository() : useFile(false), fileType(0) {}

UserRepository::UserRepository(const std::string& filename, int fileType)
    : filename(filename), useFile(true), fileType(fileType) {
    loadFromFile();
}

void UserRepository::loadFromFile() {
    if (!useFile) return;

    if (fileType == 1 || fileType == 2) { // TXT or CSV
        loadFromTxtOrCSVFile();
    }
    // For HTML, we don't need to load anything as it's just for display
}

void UserRepository::loadFromTxtOrCSVFile() {
    std::ifstream file(filename);
    if (!file.is_open()) return; // File doesn't exist yet

    adoptionList.clear();
    Dog dog;

    while (file >> dog) {
        adoptionList.push_back(dog);
    }

    file.close();
}

void UserRepository::saveToFile() const {
    if (!useFile) return;

    switch (fileType) {
    case 1: // TXT
        saveToTxtFile();
        break;
    case 2: // CSV
        saveToCSVFile();
        break;
    case 3: // HTML
        saveToHtmlFile();
        break;
    default:
        saveToTxtFile(); // Default to TXT
        break;
    }
}

void UserRepository::saveToTxtFile() const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    for (const auto& dog : adoptionList) {
        file << dog << std::endl;
    }

    file.close();
}

void UserRepository::saveToCSVFile() const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    // Write CSV header
    file << "ID,Name,Breed,Age,Photo URL" << std::endl;

    for (const auto& dog : adoptionList) {
        file << dog.getId() << ","
             << dog.getName() << ","
             << dog.getBreed() << ","
             << dog.getAge() << ","
             << dog.getPhotograph() << std::endl;
    }

    file.close();
}

void UserRepository::saveToHtmlFile() const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open HTML file for writing: " + filename);
    }

    // Write the HTML header
    outFile << "<!DOCTYPE html>\\n"
            << "<html lang=en>\\n"
            << "<head>\\n"
            << "    <meta name=viewport content=width=device-width, initial-scale=1>\\n"
                          << "    <title>AdoptMe - Adoption List</title>\\n"
                          << "    <link rel=\"icon\" type=\"image/png\" href=\"dogg.png\">\\n"
                          << "    <style>\\n"
                          << "        body { font-family: Arial, sans-serif; margin: 0; padding: 0; }\\n"
                          << "        .header { background-color: #4CAF50; color: white; text-align: center; padding: 1em; }\\n"
                          << "        .content { padding: 20px; }\\n"
                          << "        .gallery { margin: 5px; border: 1px solid #ccc; float: left; width: 180px; }\\n"
                          << "        .gallery img { width: 100%; height: auto; }\\n"
                          << "        .desc { padding: 15px; text-align: center; }\\n"
                          << "        .footer { background-color: #f1f1f1; text-align: center; padding: 10px; clear: both; }\\n"
                          << "    </style>\\n"
                          << "</head>\\n"
                          << "<body>\\n"
                          << "    <div class=\"header\">\\n"
                          << "        Adoption List\\n"
                          << "    </div>\\n"
                          << "    <div class=\"content\">\\n"
                          << "        <div id=\"gallery-container\">\\n";

    // Add each dog to the HTML
    for (const auto& dog : adoptionList) {
        outFile << "            <div class=\"gallery\">\\n"
                << "                <img src=\"" << dog.getPhotograph() << "\">\\n"
                << "                <div class=\"desc\">#" << dog.getId() << "</div>\\n"
                << "                <div class=\"desc\">" << dog.getName() << "</div>\\n"
                << "                <div class=\"desc\">" << dog.getBreed() << "</div>\\n"
                << "                <div class=\"desc\">" << dog.getAge() << " years</div>\\n"
                << "            </div>\\n";
    }

    // Write the HTML footer
    outFile << "        </div>\\n"
            << "    </div>\\n"
            << "    <div class=\"footer\">\\n"
            << "        <p>@This is your adoption list containing all details about your adopted puppies.</p>\\n"
            << "    </div>\\n"
            << "</body>\\n"
            << "</html>";

    outFile.close();
}

bool UserRepository::adoptDog(const Dog& dog) {
    if (isAdopted(dog))
        return false;
    adoptionList.push_back(dog);

    if (useFile) {
        saveToFile();
    }

    return true;
}

const std::vector<Dog>& UserRepository::getAdoptionList() const {
    return adoptionList;
}

bool UserRepository::isAdopted(const Dog& dog) const {
    for (size_t i = 0; i < adoptionList.size(); i++) {
        if (adoptionList[i].getId() == dog.getId())
            return true;
    }
    return false;
}

int UserRepository::getSize() const {
    return static_cast<int>(adoptionList.size());
}
