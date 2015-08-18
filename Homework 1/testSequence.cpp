//
//  main.cpp
//  Homework (q3)
//
//  Created by Jahan Cherian on 4/11/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;


void test()
{
    Sequence s;
    assert(s.insert(0, 10));
    assert(s.insert(0, 20));
    assert(s.size() == 2);
    ItemType x = 999;
    assert(s.get(0, x) && x == 20);
    assert(s.get(1, x) && x == 10);
}

int main()
{
    test();
    Sequence s;
    Sequence a;
    
    assert(s.empty()); // check to see if s set is empty
    assert(a.empty()); // check to see if a set is empty
    
    for (int i = 0; i < DEFAULT_MAX_ITEMS+5; i++)
    {
        s.insert(i);        // Insert 200 items in s
    }
    
    assert(s.size() == 200);    // Check to see if s contains 200
    s.swap(a);  // Swap the random values from a for numbers 0-200 into a
    assert(a.size() == 200);    // Check to see if the size is 200
    for (int k = 0; k < 100; k++)
    {
        a.remove(k);    //remove the first 100 numbers
    }
    assert(a.size() == 100);    // Checks if size is now 100
    
    Sequence r;
    r.insert(r.size(), 21); // Inserts at the last position
    r.insert(r.size(), 42); // Inserts at the last position
    assert(r.find(42) == 1); 
    assert(!r.insert(5, 69));
    assert(!r.erase(3));
    assert(r.erase(1));
    assert(r.size() == 1);
    
    unsigned long swag = 33;
    assert(r.get(0, swag) && swag == 21);
    unsigned long remix = 968;
    assert(r.set(0, remix) && r.find(968) == 0);
    cout << "Passed all tests" << endl;
}