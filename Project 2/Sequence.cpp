//
//  Sequence.cpp
//  Project 2
//
//  Created by Jahan Cherian on 4/15/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include "Sequence.h"
#include <iostream>

Sequence::Sequence()        // Construct an empty Sequence through Circularly Linked Lists
: m_size(0)
{
    head = new Node;
    head->m_next = head;
    head->m_previous = head;
}

Sequence::~Sequence()       // Destructor for Sequence, works by destroying all nodes other than the sentinel.
{
    while (head->m_previous != head)
    {
        Node* deleter = head->m_previous;
        deleter->m_previous->m_next= head;
        head->m_previous = deleter->m_previous;
        delete deleter;
    }
    delete head;
}

Sequence& Sequence::operator=(const Sequence& rhs)  // Assignment operator that works on the copy swap method.
{
    if (this != &rhs)
    {
        Sequence temp(rhs);
        swap(temp);
    }
    return *this;
}

Sequence::Sequence(const Sequence& other)   // Copy constructor
: m_size(other.m_size)
{
    head = new Node;        // Initializes new head and sentinel for the Sequence.
    head->m_next = head;
    head->m_previous = head;
    
    for (Node* p = other.head->m_next; p != other.head; p = p->m_next) // Loop through the current Sequence.
    {
        Node* adder = new Node;
        adder->m_value = p->m_value;
        adder->m_next = head;
        adder->m_previous = head->m_previous;
        head->m_previous->m_next = adder;
        head->m_previous = adder;
    }
    
}

bool Sequence::empty() const    // Returns true if the size of Sequence is zero, otherwise false.
{
    if (m_size == 0)
        return true;
    return false;
}

int Sequence::size() const      // Returns the size of the Sequence.
{
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)       // Inserts value at specified pos in the Sequence and moves everything up by one.
{
    if (pos < 0 || pos > size())        // If pos is negative or above current size, i.e an invalid position, return false.
        return false;
    if (pos >= 0 && pos < size())
    {
        Node* p = head->m_next;
        for (int i = 0; i < size(); i++)    // Loops through such that the p pointer and i are aligned so as to mimic indexing.
        {
            if (i == pos)
            {
                Node* new_guy = new Node;
                new_guy->m_value = value;
                new_guy->m_next = p;       
                new_guy->m_previous = p->m_previous;    
                p->m_previous->m_next = new_guy;   
                p->m_previous = new_guy;
                m_size++;
                return true;
            }
            p = p->m_next;  // Moves pointer p forward in unison with iterator i, so as to maintain the indexing.
        }
    }
    else if (pos == size()) // If pos is defined as size, then insert the new node at the end.
    {
        Node* p = head->m_previous;
        Node* new_guy = new Node;
        new_guy->m_value = value;
        new_guy->m_next = head;
        new_guy->m_previous = p;
        p->m_next = new_guy;        
        head->m_previous = new_guy;
        m_size++;
        return true;
    }
    return true;
}

bool Sequence::insert(const ItemType& value)    // Inserts value in an ascending order.
{
    if (empty())        // If the sequence is empty, then add node to the front.
    {
        Node* new_guy = new Node;
        new_guy->m_value = value;
        new_guy->m_next = head;
        new_guy->m_previous = head;
        head->m_next = new_guy;
        head->m_previous = new_guy;
        m_size++;
        return true;
    }
    int pos = 0;
    Node* p = head;         // Otherwise, determine the position the value should take.
    for (int i = 0; i < size(); i++)
    {
        p = p->m_next;
        if (p->m_value >= value)
        {
            pos = i;
            break;
        }
        else
            pos = size();
    }
    insert(pos, value);     // Insert the value once the position has been determined.
    return true;
}

bool Sequence::erase(int pos)       // Return true if valid position is given, and erase the item at said position.
{
    if (pos >= 0 || pos < size())
    {
        Node* p = head;
        for (int i = 0; i < size(); i++)
        {
            p = p->m_next;
            if (i == pos)
            {
                Node* deleter = p;      // Does so by creating a temporary pointer pointing to pos, reconnecting surrounding nodes, and deleting node at pos.
                p->m_previous->m_next = deleter->m_next;
                p->m_next->m_previous = deleter->m_previous;
                delete deleter;
                m_size--;
                return true;
            }
        }
    }
    return false;
}


