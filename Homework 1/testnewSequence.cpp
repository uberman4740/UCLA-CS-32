//
//  testnewSequence.cpp
//  Homework (q5)
//
//  Created by Jahan Cherian on 4/11/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence uls;
    assert(uls.insert(10));
    assert(uls.insert(20));
    assert(uls.size() == 2);
    //assert(uls.contains(20));
    ItemType x = 30;
    assert(uls.get(0, x)  &&  (x == 10  ||  x == 20));
}

int main()
{
    test();
    Sequence a(1000);   // a can hold at most 1000 items
    Sequence b(5);      // b can hold at most 5 items
    Sequence c;         // b can hold at most DEFAULT_MAX_ITEMS items
    ItemType v = 69;
    
    // No failures inserting 5 items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v));
    
    // Failure if we try to insert a sixth item into b
    assert(!b.insert(v));
    
    // When two Sequences' contents are swapped, their capacities are
    // swapped as well:
    a.swap(b);
    assert(!a.insert(v)  &&  b.insert(v));
    cout << "Passed all tests" << endl;
}