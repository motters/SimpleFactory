# Simple Factory Manager

The project demonstrates a simple factory manager that is
lightweight enough to use of microcontrollers.

## Supports

The factory manager supports the following features:
  * Creation of objects with constructor arguments
  * Retrieval of created objects includes a status param and
    shared_ptr with instance
  * Deletion of all or a selected object
  * Custom base class that all object should inherit off
  * Auto naming of index when creating and retrieving instances

## Example usage

See main.cpp for an example usage case.
  
## Requirements

  * C++11 of greater
  * Enabled RTTI for auto index naming support
  