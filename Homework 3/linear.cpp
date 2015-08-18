//
//  linear.cpp
//  Homework 3 (q2)
//
//  Created by Jahan Cherian on 4/30/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

//NOTE THAT ALL RECURSIVE FUNCTIONS USE THE FIRST - REST DIVIDE AND CONQUER METHOD

bool allTrue(const string a[], int n)       //Checks if all the elements are true for somePredicate
{
    if(n<=0)        //If its negative index then return false
        return false;
    if(n==1)    //Return the evaluation of somePredicate at the last element
        return somePredicate(a[0]);
    return (somePredicate(*a) && allTrue(a+1,n-1)); //Return the && of the somePredicate of the current index and the recursive value of the next index
}

int countFalse(const string a[], int n) //Count the number of times somePredicate evaluates to false in our array
{
    if(n <= 0)      //Base Case: If we hit zero, or the input for n < 0 then simply return 0
        return 0;
    if(somePredicate(a[0]) == false)    //If we have a false, then increment recursively but adding 1
        return 1 + countFalse(a+1,n-1);
    return countFalse(a+1,n-1);     //Else just increment recursively
}

int firstFalse(const string a[], int n)     //Returns the first instance of somePredicate evaluating false
{
    if(n <= 0)      //Base Case: If we hit zero, or the input for n < 0 then simply return -1
        return -1;
    if (somePredicate(a[0]) == false)   //If our first element is false then just return 0 (first index)
        return 0;
    else
    {
        if (firstFalse(a+1, n-1) == -1) //If during our recursion we never have a false, then return -1, by checking the next index
            return -1;
        return 1 + firstFalse(a + 1, n-1);  //Otherwise just keep adding one so it emulates the indexing of the array
    }
}

int indexOfLeast(const string a[], int n)   //Find the index of the lowest valued string in the array
{
    if (n <= 0)     //Base Case: If we hit zero, or the input for n < 0 then simply return -1
        return -1;
    int index = 1 + indexOfLeast(a+1, n-1); //Emulates indexing in the array for recursion
    if (a[0] <= a[index])   //If our first value is less than the one ahead of it return 0
        return 0;
    else
        return index;   //Otherwise return the index of the lowest value
}

bool includes(const string a1[], int n1, const string a2[], int n2)     //Checks to see if all elements in a2 are present in a1 in the same order
{
    if (n1 == 0 && n2 == 0)     //If both n1 and n2 have ended and we havent found a false, then a2 exists in a1 and return true
        return true;
    if (n1 == 0 && n2 != 0)     //If n2 is bigger than n1 then it violates the condition of the function so return false
        return false;
    if (a2[0] == a1[0])         //If the first element of a2 matches a1 then increment both recursively
        return includes(a1+1,n1-1,a2+1,n2-1);
    else
        return includes(a1+1, n1-1, a2, n2);    //Otherwise move only a1 recursively until we hit a1[0] == a2[0]
}