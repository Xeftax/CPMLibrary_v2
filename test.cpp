#include <iostream>
#include <vector>
#include <typeinfo>
#include <map>

using namespace std;

class MyClass {
static bool registered;
public:
    MyClass() {}
    virtual ~MyClass() {}

    virtual void foo() {}
};

class Factory {
public:
    template <typename T, typename... Args>
    static bool registerClass(Args&&... args) {
        vector<string> vect_args{ begin(array<string, sizeof...(Args)>{ args... }), end(array<string, sizeof...(Args)>{ args... }) };
        return registeredClasses.insert({typeid(T).name(),make_pair(&create<T>,vect_args)}).second;
    }

    MyClass* create(const vector<string>& request) {
        for (auto& createFunc : classes()) {
            MyClass* obj = createFunc(name);
            if (obj != nullptr) {
                return obj;
            }
        }
        return nullptr;
    }

private:
    // to convert create<T>(vector<string>) into create<T>(args...)
    template <typename T, typename... Args>
    static MyClass* create(const vector<string>& data, Args&... args)
    {   if data.empty() {
            return create<T>(args);
        }
        if constexpr(sizeof...(Args) > 0) {
            create<T>(data, args..., data.front());
            data.erase(data.begin());
        }
        else {
            create<T>(data, data.front()); 
            data.erase(data.begin());
        }
    }

    template <typename T, typename... Args>
    static MyClass* create(Args&&... args) {
        cout << args << endl
        return new T(args...);
    }

    static map<string,pair<MyClass* (*)(const vector<string>&, Args&&...),vector<string>>> registeredClasses;

};
bool MyClass::registered = Factory::registerClass<MyClass>();

class MyDerivedClass : public MyClass {
static bool registered;
public:
    MyDerivedClass() {}
    virtual ~MyDerivedClass() {}

    virtual void foo() override {}
};
bool MyDerivedClass::registered = Factory::registerClass<MyClass>();

int main() {

    MyClass* obj1 = Factory::create(typeid(MyClass).name());
    MyClass* obj2 = Factory::create(typeid(MyDerivedClass).name());

    std::cout << typeid(*obj1).name() << std::endl; // Output: 7MyClass
    std::cout << typeid(*obj2).name() << std::endl; // Output: 14MyDerivedClass

    delete obj1;
    delete obj2;

    return 0;
}