#pragma once
#include <vector>
#include <string>
#include "domain.h"

class Repository {
protected:
    std::vector<Dog> dogs; ///Internal storage for Dog objects
    std::string filename; ///Filename for persistence
    bool useFile; ///Flag to determine if file persistence is enabled

    // File operations
    void loadFromFile();
    void saveToFile() const;

public:
    // Constructors
    Repository();
    explicit Repository(const std::string& filename);

    //add remove update
    bool addDog(const Dog& dog);
    bool removeDog(int id);
    bool updateDog(int id, const Dog& newDogData);

    const std::vector<Dog>& getAllDogs() const;//returns dog list

    //find, check if dog exists
    Dog* findDogById(int id);
    const Dog* findDogById(int id) const;
    bool exists(int id) const;

    int getSize() const;//ret size
};

class UserRepository {
private:
    std::vector<Dog> adoptionList;
    std::string filename; ///Filename for persistence
    bool useFile; ///Flag to determine if file persistence is enabled
    int fileType; // 1 for TXT, 2 for CSV, 3 for HTML

    // Helper methods for different file formats
    void saveToTxtFile() const;
    void saveToCSVFile() const;
    void saveToHtmlFile() const;
    void loadFromTxtOrCSVFile();

public:
    UserRepository();
    UserRepository(const std::string& filename, int fileType);

    //adopt dog, check if adopted, return adoption list, return list size
    bool adoptDog(const Dog& dog);
    const std::vector<Dog>& getAdoptionList() const;
    bool isAdopted(const Dog& dog) const;
    int getSize() const;

    // File operations
    void saveToFile() const;
    void loadFromFile();
    std::string getFilename() const { return filename; }
    int getFileType() const { return fileType; }
};
