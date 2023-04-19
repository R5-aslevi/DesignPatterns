// Bridge Design Pattern
//
// Bridge is a structural design pattern that lets you split a large class or a set of closely related classes 
// into two separate hierarchies, abstraction and implementation, which can be developed independently of each other.
// 
// 
// Bridge is usually designed up-front, letting you develop parts of an application independently of each other.
// 
// Use the pattern when you need to extend a class in several orthogonal (independent) dimensions.
// The Bridge suggests that you extract a separate class hierarchy for each of the dimensions.
// The original class delegates the related work to the objects belonging to those hierarchies instead 
// of doing everything on its own.

#include <string>
#include <iostream>

// The Implementation defines the interface for all implementation classes. 
// It does not have to match the Abstraction's interface. 
// In fact, the two interfaces can be entirely different. 
// Typically the Implementation interface provides only primitive operations, 
// while the Abstraction defines higher-level operations based on those primitives.

class Implementation 
{
    public:
        virtual ~Implementation() {}
        virtual std::string OperationImplementation() const = 0;
};

// Each Concrete Implementation corresponds to a specific platform and
// implements the Implementation interface using that platform's API.
class ConcreteImplementationA : public Implementation 
{
    public:
        std::string OperationImplementation() const override 
        {
            return "ConcreteImplementationA: Here's the result on the platform A.\n";
        }
};

class ConcreteImplementationB : public Implementation 
{
    public:
        std::string OperationImplementation() const override 
        {
            return "ConcreteImplementationB: Here's the result on the platform B.\n";
        }
};

// The Abstraction defines the interface for the "control" part of the two class hierarchies. 
// It maintains a reference to an object of the Implementation hierarchy and delegates all of 
// the real work to this object.
class Abstraction 
{
    protected:
        Implementation* implementation_;

    public:
        Abstraction(Implementation* implementation) : implementation_(implementation) { }

        virtual ~Abstraction() {}

        virtual std::string Operation() const 
        {
            return "Abstraction: Base operation with:\n" + this->implementation_->OperationImplementation();
        }
};

// You can extend the Abstraction without changing the Implementation classes.
class ExtendedAbstraction : public Abstraction 
{
    public:
        ExtendedAbstraction(Implementation* implementation) : Abstraction(implementation) { }

        std::string Operation() const override 
        {
            return "ExtendedAbstraction: Extended operation with:\n" + this->implementation_->OperationImplementation();
        }
};

// Except for the initialization phase, where an Abstraction object gets linked with 
// a specific Implementation object, the client code should only depend on the Abstraction class. 
// This way the client code can support any abstraction-implementation combination.
void ClientCode(const Abstraction& abstraction) 
{
    // ...
    std::cout << abstraction.Operation();
    // ...
}

// The client code should be able to work with any pre-configured abstraction-implementation combination.
int main() 
{
    Implementation* implementation_ptr = new ConcreteImplementationA;
    Abstraction* abstraction_ptr = new Abstraction(implementation_ptr);
    ClientCode(*abstraction_ptr);
    delete implementation_ptr;
    delete abstraction_ptr;

    std::cout << std::endl;

    implementation_ptr = new ConcreteImplementationB;
    abstraction_ptr = new ExtendedAbstraction(implementation_ptr);
    ClientCode(*abstraction_ptr);
    delete implementation_ptr;
    delete abstraction_ptr;

    return 0;
}