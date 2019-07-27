#pragma once

#include <utility>
#include <memory>
#include <map>
#include <stdexcept>
#include <string>

using namespace std;

namespace Platform
{
    /**
     * Container class that allows classes inherited by to create, get and destroy instances
     * of any class that shares a base parent.
     *
     * @code
     *
     *
     *
     * @endcode
     */
    template<class BASEMANAGER>
    class ManagerFactory
    {
    public:

        ManagerFactory()
        {

        }

        ~ManagerFactory()
        {

        }

        ManagerFactory(const ManagerFactory&) = delete;


        /**
         * Return type of get
         *
         * @tparam T
         */
        template<typename T>
        struct Return
        {
            // Status of cast
            bool status;

            // Location of object
            std::shared_ptr<T> instance = nullptr;

            // Overload -> so we can dive straight into object without checking status
            std::shared_ptr<T> operator->()
            {
                return instance;
            }
        };

        /**
         * Minimal version of c++ is the 11 standard
         * as such we'll implement reinterpret_pointer_cast locally
         */
        template <typename To, typename From>
        inline std::shared_ptr<To> reinterpret_pointer_cast(std::shared_ptr<From> const & ptr) noexcept
        {
            return std::shared_ptr<To>(ptr, reinterpret_cast<To *>(ptr.get()));
        }



        /**
         * Create a new instance
         */
        template<typename T, typename... Args>
        string create(string view, Args... args)
        {
            // Create instance
            m_managers[view] = make_shared<T>(args...);

            // Return the instance reference
            return view;
        }

        template<typename T, bool I, typename... Args>
        string create(Args... args)
        {
            if (I == true)
            {
                    // Create name from class
                    string view(typeid(T).name());

                // Return the instance referance
                return create<T>(view, args...);
            }

            return "";
        }


        /**
         * Destroy an instance
         */
        void destroy()
        {
            if(m_managers.empty())
                return;

            // Erase the entry
            for(auto& i : m_managers)
            {
                m_managers.erase(i.first);
            }
        }
        void destroy(string view)
        {
            // Check exists
            if(!exists(view))
                return;

            // Erase the entry
            m_managers.erase(view);
        }
        template<typename T>
        void destroy()
        {
            // Create name from class
            string name(typeid(T).name());
            destroy(name);
        }


        /**
         * Return a sensors object
         *
         * @param Type type supported sensor type
         * @param int i id of the sensor
         * @return T class
         */
        template<typename T>
        Return<T> get(string view)
        {
            // Check exists
            if(!exists(view))
            {
                Return<T> toReturn;
                toReturn.status = false;
                return toReturn;
            }

            // Create return
            Return<T> toReturn;
            toReturn.status = true;
            toReturn.instance = reinterpret_pointer_cast<T>(m_managers[view]);

            // Cast to correct object and return
            return toReturn;
        }

        template<typename T>
        Return<T> get()
        {
            string name(typeid(T).name());
            return get<T>(name);
        }

    private:
        // Container for all our sensors
        std::map<string, std::shared_ptr<BASEMANAGER>> m_managers;

        /**
         * Check ID exists
         */
        bool exists(string view)
        {
            if (m_managers.find(view) == m_managers.end())
            {
                return false;
            }

            return true;
        }

    };

}