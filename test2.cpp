#include <iostream>
#include <string>

using namespace std;

class Abstract {
    public:
    struct Person {
        void test() {cout << "test" << endl;}
    };
};

class Concrete : Abstract {
    public:
    Concrete(int id,Abstract::Person* wife) : mPerson{{},wife} {}
    struct Person : Abstract::Person {
        Abstract::Person* wife;
    };
    private:
    Person mPerson;
};

int main() {
    // create a Person struct and initialize all values
    Abstract::Person p1 = {};
    Concrete::Person p2 = {{}, &p1};
    
    // print the values of the struct
    
    return 0;
}