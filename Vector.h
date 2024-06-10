#pragma once
#include<iostream>
using namespace std;


template <class T>
class Vector
{
protected:
    T* arr;
    int capacity;
    int _size;

public:

    Vector(); // Initialize your attributes
    // Vec(const int& s);
    Vector(const Vector<T>& v);
    Vector<T> copy(const Vector<T>& v);
    //void insertAtEnd(Vec<T>& v, T val);
    int size(); // return current size of vector
    virtual void push_back(T data); // insert data at the end of array
    virtual void pop_back(); // delete last element
    T* begin(); // returns pointer pointing to first element
    T* end();// returns pointer pointing to (size + 1)th element
    T& operator[](int index); //returns reference of element at index
    void print(); // print contents of the array
    ~Vector(); // deallocates memory 

};

template <class T>

Vector<T>::Vector()
{
    this->capacity = 10;
    this->_size = 0;
    this->arr = new T[this->capacity];
}


template <class T>

Vector<T>::Vector(const Vector<T>& v)
{
    this->capacity = v.capacity;
    this->_size = v._size;
    this->arr = new T[this->capacity];
    for (int i = 0; i < this->_size; i++)
    {
        this->arr[i] = v.arr[i];
    }
}

template <class T>

Vector<T> Vector<T>::copy(const Vector<T>& v)
{
    Vector<T> copy = v;
    return copy;
}

template <class T>

int Vector<T>::size()
{
    return this->_size;
}


template <class T>

void Vector<T>::push_back(T data)
{
    if (this->_size >= this->capacity)
    {
        T* a = new T[this->capacity * this->capacity];
        for (int i = 0; i < this->_size; i++)
        {
            a[i] = this->arr[i];
        }
        delete[] this->arr;
        this->arr = a;
        this->capacity *= this->capacity;
        this->_size += 1;
    }
    this->arr[this->_size] = data;
    this->_size += 1;
}


template <class T>

void Vector<T>::pop_back()
{
    if (this->_size == 0)
    {
        return;
    }
   // this->arr[this->_size - 1] = NULL;
    this->_size -= 1;
}

template <class T>

T* Vector<T>::begin()
{
    if (this->_size > 0)
        return &arr[0];
    return nullptr;
}

template <class T>

T* Vector<T>::end()
{
    return &arr[_size];
}

template <class T>

T& Vector<T>::operator[](int index)
{
    if (index < this->_size)
        return this->arr[index];

}

template <class T>

void Vector<T>::print()
{
    cout << "\n{ ";
    for (int i = 0; i < this->_size; i++)
    {
        cout << this->arr[i] << " ";
    }
    cout << "}\n";
}

template <class T>

Vector<T>::~Vector()
{
    // delete[]arr;
}