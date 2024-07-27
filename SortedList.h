#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {


    

    template <typename T>
    struct node {
        node<T>* prev;
        node<T>* next;
        T value;
    };
    

    template <typename T>
    class SortedList {

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

    public:
        node<T>* head;
        node<T>* tail;
        SortedList<T>() {
            head = NULL;
            tail = NULL;
        }

       class ConstIterator;
       
        void Insert(T t) {
            node<T>* newNode = new node<T>;
            newNode->next = NULL;
            newNode->prev = NULL;
            newNode->value = t;
            node<T>* currNode = head;
            node<T>* currPrev = NULL;
            while(currNode && currNode->value < t) {
                    currPrev = currNode;
                    currNode = currNode->next;
            }
            if(currPrev) {
                currPrev->next = newNode;
                newNode->prev = currPrev;
            }
            if(currNode) {
                newNode->next = currNode;
                currNode->prev = newNode;
            }
            if(newNode->prev == NULL) {
                head = newNode;
            }
            if(newNode->next == NULL) {
                tail = newNode;
            }
        }

        ConstIterator  begin() const{
            return ConstIterator(this,head);
        }
        ConstIterator end() const{
            return ConstIterator(this,NULL);
        }

        void Remove(ConstIterator& i) {
            if(i.currNode == NULL) {
                return;
            }

            const node<T>* curr = i.currNode;
            node<T>* prev = i.currNode->prev;
            node<T>* next = i.currNode->next;
            
            if(prev != NULL) {
                prev->next = next;
            }
            if(next != NULL) {
                next->prev = prev;
            }
            if(next != NULL && next->prev == NULL) {
                this->head = next;
            }
            if(next != NULL && next->next == NULL) {
                this->tail = next;
            }
            if(prev != NULL && prev->prev == NULL) {
                this->head = prev;
            }
            if(prev != NULL && prev->next == NULL) {
                this->tail = prev;
            }
            if(prev == NULL && next == NULL) {
                this->head = NULL;
                this->tail = NULL;
            }
            delete curr;
            i.currNode = NULL;
            return;
        }

        int Length() {
            int result = 0;
            for(ConstIterator it = this->begin(); it != this->end() ; ++it) {
                result++;
            }
            return result;
        }

        SortedList<T>(const SortedList& s) : head(NULL), tail(NULL) {
            for(ConstIterator it = s.begin(); it != s.end() ; ++it) {
                this->Insert(*it);
            }
        }

        SortedList<T>& operator=(const SortedList& s) {
            if(this == &s) {
                return *this;
            }
            ConstIterator it = this->begin();
            ConstIterator temp;
            while(it != this->end())
            {
                temp = it;
                ++it;
                this->Remove(temp);
            }
            it = s.begin();
            while(it != s.end())
            {
                this->Insert(it.currNode->value);
                ++it;
            }
            return *this;
        }

        ~SortedList() {
            ConstIterator it = this->begin();
            ConstIterator temp;
            while(it != this->end())
            {
                temp = it;
                ++it;
                this->Remove(temp);
            }
        }

        SortedList<T>& filter(bool (*func)(T)) {
            SortedList<T> *newList = new SortedList<T>;
            for(ConstIterator it = this->begin(); it != this->end() ; ++it) {
                if(func(*it)) {
                    newList->Insert(*it);
                }
            }
            return *newList;
        }

        SortedList<T>& apply(T (*func)(T)) {
            SortedList<T> *newList = new SortedList<T>;
            for(ConstIterator it = this->begin(); it != this->end() ; ++it) {
                newList->Insert(func(*it));
            }
            return *newList;
        }
    };

    template <class T>
    class SortedList<T>::ConstIterator {

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

    public:
        const SortedList<T>* list;
        const node<T>* currNode;
        ConstIterator() : list(NULL), currNode(NULL) {}
        ConstIterator(const SortedList<T>* list, const node<T>* currNode) : list(list), currNode(currNode) {}
        ~ConstIterator()=default;
        ConstIterator(const ConstIterator&)=default;
        ConstIterator& operator=(const ConstIterator&)=default;
        ConstIterator& operator++() {
            currNode = currNode->next;
            return *this;
        }
        bool operator!=(const ConstIterator& other) {
            if(this->currNode != NULL && other.currNode != NULL) {
                return (this->currNode->value != other.currNode->value);
            }
            if(this->currNode == NULL && other.currNode == NULL) {
                return false;
            }
            return true;

        }
        const T& operator*() const {
            return this->currNode->value;
        }

        friend SortedList<T>;
    
    };
}

