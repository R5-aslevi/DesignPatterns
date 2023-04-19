// Decorator Design Pattern
// 
// Decorator is a structural design pattern that allows adding new behaviors to objects by placing 
// these objects inside special wrapper objects (called decorators) that contain the behaviors.
// Decorator can be recognized by creation methods or constructors that 
// accept objects of the same class or interface as a current class.
// Use the Decorator pattern when you need to be able to assign extra behaviors 
// to objects at runtime without breaking the code that uses these objects.
// The Decorator lets you structure your business logic into layers, create a decorator 
// for each layer and compose objects with various combinations of this logic at runtime. 
// The client code can treat all these objects in the same way, since they all follow a common interface.
// Use the pattern when it is awkward or not possible to extend an object’s behavior using inheritance.

// https://refactoring.guru/design-patterns/decorator

#include <string>
#include <iostream>

// The base Component interface defines operations that can be altered by decorators.
class Component 
{
    public:
        virtual ~Component() {}
        virtual std::string Operation() const = 0;
};

// Concrete Components provide default implementations of the operations. 
// There might be several variations of these classes.
class ConcreteComponent : public Component 
{
    public:
        std::string Operation() const override 
        {
            return "ConcreteComponent";
        }
};

// The base Decorator class follows the same interface as the other components.
// The primary purpose of this class is to define the wrapping interface for all concrete decorators. 
// The default implementation of the wrapping code might include a field for storing a wrapped component 
// and the means to initialize it.
class Decorator : public Component 
{
    protected:
        Component* component_;

    public:
        Decorator(Component* component) : component_(component) { }

        // The Decorator delegates all work to the wrapped component.
        std::string Operation() const override 
        {
            return this->component_->Operation();
        }
};

// Concrete Decorators call the wrapped object and alter its result in some way.
class ConcreteDecoratorA : public Decorator 
{      
    public:
        ConcreteDecoratorA(Component* component) : Decorator(component) {}

        std::string Operation() const override 
        {
            // Decorators may call parent implementation of the operation, instead of calling the wrapped object directly.
            // This approach simplifies extension of decorator classes.
            return "ConcreteDecoratorA(" + Decorator::Operation() + ")";
        }
};

class ConcreteDecoratorB : public Decorator 
{
    public:
        ConcreteDecoratorB(Component* component) : Decorator(component) {}

        // Decorators can execute their behavior either before or after the call to a wrapped object.
        std::string Operation() const override 
        {
            return "ConcreteDecoratorB(" + Decorator::Operation() + ")";
        }
};

class ConcreteDecoratorC : public Decorator
{
public:
    ConcreteDecoratorC(Component* component) : Decorator(component) {}

    // Decorators can execute their behavior either before or after the call to a wrapped object.
    std::string Operation() const override
    {
        return "ConcreteDecoratorC(" + Decorator::Operation() + ")";
    }
};

// The client code works with all objects using the Component interface. 
// This way it can stay independent of the concrete classes of components it works with.
void ClientCode(Component* component) 
{
    // ...
    std::cout << "RESULT: " << component->Operation();
    // ...
}

int main() 
{
    // This way the client code can support both simple components...
    Component* simple = new ConcreteComponent;
    std::cout << "Client: I've got a simple component:\n";
    ClientCode(simple);
    std::cout << "\n\n";

    //
    // ...as well as decorated ones.
    //
    // Note how decorators can wrap not only simple components, but the other decorators as well

    Component* decorator1 = new ConcreteDecoratorA(simple);
    Component* decorator2 = new ConcreteDecoratorB(decorator1);
    Component* decorator3 = new ConcreteDecoratorC(decorator2);

    std::cout << "Client: Now I've got a decorated component:\n";

    ClientCode(decorator3);
    std::cout << "\n";

    delete simple;
    delete decorator1;
    delete decorator2;
    delete decorator3;

    return 0;
}