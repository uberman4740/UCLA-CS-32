//
//  newSequence.h
//  Homework (q5)
//
//  Created by Jahan Cherian on 4/11/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#ifndef NEW_SEQUENCE_H
#define NEW_SEQUENCE_H

#include <iostream>
#include <string>
#include <stdio.h>

const int DEFAULT_MAX_ITEMS = 200;

typedef unsigned long ItemType;

class Sequence
{
public:
    Sequence(int size = DEFAULT_MAX_ITEMS);    // Create a new Sequence of size, defaulted to DEFAULT_MAX_ITEMS
    Sequence(const Sequence& other);  // Copy constructor for the new Sequence.
    Sequence& operator=(const Sequence& rhs); // Assignment operator for = for the new Sequence.
    ~Sequence();     // Destructor for the new Sequence.
    bool empty() const;  // Return true if the Sequence is empty, otherwise false.
    int size() const;    // Return the number of items in the Sequence.
    bool insert(int pos, const ItemType& value);
    // Insert value into the Sequence so that it becomes the item at
    // position pos.  The original item at position pos and those that
    // follow it end up at positions one higher than they were at before.
    // Return true if 0 <= pos <= size() and the value could be
    // inserted.  (It might not be, if the Sequence has a fixed capacity,
    // e.g., because it's implemented using a fixed-size array.)  Otherwise,
    // leave the Sequence unchanged and return false.  Notice that
    // if pos is equal to size(), the value is inserted at the end.
    
    bool insert(const ItemType& value);
    // Let p be the smallest integer such that value <= the item at
    // position p in the Sequence; if no such item exists (i.e.,
    // value > all items in the Sequence), let p be size().  Insert
    // value into the Sequence so that it becomes the item at position
    // p.  The original item at position p and those that follow it end
    // up at positions one higher than before.  Return true if the value
    // was actually inserted.  Return false if the value was not inserted
    // (perhaps because the Sequence has a fixed capacity and is full).
    
    bool erase(int pos);
    // If 0 <= pos < size(), remove the item at position pos from
    // the Sequence (so that all items that followed this item end up at
    // positions one lower than they were at before), and return true.
    // Otherwise, leave the Sequence unchanged and return false.
    
    int remove(const ItemType& value);
    // Erase all items from the Sequence that == value.  Return the
    // number of items removed (which will be 0 if no item == value).
    
    bool get(int pos, ItemType& value) const;
    // If 0 <= pos < size(), copy into value the item at position pos
    // in the Sequence and return true.  Otherwise, leave value unchanged
    // and return false.
    
    bool set(int pos, const ItemType& value);
    // If 0 <= pos < size(), replace the item at position pos in the
    // Sequence with value and return true.  Otherwise, leave the Sequence
    // unchanged and return false.
    
    int find(const ItemType& value) const;
    // Let p be the smallest integer such that value == the item at
    // position p in the Sequence; if no such item exists, let p be -1.
    // Return p.
    
    void swap(Sequence& other);
    // Exchange the contents of this Sequence with the other one.
    
private:
    int m_size;
    int max_cap;
    ItemType* m_value;
};

#endif /* defined(NEW_SEQUENCE_H) */
