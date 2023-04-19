// Prototype Design Pattern
//
// Prototype is a creational design pattern that allows cloning objects, 
// even complex ones, without coupling to their specific classes.

// All prototype classes should have a common interface that makes it 
// possible to copy objects even if their concrete classes are unknown. 
// Prototype objects can produce full copies since objects of the same 
// class can access each other’s private fields.

// Prototype Design Pattern permits copying existing objects 
// without making the code dependent on their classes.

// https://refactoring.guru/design-patterns/prototype

#include <string>
#include <iostream>
#include <unordered_map>

using std::string;

enum Type 
{
    PROTOTYPE_1 = 0,
    PROTOTYPE_2
};

// The example class that has cloning ability. 
// We'll see how the values of fields with different types will be cloned.

// Create the prototype interface and declare the clone method in it.
class Prototype 
{
    protected:
        string prototype_name_;
        float prototype_field_;

    public:
        Prototype() : prototype_name_(""), prototype_field_(0.0) {}
        Prototype(string prototype_name) : prototype_name_(prototype_name), prototype_field_(0.0) {}
        virtual ~Prototype() {}

        // Create the prototype interface and declare the clone method in it
        virtual Prototype* Clone() const = 0;

        // 
        virtual void Method(float prototype_field) 
        {
            this->prototype_field_ = prototype_field;
            std::cout << "Call Method from " << prototype_name_ << " with field : " << prototype_field << std::endl;
        }
};

// ConcretePrototype1 is a Sub-Class of Prototype that implements the Clone Method
// In this example all data members of Prototype Class are in the Stack. 
// If you have pointers in your properties, for example String* name_, you will need to
// implement the Copy-Constructor to make sure you have a deep copy from the clone method

class ConcretePrototype1 : public Prototype 
{
    private:
        float concrete_prototype_field1_;

    public:
        ConcretePrototype1(string prototype_name, float concrete_prototype_field)
            : Prototype(prototype_name), concrete_prototype_field1_(concrete_prototype_field) { }

        // The cloning method usually consists of just one line: running a new operator 
        // with the prototypical version of the constructor. 
        // Note, that every class must explicitly override the cloning method and 
        // use its own class name along with the new operator. 
        // Otherwise, the cloning method may produce an object of a parent class.

        // Notice that Clone method returns a Pointer to a new ConcretePrototype1 replica. 
        // So, the client (who calls the clone method) has the responsibility to free that memory. 
        // If you have smart pointer knowledge you may prefer to use unique_pointer here.
        Prototype* Clone() const override 
        {
            return new ConcretePrototype1(*this);
        }
};

class ConcretePrototype2 : public Prototype 
{
    private:
        float concrete_prototype_field2_;

    public:
        ConcretePrototype2(string prototype_name, float concrete_prototype_field)
            : Prototype(prototype_name), concrete_prototype_field2_(concrete_prototype_field) { }

        Prototype* Clone() const override 
        {
            return new ConcretePrototype2(*this);
        }
};

// Optionally, create a centralized prototype registry to store a catalog of frequently used prototypes.
// You can implement the registry as a new factory class or put it in the base prototype class with 
// a static method for fetching the prototype. 
// This method should search for a prototype based on search criteria that the client code passes to the method. 
// The criteria might either be a simple string tag or a complex set of search parameters. 
// After the appropriate prototype is found, the registry should clone it and return the copy to the client.
// 
// In PrototypeFactory you have two concrete prototypes, one for each concrete prototype class, 
// so each time you want to create a bullet, you can use the existing ones and clone those.

class PrototypeFactory 
{
    private:
        std::unordered_map<Type, Prototype*, std::hash<int>> prototypes_;

    public:
        PrototypeFactory() 
        {
            prototypes_[Type::PROTOTYPE_1] = new ConcretePrototype1("PROTOTYPE_1 ", 50.f);
            prototypes_[Type::PROTOTYPE_2] = new ConcretePrototype2("PROTOTYPE_2 ", 60.f);
        }

        // Be carefull to free all memory allocated. 
        // Again, if you have smart pointers knowelege will be better to use it here.
        ~PrototypeFactory() {
            delete prototypes_[Type::PROTOTYPE_1];
            delete prototypes_[Type::PROTOTYPE_2];
        }

        // Just specify the type of the prototype you want and 
        // the method will create one from the existing object of this type.
        Prototype* CreatePrototype(Type type) 
        {
            return prototypes_[type]->Clone();
        }
};

void Client(PrototypeFactory& prototype_factory) 
{
    std::cout << "Let's create Prototype 1\n";

    Prototype* prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_1);
    prototype->Method(90);
    delete prototype;

    std::cout << "\n";

    std::cout << "Let's create Prototype 2\n";

    prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_2);
    prototype->Method(10);

    delete prototype;
}

int main() 
{
    PrototypeFactory* prototype_factory = new PrototypeFactory();
    Client(*prototype_factory);
    delete prototype_factory;

    return 0;
}