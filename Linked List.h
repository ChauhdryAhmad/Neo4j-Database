#pragma once
#include<iostream>
using namespace std;

template <class T>
struct ListNode
{
    T data;
    ListNode<T>* next; //This points to nullptr for tail

    ListNode()//initialize next with nullptr
    {
        this->next = NULL;
    }

    ListNode(T _data)//initialize data with _data and next with nullptr
    {
        this->data = _data;
        this->next = NULL;
    }

    ListNode(ListNode<T>* _next)//initialize next with _next
    {
        this->next = _next;
    }

    ListNode(T _data, ListNode<T>* _next)//initialize data with _data and next with _next
    {
        this->data = _data;
        this->next = _next;
    }

    ListNode<T>*& operator=(const ListNode<T>*& _N)
    {
        if (this != &_N)
        {
            this->data = _N->data;
            this->next = _N->next;
        }
        return *this;
    }

    bool operator==(const ListNode<T>*& _N)
    {
        if ((this->data == _N->data) && (this->next == _N->next))
            return true;
        return false;
    }

};


template <class T>
class LinkedList
{
    ListNode<T>* head;
    ListNode<T>* tail;

public:

    LinkedList()//initialize both pointers with nullptr
    {
        this->head = nullptr;
        this->tail = nullptr;
    }

    LinkedList(int _size) //create linked list of given size
    {
        for (int i = 0; i < _size; i++)
        {
            this->insert(0);
        }
    }

    LinkedList(const LinkedList& L) //create a deep copy
    {
        ListNode<T>* cur = L.head;
        while (cur != nullptr)
        {
            if (this->head == nullptr)
            {
                this->head = cur;
                this->tail = cur;
            }
            else
            {
                this->tail = cur;
            }
            cur = cur->next;
        }
    }

    ListNode<T>* begin() //return head
    {
        return this->head;
    }

    ListNode<T>* end() // return tail
    {
        return this->tail;
    }

    void insert(T _data) //add node with data at the end of list
    {
        ListNode<T>* nn = new ListNode<T>(_data);
        // nn->data = _data;
        nn->next = nullptr;
        if (head == nullptr)
        {
            this->head = nn;
            this->tail = nn;
        }
        else
        {
            this->tail->next = nn;
            this->tail = nn;
        }
    }

    void insert(T _data, int pos) //add node with data at pos-th index
    {
        try
        {
            if (pos < 0)
                throw"i is out of bounds";
            ListNode<T>* nn = new ListNode<T>(_data);
            nn->next = nullptr;
            if (pos == 0)
            {
                nn->next = this->head;
                this->head = nn;
            }
            else
            {
                ListNode<T>* cur = this->head;
                for (int i = 1; i < pos; i++)
                {
                    cur = cur->next;
                    if (cur->next == nullptr && pos > (i + 1))
                    {
                        throw"i is out of bounds";
                    }
                }
                if (cur->next != nullptr)
                {
                    nn->next = cur->next;
                    cur->next = nn;
                }
                else
                {
                    this->tail->next = nn;
                    this->tail = nn;
                }
            }
        }
        catch (const char* ch)
        {
            cout << "\n" << ch;
        }
    }

    ListNode<T>* find(T _data) //return node with matching data
    {
        ListNode<T>* cur = this->head;
        while (cur != nullptr)
        {
            if (cur->data == _data)
            {
                return cur;
            }
            cur = cur->next;
        }
        return 0;
    }

    void erase(ListNode<T>* it) //remove the matching node from list
    {
        if (it == this->head)
        {
            this->head = this->head->next;
        }
        else
        {
            ListNode<T>* pn = this->head;
            ListNode<T>* cn = this->head->next;
            while (cn != nullptr)
            {
                if (it == cn)
                {
                    if (cn->next == nullptr)
                        tail = pn;
                    pn->next = cn->next;
                    delete cn;
                    return;
                }
                pn = pn->next;
                cn = cn->next;
            }
        }
    }

    T& operator[](int i) //return the data at ith node
    {
        try
        {
            if (i < 0)
                throw "Illegal Memory access";
            ListNode<T>* cur = this->head;
            int j = 0;
            while (j <= i && cur != nullptr)
            {
                if (j == i)
                {
                    return cur->data;
                }
                j++;
                cur = cur->next;
            }
            throw "Illegal Memory access";
        }
        catch (const char* ch)
        {
            cout << "\n" << ch;
        }
    }

    LinkedList& operator=(const LinkedList& L)//create a deep copy
    {
        if (this != &L)
        {
            this->head = L.head;
            this->tail = L.tail;
        }
        return *this;
    }

    friend ostream& operator<<(ostream& _cout, const LinkedList& L)//print the linked list
    {
        ListNode<T>* cur = L.head;
        _cout << "\n{ ";
        while (cur != nullptr)
        {
            _cout << cur->data;
            if (cur->next != nullptr)
                _cout << " -> ";
            cur = cur->next;
        }
        _cout << "}\n";
        return _cout;
    }

    void reverse() //reverse the linked list in a single iteration- O(N)
    {
        ListNode<T>* pre = nullptr;
        ListNode<T>* cur = this->head;
        ListNode<T>* nxt = this->head->next;
        while (nxt != nullptr)
        {
            cur->next = pre;
            pre = cur;
            cur = nxt;
            nxt = cur->next;
        }
        cur->next = pre;
        pre = cur;
        this->tail = this->head;
        this->head = pre;
    }

    int size()
    {
        int sz = 0;
        ListNode<T>* nn = head;
        if (nn == nullptr)
            return sz;
        while (nn != nullptr)
        {
            nn = nn->next;
            sz++;
        }
        return sz;
    }

    ListNode<T>* getMid() //return the middle node of the linked list getMid() is not allowed to count/use size of list!
    {
        ListNode<T>* tort = head;
        ListNode<T>* hare = head;
        while (hare->next != nullptr)
        {
            hare = hare->next->next;
            tort = tort->next;
        }
        return tort;
    }

    ~LinkedList() //deallocate all nodes
    {

        while (head != nullptr)
        {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        delete head;
        //delete tail;
    }
};