int Sequence::remove(const ItemType& value)     // Returns the number of items removed that are all equal to value. If no value, then return 0.
{
    int removed = 0;
    Node* p;
    for (p = head->m_next; p != head; p = p->m_next)
    {
        if (p->m_value == value)
        {
            Node* deleter = p;  // Does so by creating a temporary pointer pointing to the value, reconnecting surrounding nodes, and deleting node with value.
            p->m_previous->m_next = deleter->m_next;
            p->m_next->m_previous = deleter->m_previous;
            delete deleter;
            removed++;
            m_size--;
        }
    }
    return removed;
}

bool Sequence::get(int pos, ItemType& value) const  // Returns true and gets the value at the position and sets value = m_value, if position is valid.
{
    if (pos >= 0 || pos < size())
    {
        Node* p = head;
        for (int i = 0; i < size(); i++)
        {
            p = p->m_next;      // Point p to the item after the sentinel when i = 0 so as to mimic indexing.
            if (i == pos)
            {
                value = p->m_value;
                return true;
            }
        }
    }
    return false;
}

bool Sequence::set(int pos, const ItemType& value)  // Returns true and sets m_value at pos to value, if position is valid.
{
    if (pos >= 0 || pos < size())
    {
        Node* p = head;
        for (int i = 0; i < size(); i++)
        {
            p = p->m_next;  // Point p to the item after the sentinel when i = 0 so as to mimic indexing.
            if (i == pos)
            {
                p->m_value = value;
                return true;
            }
        }
    }
    return false;
}

int Sequence::find(const ItemType& value) const // If value exists in Sequence, return its position, else return -1.
{
    if (size() > 0)
    {
        Node* p = head;
        for (int i = 0; i < size(); i++)
        {
            p = p->m_next;      // Point p to the item after the sentinel when i = 0 so as to mimic indexing.
            if (p->m_value == value)
                return i;
        }
    }
    return -1;
}

void Sequence::swap(Sequence& other)    // Swap two sequences by exchanging their sizes and head pointers.
{
    Node* temp_head = other.head;
    other.head = head;
    head = temp_head;
    
    int temp_size = other.m_size;
    other.m_size = m_size;
    m_size = temp_size;
}

void Sequence::dump() const     // Prints out all the values in the Sequence to cerr for teting purposes.
{
    for (Node* p = head->m_next; p != head; p = p->m_next)
        std::cerr << p->m_value << std::endl;
}

int subsequence(const Sequence& seq1, const Sequence& seq2) // Returns the earliest instance of a consecutive subsequence of seq2 in seq1.
{
    if (seq2.empty() || seq2.size() > seq1.size())  // If seq2 is empty or bigger than seq1, then return -1.
        return -1;
    ItemType val_1, val_2;
    bool is_Sequence = false;
    int startPoint;
    for (int i  = 0; i < seq1.size(); i++)  // Look through seq1.
    {   seq1.get(i, val_1);
        seq2.get(0, val_2);
        if (val_1 == val_2)     // If the first element in seq2 exists in seq1.
        {
            startPoint = i;
            for (int j = 0, iPos = i; j<seq2.size(); j++, iPos++)   // Look through the rest of seq1 and other seq2 elements.
            {
                is_Sequence = true;
                seq2.get(j, val_2);
                seq1.get(iPos, val_1);
                if(val_2 != val_1)  // If any item in seq2 doesn't exist in seq1 then break.
                {
                    is_Sequence = false;
                    break;
                }
            }
            if (is_Sequence)    // If after going through both sequences, consecutive elements exist then return the startpoint of the subsequence.
                return startPoint;
        }
    }
    return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)   // Inserts values from seq1 and seq2 into result one by one.
{
    Sequence temp_result(result);   //Use temporary sequence to avoid any case of aliasing
    while (!temp_result.empty()) // Erase all the items currently in temp_result.
        temp_result.erase(0);
    if (seq1.empty())   // If seq1 is empty, make result seq2.
        temp_result = seq2;
    else if (seq2.empty())  // If seq2 is empty, make result seq1.
        temp_result = seq1;
        ItemType val_1, val_2;
        int q = 0, r = 0, p = 0;
        while (q < seq1.size() || r < seq2.size())  // Go through both sequences until they both reach their ends.
        {
            if (q < seq1.size())        // Input value from seq1.
            {
                seq1.get(q, val_1);
                temp_result.insert(p, val_1);
                q++,p++;        // Increment seq1 and temp_result.
            }
            if (r < seq2.size())    // Then input value from seq2.
            {
                seq2.get(r, val_2);
                temp_result.insert(p, val_2);
                r++,p++;    // Increment seq2 and temp_result.
            }
        }
    result = temp_result;   // Make result equal to temp_result.
}