#include <iostream>

#include "FactoryManager.h"


class base
{

};

class dog: public base
{
    public:
        dog()
        {
        }

        dog(std::string type)
            :   m_type(type)
        {
        }

        void talk()
        {
            std::cout << m_type << " woof" << std::endl;
        }

    private:

        std::string m_type = "Unknown";
};

class cat: public base
{
    public:
        void talk()
        {
            std::cout << "Meow" << std::endl;
        }
};

int main()
{
    // Create factory
    Platform::ManagerFactory<base> factory;

    // Create classes with auto naming
    factory.create<dog, true>();
    factory.create<cat, true>();

    // Create another dog with a unique name of 2 and set the dog type within the class via the constructor
    factory.create<dog>("2", "German Shepard");

    // Speak
    factory.get<cat>().instance->talk();
    factory.get<dog>().instance->talk();
    factory.get<dog>("2").instance->talk();

    return 0;
}