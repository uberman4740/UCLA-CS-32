// Dictionary.cpp

// This is a correct but horribly inefficient implementation of Dictionary
// functionality.  Your job is to change DictionaryImpl to a more efficient
// correct implementation.  You can change it any way you like, subject to
// restrictions in the spec (e.g., that the only C++ library container you
// are allowed to use are vector, list, stack, and queue (and string); if you
// want anything fancier, implement it yourself).

#include "Dictionary.h"
#include <string>
#include <cctype>
#include <algorithm>  // for swap
using namespace std;

const int HASH_TABLE_SIZE = 50000;
const int PRIME_LIMIT = 26;

void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);

  // This class does the real work of the implementation.

struct Node     //Node that is used to build a linked list in each bucket
{
    Node(string val, int key, unsigned long super)  //Holds a word, key, superKey (unique identifier) and a null next pointer
    : value(val), key(key), next(nullptr), superKey(super)
    {}
    string value;
    int key;
    unsigned long superKey;
    Node* next;
};

class DictionaryImpl
{
  public:
    DictionaryImpl();   //Sets all the values in the hash_table to be null
    ~DictionaryImpl();  //Destroys any Nodes that have been created
    void insert(string& word);
    void lookup(string& letters, void callback(string));
  private:
    Node* hash_table[HASH_TABLE_SIZE];  //Array of Node pointers that is our hashtable
    int hashFunction(string& word);     //Hash function to map our key to the bucket
    unsigned long primeMultiple;    //This acts as our unique identifier
};

DictionaryImpl::DictionaryImpl()
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        hash_table[i] = nullptr;
}

DictionaryImpl::~DictionaryImpl()
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)   //Go through all of the hash table
    {
        Node* start = hash_table[i];
        while (start != nullptr)    //Follow the linked list in each bucket, destroying each node
        {
            Node* previous = start;
            start = start->next;
            delete previous;
        }
    }
}
int DictionaryImpl::hashFunction(string& word)
{
    removeNonLetters(word);     //Remove any unnecessary characters in the word
    
    int prime[PRIME_LIMIT] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};  //Array of prime numbers for each character to map to
    primeMultiple = 1;
    for (int i = 0; i < word.length(); i++)
        primeMultiple *= prime[word[i] - 'a']; //Multiply each character (as its prime num value) with the next character
    return primeMultiple % HASH_TABLE_SIZE; //Get the key
}

void DictionaryImpl::insert(string& word)
{
    int hash_key = hashFunction(word);
    Node*& newEntry = hash_table[hash_key]; //Our new word is set to its relative bucket
    if (newEntry == nullptr)    //If that bucket is empty, add our value
        newEntry= new Node(word, hash_key,primeMultiple);
    else    //Otherwise go through the items in the bucket until we get to the end and add it there
    {
        Node* temp = new Node(word,hash_key,primeMultiple);
        Node* last = newEntry;
        while (last->next) last = last->next;
        last->next = temp;
    }
}

void DictionaryImpl::lookup(string& letters, void callback(string))
{
    if (callback == nullptr)
        return;

    if (letters.empty())
        return;
    
    int hash_key = hashFunction(letters);
    const unsigned long callerSuper = primeMultiple;    //Unique identifier
    Node* caller = hash_table[hash_key];
    while (caller != nullptr)
    {
        if (caller->superKey == callerSuper)    //Each anagram has the same unique identifier
        {
            callback(caller->value);
            caller = caller->next;
        }
        else
            caller = caller->next;
    }
}
    
void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop off everything from "to" to end.
} 

  // Each successive call to this function generates the next permutation of the
  // characters in the parameter.  For example,
  //    string s = "eel";
  //    generateNextPermutation(s);  // now s == "ele"
  //    generateNextPermutation(s);  // now s == "lee"
  //    generateNextPermutation(s);  // now s == "eel"
  // You don't have to understand this function's implementation.
void generateNextPermutation(string& permutation)
{
    string::iterator last = permutation.end() - 1;
    string::iterator p;

    for (p = last; p != permutation.begin()  &&  *p <= *(p-1); p--)
        ;
    if (p != permutation.begin())
    {
        string::iterator q;
        for (q = p+1; q <= last  &&  *q > *(p-1); q++)
            ;
        swap(*(p-1), *(q-1));
    }
    for ( ; p < last; p++, last--)
        swap(*p, *last);
}

//******************** Dictionary functions **********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code.

Dictionary::Dictionary()
{
    m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters, callback);
}
