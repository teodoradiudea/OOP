#pragma once
#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

class Dog {
private:
    int id;
    string name;
    string breed;
    int age;
    string photograph;

public:
    Dog();
    Dog(int id, string name, string breed, int age, string photograph);

    // Getters
    int getId()const;
    string getBreed() const;
    string getName() const;
    int getAge() const;
    string getPhotograph() const;

    //file
    friend ostream& operator << (ostream& os, const Dog& dog);
    friend istream& operator >> (istream& is, Dog& dog);

    // Setters
    void setBreed(const string& newBreed);
    void setName(const string& newName);
    void setAge(int newAge);
    void setPhotograph(const string& newPhoto);

    //Operator overload
    bool operator==(const Dog& other) const {
        return id == other.id;
    }
};
