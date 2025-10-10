#include "domain.h"
#include <sstream>

Dog::Dog() : id(0), name(""), breed(""), age(0), photograph("") {
	//default values
}

Dog::Dog(int id, string name, string breed, int age, string photograph) {
	this->id = id;
	this->name = name;
	this->breed = breed;
	this->age = age;
	this->photograph = photograph;
}

//getters
int Dog::getId() const {
	return id;
}
string Dog::getBreed() const
{
	return breed;
}

string Dog::getName() const
{
	return name;
}

int Dog::getAge() const
{
	return age;
}

string Dog::getPhotograph() const
{
	return photograph;
}

//file
ostream& operator << (ostream& os, const Dog& dog)
{
	os << dog.id << ',' << dog.name << ',' << dog.breed << ',' << dog.age << ',' << dog.photograph;
	return os;
}

istream& operator >> (istream& is, Dog& dog)
{
	string line;
	if (getline(is, line)) {
		stringstream ss(line);
		string token;

		getline(ss, token, ',');
		dog.id = stoi(token);

		getline(ss, dog.name, ',');

		getline(ss, dog.breed, ',');

		getline(ss, token, ',');
		dog.age = stoi(token);

		getline(ss, dog.photograph);
	}
	return is;
}

//setters
void Dog::setBreed(const string& newBreed)
{
	breed = newBreed;
}

void Dog::setName(const string& newName)
{
	name = newName;
}

void Dog::setAge(int newAge)
{
	age = newAge;
}

void Dog::setPhotograph(const string& newPhoto)
{
	photograph = newPhoto;
}