#pragma once

#include <iostream>
#include <stdexcept>

using std::cout, std::endl;

namespace mtm
{

    template <typename T>
    struct node {
        node<T> *prev;
        node<T> *next;
        T *value;
    };

    template <typename T>
    class SortedList {

    private:
        node<T> *head;
        node<T> *tail;

        void clear() {
            node<T> *currentNode = head;
            while (currentNode != nullptr) {
                node<T> *nextNode = currentNode->next;
                delete currentNode->value;
                delete currentNode;
                currentNode = nextNode;
            }
            head = tail = nullptr;
        }

    public:
        SortedList<T>() {
            head = nullptr;
            tail = nullptr;
        }

        class ConstIterator;

        SortedList<T> &insert(const T &valueToInsert) {
            node<T> *newNode = nullptr;
            T *newValue = nullptr;
            try {

                newValue = new T(valueToInsert);
                newNode = new node<T>;
                newNode->value = newValue;
                newNode->next = nullptr;
                newNode->prev = nullptr;

                node<T> *currentNode = head;
                node<T> *currentPrev = nullptr;

                while (currentNode && *(currentNode->value) > valueToInsert) {
                    currentPrev = currentNode;
                    currentNode = currentNode->next;
                }
                if (currentPrev) {
                    currentPrev->next = newNode;
                    newNode->prev = currentPrev;
                }
                
                if (currentNode) {
                    newNode->next = currentNode;
                    currentNode->prev = newNode;
                }

                if (newNode->prev == nullptr) {
                    head = newNode;
                }

                if (newNode->next == nullptr) {
                    tail = newNode;
                }
            }
            catch (...) {
                delete newValue;
                delete newNode;
                throw;
            }
            return *this;
        }

        ConstIterator begin() const {
            return ConstIterator(this, head);
        }

        ConstIterator end() const {
            return ConstIterator(this, nullptr);
        }

        SortedList<T> &remove(const ConstIterator &iteratorToRemoveFrom) {
            if (iteratorToRemoveFrom.currentNode == nullptr) {
                return *this;
            }

            const node<T> *nodeToRemove = iteratorToRemoveFrom.currentNode;
            node<T> *prev = iteratorToRemoveFrom.currentNode->prev;
            node<T> *next = iteratorToRemoveFrom.currentNode->next;

            if (prev != nullptr) {
                prev->next = next;
            }

            if (next != nullptr) {
                next->prev = prev;
            }
            
            if (next != nullptr && next->prev == nullptr) {
                this->head = next;
            }

            if (next != nullptr && next->next == nullptr) {
                this->tail = next;
            }

            if (prev != nullptr && prev->prev == nullptr) {
                this->head = prev;
            }

            if (prev != nullptr && prev->next == nullptr) {
                this->tail = prev;
            }

            if (prev == nullptr && next == nullptr) {
                this->head = nullptr;
                this->tail = nullptr;
            }

            delete nodeToRemove->value;
            delete nodeToRemove;
            return *this;
        }

        int length() const {
            int result = 0;
            for (SortedList::ConstIterator it = this->begin(); it != this->end(); ++it) {
                result++;
            }

            return result;
        }

        SortedList<T>(const SortedList &listToCopy) : head(nullptr), tail(nullptr) {
            try {
                for (ConstIterator it = listToCopy.begin(); it != listToCopy.end(); ++it) {
                    this->insert(*it);
                }
            }
            catch (...) {
                this->clear();
                throw;
            }
        }

        SortedList<T> &operator=(const SortedList &listToAssign) {
            if (this == &listToAssign) {
                return *this;
            }

            try {
                SortedList<T> tempList(listToAssign);
                this->clear();
                this->head = tempList.head;
                this->tail = tempList.tail;

                tempList.head = nullptr;
                tempList.tail = nullptr;
            }
            catch (...) {
                throw;
            }
            return *this;
        }

        ~SortedList() {
            this->clear();
        }

        template <class Condition>
        SortedList<T> filter(Condition filterCondition) const {
            SortedList<T> newList;

            for (const T &currentValue : *this) {
                if (filterCondition(currentValue)) {
                    newList.insert(currentValue);
                }
            }

            return newList;
        }

        template <class Operation>
        SortedList<T> apply(Operation operationToApply) const {
            SortedList<T> newList;

            for (const T &currentValue : *this) {
                newList.insert(operationToApply(currentValue));
            }

            return newList;
        }
    };

    template <class T>
    class SortedList<T>::ConstIterator {

    private:
        const SortedList<T> *list;
        const node<T> *currentNode;
        ConstIterator(const ConstIterator &) = default;

    public:
        ConstIterator() : list(nullptr), currentNode(nullptr) {}
        ConstIterator(const SortedList<T> *list, const node<T> *currentNode) : 
                    list(list), currentNode(currentNode) {}
        ~ConstIterator() = default;
        ConstIterator &operator=(const ConstIterator &) = default;

        ConstIterator &operator++() {
            if (currentNode == nullptr) {
                throw std::out_of_range("You cannot advance yourself further the end of the list");
            }
            currentNode = currentNode->next;
            return *this;
        }

        bool operator!=(const ConstIterator &other) {
            return (this->currentNode != other.currentNode);
        }

        const T &operator*() const {
            return *(this->currentNode->value);
        }

        friend SortedList<T>;
    };
}
