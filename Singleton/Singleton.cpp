// Singleton Design Pattern (thread-safe)
// 
// It is pretty easy to implement a sloppy Singleton. 
// You just need to hide the constructor and implement a static creation method.
// The same class behaves incorrectly in a multithreaded environment. 
// Multiple threads can call the creation method simultaneously and get several instances of a Singleton class.
// To fix the problem, you have to synchronize threads during the first creation of the Singleton object.

// https://refactoring.guru/design-patterns/singleton

#include <mutex>
#include <string>
#include <thread>
#include <iostream>

// The Singleton class defines the `GetInstance` method that serves as an alternative to 
// constructor and lets clients access the same instance of this class over and over.
class Singleton
{
         // The Singleton's constructor/destructor should always be private to
         // prevent direct construction/desctruction calls with the `new`/`delete` operator.
    private:
        static Singleton* m_instance_ptr;
        static std::mutex m_mutex;

    protected:

        Singleton(const std::string value) : m_value(value) {}
        ~Singleton() {}
        std::string m_value;

    public:

        // Singletons should not be cloneable.
        Singleton(Singleton& other) = delete;

        // Singletons should not be assignable.
        void operator=(const Singleton&) = delete;

        // This is the static method that controls the access to the singleton instance. 
        // On the first run, it creates a singleton object and places it into the static field. 
        // On subsequent runs, it returns the client existing object stored in the static field.
        static Singleton* GetInstance(const std::string& value);
    
        // Finally, any singleton should define some business logic, which can be executed on its instance.
        void SomeBusinessLogic()
        {
            // ...
        }

        std::string Value() const 
        {
            return m_value;
        }
};

// Static methods should be defined outside the class.
Singleton* Singleton::m_instance_ptr{ nullptr };
std::mutex Singleton::m_mutex;

// The first time we call GetInstance we will lock the storage location and then 
// we make sure again that the variable is null and then we set the value.
Singleton* Singleton::GetInstance(const std::string& value)
{
    // RAII, exeption safe
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_instance_ptr == nullptr)
    {
        m_instance_ptr = new Singleton(value);
    }

    return m_instance_ptr;
}

void ThreadFoo() 
{
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Singleton* singleton = Singleton::GetInstance("FOO\n");

    std::cout << singleton->Value() << "\n";
}

void ThreadBar() 
{
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Singleton* singleton = Singleton::GetInstance("BAR\n");

    std::cout << singleton->Value() << "\n";
}

int main()
{
    std::cout << "If you see the same value, then singleton was reused (yay!\n" <<
        "If you see different values, then 2 singletons were created (booo!!)\n\n" <<
        "RESULT:\n";

    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);

    t1.join();
    t2.join();

    return 0;
}