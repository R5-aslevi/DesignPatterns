// Strategy Design Pattern
//
// Strategy is a behavioral design pattern that turns a set of behaviors into objects
// and makes them interchangeable inside original context object.
// The original object, called context, holds a reference to a strategy object.
// The context delegates executing the behavior to the linked strategy object.
// In order to change the way the context performs its work, other objects may replace 
// the currently linked strategy object with another one.

// The Strategy pattern suggests that you take a class that does something specific in a lot of 
// different ways and extract all of these algorithms into separate classes called strategies.

// The original class, called context, has a field for storing a reference to one of the strategies.
// The context delegates the work to a linked strategy object instead of executing it on its own.

// The context is not responsible for selecting an appropriate algorithm for the job. In fact, the context 
// does not know much about strategies. Instead, it is the client that passes the desired strategy to the context. 
// It works with all strategies through the same generic interface,  which only exposes 
// a single method for triggering the algorithm encapsulated within the selected strategy.

// This way the context becomes independent of concrete strategies, so you can add new algorithms 
// or modify existing ones without changing the code of the context or other strategies.

// https://refactoring.guru/design-patterns/strategy

#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

// Provides read-only access to an existing string (a C-style string literal, a std::string, or a char array) 
// without making a copy.
#include <string_view>

// The Strategy interface declares operations common to all supported versions of some algorithm.
// The Context uses this interface to call the algorithm defined by Concrete Strategies.
class Strategy
{
    public:
        virtual ~Strategy() = default;
        virtual std::string doAlgorithm(std::string_view data) const = 0;
};

// The Context defines the interface of interest to clients.
class Context
{
    private:

        // The Context maintains a reference to one of the concrete Strategy 
        // objects and communicates with it only via the strategy interface.
        // The Context does not know the concrete class of a Strategy object.
        // It should work with all Strategy objects via the Strategy interface.
        std::unique_ptr<Strategy> strategy_;
    
    public:

        // The Context accepts a strategy through the constructor and also provides a setter to change it at runtime.
        explicit Context(std::unique_ptr<Strategy>&& strategy = {}) : strategy_(std::move(strategy)) { }

        // Usually, the Context allows replacing a Strategy object at runtime.
        void set_strategy(std::unique_ptr<Strategy>&& strategy)
        {
            strategy_ = std::move(strategy);
        }

        // The Context delegates some work to the Strategy object instead of
        // implementing multiple versions of the algorithm on its own.
        void doSomeBusinessLogic(std::string text_to_sort) const
        {
            // The context calls the execution method on the linked strategy object each time it needs to run the algorithm. 
            // The context does not know what type of strategy it works with or how the algorithm is executed.
            if (strategy_) 
            {
                std::cout << "Context: Sorting data using the strategy (not sure how it will do it)\n";
                std::string result = strategy_->doAlgorithm(text_to_sort);
                std::cout << result << "\n";
            }
            else 
            {
                std::cout << "Context: Strategy isn't set\n";
            }
        }
};

// Concrete Strategies implement different variations of an algorithm 
// the context uses, while following the base Strategy interface. 
// The interface makes them interchangeable in the Context.
class ConcreteStrategyA : public Strategy
{
    public:
        std::string doAlgorithm(std::string_view data) const override
        {
            std::string result(data);
            std::sort(std::begin(result), std::end(result), std::less<>());

            return result;
        }
};

class ConcreteStrategyB : public Strategy
{
    std::string doAlgorithm(std::string_view data) const override
    {
        std::string result(data);
        std::sort(std::begin(result), std::end(result), std::greater<>());

        return result;
    }
};

// The client code picks a concrete strategy and passes it to the context. 
// The client should be aware of the differences between strategies in order to make the right choice.
void clientCode()
{
    // std::string text_to_sort("haegicbjdf");
    std::string text_to_sort{"haegicbjdf"};
    //std::string_view text_to_sort{"haegicbjdf"};

    // The Client creates a specific strategy object and passes it to the context. 
    Context context(std::make_unique<ConcreteStrategyA>());

    std::cout << "Client: Strategy is set to sort in ascending order.\n";
    context.doSomeBusinessLogic(text_to_sort);

    std::cout << "\n";

    // The context exposes a setter which lets the Client replace 
    // the strategy object associated with the context at runtime.
    context.set_strategy(std::make_unique<ConcreteStrategyB>());

    std::cout << "Client: Strategy is set to sort in descending order.\n";
    context.doSomeBusinessLogic(text_to_sort);
}

int main()
{
    clientCode();
    return 0;
}