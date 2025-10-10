#include "service.h"
#include <fstream>
#include <exception>
#include <stdexcept>

Service::Service(Repository& repo) : repository(repo), nextId(113) {
    const auto& allDogs = repository.getAllDogs();
    for (size_t i = 0; i < allDogs.size(); i++) {
        if (allDogs[i].getId() >= nextId) {
            nextId = allDogs[i].getId() + 1;
        }
    }
}

bool Service::addDog(const string& name, const string& breed,
                     int age, const string& photograph) {
    return repository.addDog(Dog(nextId++, name, breed, age, photograph));
}

bool Service::removeDog(int id) {
    return repository.removeDog(id);
}

bool Service::updateDog(int id, const string& name,
                        const string& breed, int age,
                        const string& photograph) {
    return repository.updateDog(id, Dog(id, name, breed, age, photograph));
}

int Service::validateInput(const string& name, const string& breed,
                           string& age) {
    if (name == "" || breed == "" || age == "")
        throw std::runtime_error("Name, breed, age can't be empty\n");
    for (char c : age)
        if (!isdigit(c))
            throw std::runtime_error("Age must be integer\n");
    return stoi(age);
}

const std::vector<Dog>& Service::getAllDogs() const {
    return repository.getAllDogs();
}

void Service::sampleDogs() {

    Dog newDog1(101, "cutu1", "ham1", 2, "https://i.pinimg.com/736x/7b/3d/cd/7b3dcd0f95e6f30d2c326e0a9066a718.jpg");
    repository.addDog(newDog1);

    Dog newDog2(102, "cutu2", "ham2", 3, "https://i.pinimg.com/736x/b4/66/31/b4663179130a2ac441dcb99ef38d671c.jpg");
    repository.addDog(newDog2);

    Dog newDog3(103, "cutu3", "ham3", 1, "https://i.pinimg.com/736x/fc/14/72/fc1472274291239f5d53da36ac23e93a.jpg");
    repository.addDog(newDog3);

    Dog newDog4(104, "cutu4", "ham4", 4, "https://i.pinimg.com/736x/db/9d/ab/db9dab07f96f32d4ba333794dad86a6b.jpg");
    repository.addDog(newDog4);

    Dog newDog5(105, "linda", "ham1", 4, "https://i.pinimg.com/736x/b6/67/14/b667143861ed1542248dcf48b793ffc8.jpg");
    repository.addDog(newDog5);

    Dog newDog6(106, "durau", "ham1", 2, "https://i.pinimg.com/736x/37/06/36/370636361f3424579ae7b4693281c4c4.jpg");
    repository.addDog(newDog6);

    Dog newDog7(107, "random", "ham2", 3, "https://i.pinimg.com/736x/b2/7d/2d/b27d2d22ab6b6c7d9e7dfd862d607786.jpg");
    repository.addDog(newDog7);

    Dog newDog8(108, "bimbim", "ham3", 1, "https://i.pinimg.com/736x/de/49/0a/de490a03adadb87ae5bc9368ed5df8eb.jpg");
    repository.addDog(newDog8);

    Dog newDog9(109, "thor", "ham4", 7, "https://i.pinimg.com/736x/d0/e4/90/d0e49067be39d21a040660a96d26237f.jpg");
    repository.addDog(newDog9);

    Dog newDog10(110, "negrutu", "ham3", 1, "https://i.pinimg.com/736x/4b/13/08/4b130824e3b8eb3dcfb8c24e9f318086.jpg");
    repository.addDog(newDog10);
}


//user service

UserService::UserService(Repository& repo)
    : dogRepository(repo), currentDogIndex(0), isFiltered(false), fileType(1), adoptionFile("adoption.txt") {
}

const Dog* UserService::getCurrentDog() const {
    const std::vector<Dog>& dogs = isFiltered ? filteredDogs : dogRepository.getAllDogs();

    if (dogs.size() == 0) {
        return nullptr;
    }

    return &dogs[currentDogIndex];
}

void UserService::nextDog() {
    const std::vector<Dog>& dogs = isFiltered ? filteredDogs : dogRepository.getAllDogs();

    if (dogs.size() == 0) {
        return;
    }

    currentDogIndex = (currentDogIndex + 1) % dogs.size();
}

bool UserService::adoptCurrentDog() {
    const Dog* dog = getCurrentDog();
    if (!dog) {
        return false;
    }

    bool result = adoptionRepository.adoptDog(*dog);
    if (result) {
        saveAdoptionList();
    }
    return result;
}

