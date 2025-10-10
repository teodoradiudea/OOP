#pragma once
#include "repo.h"
//add, remove, update

class Service
{
private:
    Repository& repository;
    int nextId;

public:
    explicit Service(Repository& repo);

    //add remove update
    bool addDog(const string& name, const string& breed,
                int age, const string& photograph);
    bool removeDog(int id);
    bool updateDog(int id, const string& name, const string& breed, int age, const string& photograph);

    int validateInput(const string& name, const string& breed,
                      string& age);

    const std::vector<Dog>& getAllDogs() const;//returns dogs

    void sampleDogs();//to already have dogs when running the program
};

class UserService
{
private:
    Repository& dogRepository;
    UserRepository adoptionRepository;
    int currentDogIndex;
    std::vector<Dog> filteredDogs;
    bool isFiltered;
    string adoptionFile;
    int fileType; // 1 for TXT, 2 for CSV, 3 for HTML

public:
    explicit UserService(Repository& repo);

    const Dog* getCurrentDog() const;//returns current dog

    void nextDog();//returns next dog

    bool adoptCurrentDog();//uses repo to add dog to list

    const std::vector<Dog>& getAdoptionList() const;//returns adopted list

    //filter and clear filter for dogs
    void filterDogs(const string& breed, int maxAge);
    void clearFilter();

    bool isAdopted(const Dog& dog) const;

    int getDogCount() const;

    // Adoption file methods
    void setAdoptionFile(const string& filename, int type);
    string getAdoptionFile() const;
    int getFileType() const;
    void saveAdoptionList() const;
};
