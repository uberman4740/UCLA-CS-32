//
//  Sequence.h
//  Project 2
//
//  Created by Jahan Cherian on 4/15/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdio.h>
#include <string>

typedef std::string ItemType;

class Sequence
{
public:
    Sequence();     // Create an empty sequence
    Sequence(const Sequence& other);  // Copy Constructor for Sequence.
    bool empty() const;     // Return true if Sequence is empty, otherwise false.
    int size() const;       // Return the number of items in the sequence.
    bool insert(int pos, const ItemType& value);
    // Insert value into the sequence so that it becomes the item at
    // position pos.  The original item at position pos and those that
    // follow it end up at positions one higher than they were at before.
    // Return true if 0 <= pos <= size() and the value could be
    // inserted. Otherwise, leave the sequence unchanged and return false.
    // If pos is equal to size(), the value is inserted at the end.
    bool insert(const ItemType& value);
    // Let p be the smallest integer such that value <= the item at
    // position p in the sequence; if no such item exists (i.e.,
    // value > all items in the sequence), let p be size().  Insert
    // value into the sequence so that it becomes the item at position
    // p. The original item at position p and those that follow it end
    // up at positions one higher than before.  Return true if the value
    // was actually inserted.  Return false if the value was not inserted
    // (perhaps because the sequence has a fixed capacity and is full).
    // This essentially creates a sorted Sequence in ascending order.
    bool erase(int pos);
    // If 0 <= pos < size(), remove the item at position pos from
    // the sequence (so that all items that followed this item end up at
    // positions one lower than they were at before), and return true.
    // Otherwise, leave the sequence unchanged and return false.
    int remove(const ItemType& value);
    // Erase all items from the sequence that == value.  Return the
    // number of items removed (which will be 0 if no item == value).
    bool get(int pos, ItemType& value) const;
    // If 0 <= pos < size(), copy into value the item at position pos
    // in the sequence and return true.  Otherwise, leave value unchanged
    // and return false.
    bool set(int pos, const ItemType& value);
    // If 0 <= pos < size(), replace the item at position pos in the
    // sequence with value and return true.  Otherwise, leave the sequence
    // unchanged and return false.
    int find(const ItemType& value) const;
    // Let p be the smallest integer such that value == the item at
    // position p in the sequence; if no such item exists, let p be -1.
    // Return p.
    void swap(Sequence& other);
    // Exchange the contents of this sequence with the other one.
    Sequence& operator=(const Sequence& other);
    // Assignment operator for Sequence.
    ~Sequence();
    // Destructor for Sequence.
    void dump() const;
    // Helper function to print out the values in the Sequence to cerr.
    
private:
    struct Node // Create a node for the Sequence implemented as a Linked List.
    {
        ItemType m_value;
        Node* m_next;       // Connects Nodes forward.
        Node* m_previous;   // Connects Nodes backwards.
    };
    int m_size;
    Node* head; // Head pointer for the Linked List.
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
// Return the earliest position of the occurrence when seq1 contains consecutive elements
// from seq2. If no such sub sequence exists or seq2 is bigger than seq1 then return -1.
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
// Store elements from seq1 and seq2 into result in a an alternating order, such that 1st element
// of result is the 1st element from seq1 and the 2nd element of result is the 1st element of seq2,
// clearing result of whatever it previously contained.
// If either sequence is empty, then when this function returns, result must consist of a copy of the
// other sequence, and no other items.

#endif /* defined(SEQUENCE_H) */