const std::vector<Dog>& UserService::getAdoptionList() const {
    return adoptionRepository.getAdoptionList();
}

void UserService::filterDogs(const string& breed, int maxAge) {
    const std::vector<Dog>& allDogs = dogRepository.getAllDogs();
    filteredDogs.clear();

    for (size_t i = 0; i < allDogs.size(); i++) {
        const Dog& dog = allDogs[i];
        bool breedMatch = breed.empty() || dog.getBreed() == breed;
        bool ageMatch = dog.getAge() <= maxAge;

        if (breedMatch && ageMatch) {
            filteredDogs.push_back(dog);
        }
    }

    isFiltered = true;
    currentDogIndex = 0;
}

void UserService::clearFilter() {
    isFiltered = false;
    currentDogIndex = 0;
}

bool UserService::isAdopted(const Dog& dog) const {
    return adoptionRepository.isAdopted(dog);
}

int UserService::getDogCount() const {
    return isFiltered ? static_cast<int>(filteredDogs.size()) : static_cast<int>(dogRepository.getAllDogs().size());
}

void UserService::setAdoptionFile(const string& filename, int type) {
    adoptionFile = filename;
    fileType = type;
}

string UserService::getAdoptionFile() const {
    return adoptionFile;
}

int UserService::getFileType() const {
    return fileType;
}

void UserService::saveAdoptionList() const {
    const std::vector<Dog>& dogs = adoptionRepository.getAdoptionList();

    if (fileType == 1) { // TXT
        std::ofstream file(adoptionFile);
        if (!file.is_open()) {
            return;
        }

        for (const auto& dog : dogs) {
            file << dog << std::endl;
        }

        file.close();
    }
    else if (fileType == 2) { // CSV
        std::ofstream file(adoptionFile);
        if (!file.is_open()) {
            return;
        }

        // Write CSV header
        file << "ID,Name,Breed,Age,Photo URL" << std::endl;

        for (const auto& dog : dogs) {
            file << dog.getId() << ","
                 << dog.getName() << ","
                 << dog.getBreed() << ","
                 << dog.getAge() << ","
                 << dog.getPhotograph() << std::endl;
        }

        file.close();
    }
    else if (fileType == 3) { // HTML
        std::ofstream outFile(adoptionFile);
        if (!outFile.is_open()) {
            return;
        }

        outFile << "<!DOCTYPE html>" << std::endl;
        outFile << "<html lang=\"en\">" << std::endl;
        outFile << "<head>" << std::endl;
        outFile << "<meta name = \"viewport\" content = \"width=device-width, initial-scale=1\" > " << std::endl;
        outFile << "<title>AdoptMe</title>"<< std::endl;
        outFile << "<link rel = \"icon\" type = \"image/png\" href = \"dogg.png\">" << std::endl;
        outFile << "<link rel = \"stylesheet\" href = \"stylelist.css\">" << std::endl;
        outFile << "<link href = \"https://fonts.googleapis.com/css2?family=Cherry+Bomb+One&family=Knewave&display=swap\" rel = \"stylesheet\">" << std::endl;
        outFile << "</head>" << std::endl;
        outFile << "<body>" << std::endl;
        outFile << "    <div class=\"header\">" << std::endl;
        outFile << "        Adoption List" << std::endl;
        outFile << "    </div>" << std::endl;
        outFile << "    <div class=\"content\">" << std::endl;
        outFile << "        <div id=\"gallery-container\">" << std::endl;

        for (const auto& dog : dogs) {
            outFile << "            <div class=\"gallery\">" << std::endl;
            outFile << "                <img src=\"" << dog.getPhotograph() << "\">" << std::endl;
            outFile << "                <div class=\"desc\">#" << dog.getId() << "</div>" << std::endl;
            outFile << "                <div class=\"desc\">" << dog.getName() << "</div>" << std::endl;
            outFile << "                <div class=\"desc\">" << dog.getBreed() << "</div>" << std::endl;
            outFile << "                <div class=\"desc\">" << dog.getAge() << " years</div>" << std::endl;
            outFile << "            </div>" << std::endl;
        }

        outFile << "        </div>" << std::endl;
        outFile << "    </div>" << std::endl;
        outFile << "    <div class=\"footer\">" << std::endl;
        outFile << "        <p>@This is your adoption list containing all details about your adopted puppies.</p>" << std::endl;
        outFile << "    </div>" << std::endl;
        outFile << "</body>" << std::endl;
        outFile << "</html>" << std::endl;

        outFile.close();
    }
}
