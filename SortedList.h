#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm
{

    template <typename T>
    struct node
    {
        node<T> *prev;
        node<T> *next;
        T *value;
    };

    template <typename T>
    class SortedList
    {

        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method
         *
         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */

    private:
        node<T> *head;
        node<T> *tail;

        // SHELLY - Add list clear method
        void clear()
        {
            node<T> *currentNode = head;
            while (currentNode != nullptr)
            {
                node<T> *nextNode = currentNode->next;
                delete currentNode->value;
                delete currentNode;
                currentNode = nextNode;
            }
            head = tail = nullptr;
        }

    public:
        SortedList<T>()
        {
            head = nullptr;
            tail = nullptr;
        }

        class ConstIterator;

        // SHELLY - Apply this supports pointers
        SortedList<T>& insert(T& valueToInsert)
        {
            node<T> *newNode = new node<T>;
            newNode->next = nullptr;
            newNode->prev = nullptr;
            newNode->value = new T(valueToInsert);

            node<T> *currentNode = head;
            node<T> *currentPrev = nullptr;

            // TODO - SHELLY - Check this makes sense
            while (currentNode && currentNode->value > valueToInsert)
            {
                currentPrev = currentNode;
                currentNode = currentNode->next;
            }
            if (currentPrev)
            {
                currentPrev->next = newNode;
                newNode->prev = currentPrev;
            }
            if (currentNode)
            {
                newNode->next = currentNode;
                currentNode->prev = newNode;
            }
            if (newNode->prev == nullptr)
            {
                head = newNode;
            }
            if (newNode->next == nullptr)
            {
                tail = newNode;
            }

            return *this;
        }

        ConstIterator begin() const
        {
            return ConstIterator(this, head);
        }

        ConstIterator end() const
        {
            return ConstIterator(this, nullptr);
        }

        SortedList<T>& remove(ConstIterator& iteratorToRemoveFrom)
        {
            if (iteratorToRemoveFrom.currentNode == nullptr)
            {
                throw std::invalid_argument();
            }

            const node<T> *nodeToRemove = iteratorToRemoveFrom.currentNode;
            node<T> *prev = iteratorToRemoveFrom.currentNode->prev;
            node<T> *next = iteratorToRemoveFrom.currentNode->next;

            if (prev != nullptr)
            {
                prev->next = next;
            }
            if (next != nullptr)
            {
                next->prev = prev;
            }
            if (next != nullptr && next->prev == nullptr)
            {
                this->head = next;
            }
            if (next != nullptr && next->next == nullptr)
            {
                this->tail = next;
            }
            if (prev != nullptr && prev->prev == nullptr)
            {
                this->head = prev;
            }
            if (prev != nullptr && prev->next == nullptr)
            {
                this->tail = prev;
            }
            if (prev == nullptr && next == nullptr)
            {
                this->head = nullptr;
                this->tail = nullptr;
            }
            delete nodeToRemove;
            iteratorToRemoveFrom.currentNode = nullptr;
            return *this;
        }

        int length()
        {
            int result = 0;
            // SHELLY - Use cleaner iterartion
            for (T& currentValue : *this)
            {
                result++;
            }

            return result;
        }

        SortedList<T>(const SortedList &listToCopy) : head(nullptr), tail(nullptr)
        {
            for (T &currentNode : *listToCopy)
            {
                this->insert(currentNode);
            }
        }

        //SHELLY - Rewrite
        SortedList<T>& operator=(const SortedList &listToAssign)
        {
            if (this == &listToAssign)
            {
                return *this;
            }

            try
            {
                SortedList<T> newListToAssign = new SortedList<T>(listToAssign);
                clear();
                this = newListToAssign;
            }
            catch (...)
            {
                throw std::bad_alloc();
            }
            return *this;
        }

        // SHELLY - Rewrite
        ~SortedList()
        {
            clear();
        }

        SortedList<T> &filter(bool (*filterFunction)(T))
        {
            SortedList<T> *newList = new SortedList<T>;

            // SHELLY - Change to cleaner iteration
            for (T &currentValue : *this)
            {
                if (filterFunction(currentValue))
                {
                    newList->insert(&currentValue);
                }
            }

            return *newList;
        }

        SortedList<T> &apply(T (*applyFunction)(T))
        {
            SortedList<T> *newList = new SortedList<T>;

            // SHELLY - Change to cleaner iteration
            for (T &currentValue : *this)
            {
                newList->insert(applyFunction(currentValue));
            }

            return *newList;
        }
    };

    template <class T>
    class SortedList<T>::ConstIterator
    {

        /**
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. a ctor(or ctors) your implementation needs
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~ConstIterator() - destructor
         *
         * operators:
         * 5. operator* - returns the element the iterator points to
         * 6. operator++ - advances the iterator to the next element
         * 7. operator!= - returns true if the iterator points to a different element
         *
         */

    private:
        const SortedList<T> *list;
        const node<T> *currentNode;

        ConstIterator() : list(nullptr), currentNode(nullptr) {}
        ConstIterator(const SortedList<T> *list, const node<T> *currentNode) : list(list), currentNode(currentNode) {}
        ~ConstIterator() = default;
        ConstIterator(const ConstIterator &) = default;

    public:
        ConstIterator &operator=(const ConstIterator &) = default;

        ConstIterator &operator++()
        {
	        if(currentNode->next == nullptr)
            {
                throw std::out_of_range();
            }
            currentNode = currentNode->next;
            return *this;
        }

        // SHELLY - Fix this to check of the iterator points to the same cell and not if it contains the same value.
        bool operator!=(const ConstIterator &other)
        {
            return (this->currentNode != other.currentNode);
        }

        const T &operator*() const
        {
            return this->currentNode->value;
        }

        friend SortedList<T>;
    };
}