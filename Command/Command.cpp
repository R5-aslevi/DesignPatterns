// Command Design Pattern
//
// The command pattern is a behavioural design pattern in which an object is used to represent 
// and encapsulate all the information needed to call a method at a later time.
// This information includes the method name, the object that owns the method and values for the method parameters.

// Command is behavioral design pattern that converts requests or simple operations into objects.
// The conversion allows deferred or remote execution of commands, storing command history, etc.

// Identification: The Command pattern is recognizable by behavioral methods in an abstract/interface type (sender) 
// which invokes a method in an implementation of a different abstract/interface type (receiver) which has been 
// encapsulated by the command implementation during its creation. 
// Command classes are usually limited to specific actions.

// https://refactoring.guru/design-patterns/command

#include <iostream>
#include <string>

// The Command interface usually declares just a single method for executing the command.
class Command 
{
    public:
        virtual ~Command() { }
        virtual void Execute() const = 0;
};

// Concrete Commands implement various kinds of requests. 
// A concrete command is not supposed to perform the work on its own, but rather to pass the call to one of the business logic objects. 
// However, for the sake of simplifying the code, these classes can be merged.
// Parameters required to execute a method on a receiving object can be declared as fields in the concrete command.
// You can make command objects immutable by only allowing the initialization of these fields via the constructor.

// Some commands can implement simple operations on their own.
class SimpleCommand : public Command 
{
    private:
        std::string pay_load_;

    public:
        explicit SimpleCommand(std::string pay_load) : pay_load_(pay_load) { }

        void Execute() const override 
        {
            std::cout << "SimpleCommand: See, I can do simple things like printing (" << this->pay_load_ << ")\n";
        }
};

// The Receiver classes contain some important business logic. In fact, any class may serve as a Receiver.
// They know how to perform all kinds of operations, associated with carrying out a request. 
// Most commands only handle the details of how a request is passed to the receiver, while the receiver itself does the actual work.
class Receiver 
{
    public:
        void DoSomething(const std::string& a) 
        {
            std::cout << "Receiver: Working on (" << a << ".)\n";
        }

        void DoSomethingElse(const std::string& b) 
        {
            std::cout << "Receiver: Also working on (" << b << ".)\n";
        }
};

// However, some commands can delegate more complex operations to other objects, called "receivers."
class ComplexCommand : public Command 
{
    private:
        Receiver* receiver_;
    
        // Context data, required for launching the receiver's methods.
        std::string a_;
        std::string b_;
     
    public:

        // Complex commands can accept one or several receiver objects along with any context data via the constructor.
        ComplexCommand(Receiver* receiver, std::string a, std::string b) : receiver_(receiver), a_(a), b_(b) { }
        
        // Commands can delegate to any methods of a receiver.
        void Execute() const override 
        {
            std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
            this->receiver_->DoSomething(this->a_);
            this->receiver_->DoSomethingElse(this->b_);
        }
};

// The Invoker is associated with one or several commands. It sends a request to the command.

// The Invoker is responsible for initiating requests. 
// This class must have a field for storing a reference to a command object. 
// The Invoker triggers that command instead of sending the request directly to the receiver. 
// Note that the Invoker is not responsible for creating the command object. 
// Usually, it gets a pre-created command from the client via the constructor.
class Invoker 
{
    private:
        Command* on_start_;
        Command* on_finish_;

    public:
        ~Invoker() 
        {
            delete on_start_;
            delete on_finish_;
        }

        void SetOnStart(Command* command) 
        {
            this->on_start_ = command;
        }

        void SetOnFinish(Command* command) 
        {
            this->on_finish_ = command;
        }

        // The Invoker does not depend on concrete command or receiver classes. 
        // The Invoker passes a request to a receiver indirectly, by executing a command.
        void DoSomethingImportant() 
        {
            std::cout << "Invoker: Does anybody want something done before I begin?\n";

            if (this->on_start_) 
            {
                this->on_start_->Execute();
            }

            std::cout << "Invoker: ...doing something really important...\n";
            std::cout << "Invoker: Does anybody want something done after I finish?\n";

            if (this->on_finish_) 
            {
                this->on_finish_->Execute();
            }
        }
};

// The client code can parameterize an invoker with any commands.
// The client creates and configures concrete command objects. 
// The client must pass all of the request parameters, including a receiver instance, into the command’s constructor. 
// After that, the resulting command may be associated with one or multiple senders.
int main() 
{
    Invoker* invoker = new Invoker;
    invoker->SetOnStart(new SimpleCommand("Say Hi!"));

    Receiver* receiver = new Receiver;
    invoker->SetOnFinish(new ComplexCommand(receiver, "Send email", "Save report"));
    invoker->DoSomethingImportant();

    delete invoker;
    delete receiver;

    return 0;
}