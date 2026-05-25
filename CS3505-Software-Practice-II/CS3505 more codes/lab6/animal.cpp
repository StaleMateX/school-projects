#include <iostream>
#include <string>

using namespace std;

class Animal {
protected:
   double age;
   string name;
public:
   Animal(string name) : name(name), age(0) {};
   virtual void speak() {
      cout << "I am an Animal named " << name << endl;
   }
}; 

class Cat : public Animal {
   bool isHouseCat;
public:
    Cat(string catName, bool houseCat) : Animal(catName), isHouseCat(houseCat) {};
   virtual void speak() {
      cout << "Meow" << " and I am " << name << " and my housecatness is " << isHouseCat << endl;
   }
};