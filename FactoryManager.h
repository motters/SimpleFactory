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
      * Container class to create, get and destroy instances of objects
      * of any class that inherits BASEMANAGER.
      *
      * @tparam BASEMANAGER
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
         *      Note: can use -> instead of asking for the instance
         *
         * @tparam T type of class
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
         *
         * This function has the ability to cast object within shared_ptr
         *
         * Minimal version of c++ is the 11 standard
         * as such we'll implement reinterpret_pointer_cast locally
         *
         * @tparam To what you which to cast the object to
         * @tparam From what you are casting from
         * @param ptr object requiring cast
         * @return cast object
         */
        template <typename To, typename From>
        inline std::shared_ptr<To> reinterpret_pointer_cast(std::shared_ptr<From> const & ptr) noexcept
        {
            return std::shared_ptr<To>(ptr, reinterpret_cast<To *>(ptr.get()));
        }


        /**
         * Create a new instance of an object and store in container
         *
         * @tparam T Object being created
         * @tparam Args arguments to pass to object constructor
         * @param view name / id of the class for indexing
         * @param args Args arguments to pass to object constructor
         * @return string name of the index
         */
        template<typename T, typename... Args>
        string create(string view, Args... args)
        {
            // Create instance
            m_managers[view] = make_shared<T>(args...);

            // Return the instance reference
            return view;
        }


        /**
         * Create a new instance of an object and store in container
         *      Note the instance index is given an automatic name via its class type
         *
         * @tparam T Object being created
         * @tparam I Double check we want auto naming; only 1 instance of any class can be handled this way
         * @tparam Args Args arguments to pass to object constructor
         * @param args Args arguments to pass to object constructor
         * @return string name of the index
         */
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
         * Destroy all instances within container
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


        /**
         * Destroy an instance via the index
         *
         * @param view
         */
        void destroy(string view)
        {
            // Check exists
            if(!exists(view))
                return;

            // Erase the entry
            m_managers.erase(view);
        }


        /**
         * Destroy an instance via the auto naming indexing method
         *
         * @tparam T
         */
        template<typename T>
        void destroy()
        {
            // Create name from class
            string name(typeid(T).name());
            destroy(name);
        }


        /**
         * Return a instance of an object via the index
         *
         * @tparam T type of object
         * @param view index of object
         * @return Return<T> contains shared_ptr to instance and status data
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


        /**
         * Return a instance of an object via the auto naming method
         *
         * @tparam T type of object
         * @return Return<T> contains shared_ptr to instance and status data
         */
        template<typename T>
        Return<T> get()
        {
            string name(typeid(T).name());
            return get<T>(name);
        }

    private:
        // Container for all our instances
        std::map<string, std::shared_ptr<BASEMANAGER>> m_managers;

        /**
         * Check ID exists
         */
        /**
         * Checks if an index exists in the container
         *
         * @param view
         * @return
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