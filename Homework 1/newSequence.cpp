//
//  newSequence.cpp
//  Homework (q5)
//
//  Created by Jahan Cherian on 4/11/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include "newSequence.h"
#include <iostream>
#include <cstdlib>

Sequence::Sequence(int size)    // Initializes the Sequence to be of size by declaring its m_size as size
{
    if (size < 0)
    {
        std::cout << "Size must be non negative!\n";
        exit(1);
    }
    else
    {
        m_size = 0;
        max_cap = size;
        m_value = new ItemType[max_cap];
    }
}

Sequence::Sequence(const Sequence& other)
{
    m_size = other.m_size;
    max_cap = other.max_cap;
    m_value = new ItemType[max_cap];
    for (int i = 0; i < m_size; i++)
    {
        m_value[i] = other.m_value[i];
    }
}

Sequence& Sequence::operator=(const Sequence& rhs)
{
    if (this != &rhs)
    {
        Sequence temp(rhs);
        swap(temp);
    }
    return *this;
}

Sequence::~Sequence()
{
    delete [] m_value;
}

bool Sequence::empty() const    // The Sequence is empty if its m_size is zero
{
    if (m_size > 0)
    {
        return false;
    }
    return true;
}

int Sequence::size() const  // Returns the m_size, which is essentially the size of the Sequence
{
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)   // Places value at pos in the Sequence
{
    if (pos >= 0 && pos <= size())
    {
        if (size() >= max_cap)
        {
            return false;
        }
        m_size++;        // Increase the size by 1
        if (pos == size())
        {
            m_value[size() - 1] = value;
        }
        for (int i = size() - 1; i >= pos; i--)  // Traverse backwards through the Sequence and copy all the other items over placing them one
            // higher
            m_value[i] = m_value[i-1];
        m_value[pos] = value;   // Now set the value at pos to the value in the argument of the method
        return true;
    }
    return false;   // Return false if the pos is less than zero or greater than the size of the Sequence
}

bool Sequence::insert(const ItemType& value)    // Places value within the Sequence
{
    int p = 0;  // Default position is set as zero
    
    for (int i = 0; i < size(); i++)
    {
        if (m_value[i] >= value)    // If in Sequence we find a value less than the argument value, then set this position as the pos and break
        {
            p = i;
            break;
        }
        else        // Otherwise, if the value is greater than all the items in the Sequence, make p the size of the Sequence
        {
            p = size();
        }
    }
    if (p >= 0 && p <= size())  // Then insert the item in p in the Sequence
    {
        if (m_size < max_cap)
        {
            m_size++;
            for (int i = size() - 1; i >= p; i--)
                m_value[i] = m_value[i-1];
            m_value[p] = value;
            return true;
        }
        else{
            return false;
        }
    }

    return false;
}

bool Sequence::erase(int pos)       // Erases the value at the position pos in the Sequence
{
    if (pos >= 0 && pos < size())
    {
        for (int i = pos; i < size(); i++)  // Loop through the Sequence and move all the items from pos to the end back by one, thus
            // effectively deleting the item at pos and then decrease the size by one
        {
            m_value[i] = m_value[i+1];
        }
        m_size--;
        return true;
    }
    return false;
}

int Sequence::remove(const ItemType& value) // Removes the value in the Sequence if it exists
{
    int removed = 0;
    
    for (int i = 0; i < size(); i++)        // Loop through the Sequence and check for whether value exists in the Sequence
    {
        if (m_value[i] == value)
        {
            for (int k = i; k < size(); k++)    // If the item exits, then move the items in the Sequence down by one and increase removed by 1
                m_value[k] = m_value[k+1];
            m_size--;
            removed++;
        }
    }
    return removed;
}

bool Sequence::get(int pos, ItemType& value) const      // Sets the value to the value in the Sequence at position pos
{
    if (pos >= 0 && pos < size())
    {
        value = m_value[pos];
        return true;
    }
    return false;
}

bool Sequence::set(int pos, const ItemType& value)      // Sets the value in the Sequence to the value paramenter at position pos
{
    if (pos >= 0 && pos < size())
    {
        m_value[pos] = value;
        return true;
    }
    return false;
}

int Sequence::find(const ItemType& value) const     // Finds the value passed within the Sequence if such an item exists
{
    int p = -1;
    if (size() > 0)
    {
        for (int i = 0; i < size(); i++)
        {
            if (m_value[i] == value)
            {
                p = i;
                return p;
            }
        }
    }
    return p;
}

void Sequence::swap(Sequence& other)        // Swaps the values in two Sequences, by creating a temporary assist array
{
    ItemType* temp_ptr;
    
    temp_ptr = m_value;
    m_value = other.m_value;
    other.m_value = temp_ptr;
    
    int temp_max_cap;
    temp_max_cap = max_cap;
    max_cap = other.max_cap;
    other.max_cap = temp_max_cap;
    
    int temp_size;
    
    temp_size = m_size;
    m_size = other.m_size;
    other.m_size = temp_size;
}