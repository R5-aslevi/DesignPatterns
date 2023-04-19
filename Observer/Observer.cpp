// Observer Design Pattern
// 
// Observer is a behavioral design pattern that allows some objects to notify other objects 
// about changes in their state. The Observer pattern provides a way to subscribe and 
// unsubscribe to and from these events for any object that implements a subscriber interface.
// Th epattern defines a subscription mechanism to notify multiple objects
// about any events that happen to the object they are observing.

// Note that there is a lot of different terms with similar meaning associated with this pattern. 
// Just remember that the Publisher is also called the Publisher and the Observer is often called the 
// Subscriber and vice versa. Also the verbs "observe", "listen" or "track" usually mean the same thing.

// Use the Observer pattern when changes to the state of one object may require changing 
// other objects, and the actual set of objects is unknown beforehand or changes dynamically.

// Use the pattern when some objects in your app must observe others, but only for a limited 
// time or in specific cases.

// https://refactoring.guru/design-patterns/observer


#include <iostream>
#include <list>
#include <string>

class IObserver 
{
    public:

        virtual ~IObserver() {};
        virtual void Update(const std::string& message_from_publisher) = 0;
};

class IPublisher 
{
    public:

        virtual ~IPublisher() {};
        virtual void Attach(IObserver* observer) = 0;
        virtual void Detach(IObserver* observer) = 0;
        virtual void Notify() = 0;
};

// The Publisher owns some important state and notifies observers when the state changes.
class Publisher : public IPublisher 
{
    public:

        Publisher()
        {
            std::cout << "Publisher: Hi.\n";
        }

        virtual ~Publisher() 
        {
            std::cout << "Publisher: Goodbye.\n";
        }

        // The subscription management methods.
        void Attach(IObserver* observer) override 
        {
            list_observer_.push_back(observer);
        }

        void Detach(IObserver* observer) override 
        {
            list_observer_.remove(observer);
        }

        void Notify() override 
        {
            std::list<IObserver*>::iterator iterator = list_observer_.begin();

            HowManyObservers();

            while (iterator != list_observer_.end()) 
            {
                (*iterator)->Update(message_);
                ++iterator;
            }
        }

        void CreateMessage(std::string message = "Empty") 
        {
            this->message_ = message;
            Notify();
        }

        void HowManyObservers() 
        {
            std::cout << "There are " << list_observer_.size() << " observers in the list.\n";
        }

        // Usually, the subscription logic is only a fraction of what a Publisher can really do. 
        // Publishers commonly hold some important business logic, that triggers a notification 
        // method whenever something important is about to happen (or after it).
        void SomeBusinessLogic() 
        {
            this->message_ = "change message message";
            Notify();
            std::cout << "I'm about to do something important\n";
        }

    private:
        std::list<IObserver*> list_observer_;
        std::string message_;
};

class Observer : public IObserver 
{
    public:

        Observer(Publisher& publisher) : publisher_(publisher) 
        {
            this->publisher_.Attach(this);
            std::cout << "Observer \"" << ++Observer::num_of_observers_ << "\" has been added to the list.\n";
            this->number_ = Observer::num_of_observers_;
        }

        virtual ~Observer() 
        {
            std::cout << "Observer \"" << this->number_ << "\": Goodbye.\n";
        }

        void Update(const std::string& message_from_publisher) override 
        {
            message_from_publisher_ = message_from_publisher;
            PrintInfo();
        }

        void RemoveMeFromTheList() 
        {
            publisher_.Detach(this);
            std::cout << "Observer \"" << number_ << "\" has been removed from the list.\n";
        }

        void PrintInfo() 
        {
            std::cout << "Observer \"" << this->number_ << "\": a new message is available --> " << this->message_from_publisher_ << "\n";
        }

    private:

        Publisher& publisher_;
        std::string message_from_publisher_;   
        static int num_of_observers_;
        int number_;
};

int Observer::num_of_observers_ = 0;

void ClientCode() 
{
    Publisher* publisher = new Publisher;
    Observer* observer1 = new Observer(*publisher);
    Observer* observer2 = new Observer(*publisher);
    Observer* observer3 = new Observer(*publisher);
    Observer* observer4;
    Observer* observer5;

    publisher->CreateMessage("Hello World! :D");
    observer3->RemoveMeFromTheList();

    publisher->CreateMessage("The weather is hot today! :p");

    observer4 = new Observer(*publisher);
    observer2->RemoveMeFromTheList();
    observer5 = new Observer(*publisher);

    publisher->CreateMessage("My new car is great! ;)");

    observer5->RemoveMeFromTheList();
    observer4->RemoveMeFromTheList();
    observer1->RemoveMeFromTheList();

    delete observer5;
    delete observer4;
    delete observer3;
    delete observer2;
    delete observer1;
    delete publisher;
}

int main() 
{
    ClientCode();

    return 0;
}