#include <iostream>
#include <algorithm>
#include <numeric>  // for std::accumulate
#include <vector>
#include <string>
#include <cstdlib>  // for std::rand
#include <cassert>

using namespace std;

//========================================================================
//  Set this to false to skip the insertion sort tests; you'd do this if
//  you're sorting so many items that insertion_sort would take more time
//  than you're willing to wait.

const bool TEST_INSERTION_SORT = true;

//========================================================================

//========================================================================
// Timer t;                 // create a timer
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#if __cplusplus >= 201103L  // C++11

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

#elif defined(_MSC_VER)  // not C++11, but Windows

#include <windows.h>

class Timer
{
public:
    Timer()
    {
        QueryPerformanceFrequency(&ticksPerSecond);
        start();
    }
    void start()
    {
        QueryPerformanceCounter(&m_time);
    }
    double elapsed() const
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000.0 * (now.QuadPart - m_time.QuadPart)) / ticksPerSecond.QuadPart;
    }
private:
    LARGE_INTEGER m_time;
    LARGE_INTEGER ticksPerSecond;
};

#else // not C++11 or Windows, so C++98

#include <ctime>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::clock();
    }
    double elapsed() const
    {
        return (1000.0 * (std::clock() - m_time)) / CLOCKS_PER_SEC;
    }
private:
    std::clock_t m_time;
};

#endif

//========================================================================

// Here's a class that is not cheap to copy because the objects contain
// a large array.

// We'll simplify writing our timing tests by declaring everything public
// in this class.  (We wouldn't make data public in a class intended for
// wider use.)

typedef int IdType;

const int NWEEKS = 3 * 52;

struct Store
{
    IdType id;
    double total;
    double weeklySales[NWEEKS];
    Store(IdType i) : id(i)
    {
        // create random sales figures (from 20 to 60)
        for (size_t k = 0; k < NWEEKS; k++)
            weeklySales[k] = 20 + rand() % 40;
        // (accumulate computes 0.0 + weeklySales[0] + weeklySales[1] + ...)
        total = accumulate(weeklySales, weeklySales + NWEEKS, 0.0);
    }
};

inline
bool compareStore(const Store& lhs, const Store& rhs)
{
    // The Store with the higher total should come first.  If they have
    // the same total, then the Store with the smaller id number should
    // come first.  Return true iff lhs should come first.  Notice that
    // this means that a false return means EITHER that rhs should come
    // first, or there's a tie, so we don't care which comes first,
    
    if (lhs.total > rhs.total)
        return true;
    if (lhs.total < rhs.total)
        return false;
    return lhs.id < rhs.id;
}

inline
bool compareStorePtr(const Store* lhs, const Store* rhs)
{
    return compareStore(*lhs, *rhs);
}

bool isSorted(const vector<Store>& s)
{
    // Return true iff the vector is sorted according to the ordering
    // relationship compareStore
    
    for (size_t k = 1; k < s.size(); k++)
    {
        if (compareStore(s[k], s[k-1]))
            return false;
    }
    return true;
}

void insertion_sort(vector<Store>& s, bool comp(const Store&, const Store&))
{
    for (int unsorted = 1; unsorted < s.size(); ++unsorted)
    {
        Store nextItem = s[unsorted];
        int loc = unsorted;
        for (; (loc > 0) && comp(nextItem,s[loc-1]); --loc)
            s[loc] = s[loc-1];
        s[loc] = nextItem;
    }
}

// Report the results of a timing test

void report(string caption, double t, const vector<Store>& s)
{
    cout << t << " milliseconds; " << caption
    << "; first few stores are\n\t";
    size_t n = s.size();
    if (n > 5)
        n = 5;
    for (size_t k = 0; k < n; k++)
        cout << " (" << s[k].id << ", " << s[k].total << ")";
    cout << endl;
}

int main()
{
    size_t nstores;
    cout << "Enter number of stores to sort: ";
    cin >> nstores;
    if ( !cin  ||  nstores <= 0)
    {
        cout << "You must enter a positive number.  Goodbye." << endl;
        return 1;
    }
    
    // Create a random ordering of id numbers 0 through nstores-1
    vector<IdType> ids;
    for (size_t j = 0; j < nstores; j++)
        ids.push_back(IdType(j));
    random_shuffle(ids.begin(), ids.end());  // from <algorithm>
    
    // Create a bunch of Stores
    vector<Store> unorderedStores;
    for (size_t k = 0; k < ids.size(); k++)
        unorderedStores.push_back(Store(ids[k]));
    
    // Create a timer
    
    Timer timer;
    
    // Sort the Stores using the STL sort algorithm.  It uses a variant
    // of quicksort called introsort.
    
    vector<Store> stores(unorderedStores);
    timer.start();
    sort(stores.begin(), stores.end(), compareStore);
    report("STL sort", timer.elapsed(), stores);
    assert(isSorted(stores));
    
    // Sort the already sorted array using the STL sort.  This should be
    // fast.
    
    timer.start();
    sort(stores.begin(), stores.end(), compareStore);
    report("STL sort if already sorted", timer.elapsed(), stores);
    assert(isSorted(stores));
    
    if (TEST_INSERTION_SORT)
    {
        // Sort the original unsorted array using insertion sort.  This
        // should be really slow.  If you have to wait more than a minute,
        // try rerunning the program with a smaller number of Stores.
        
        stores = unorderedStores;
        timer.start();
        insertion_sort(stores, compareStore);
        report("insertion sort if not already sorted", timer.elapsed(), stores);
        assert(isSorted(stores));
        
        // Sort the already sorted array using insertion sort.  This should
        // be fast.
        
        timer.start();
        insertion_sort(stores, compareStore);
        report("insertion sort if already sorted", timer.elapsed(), stores);
        assert(isSorted(stores));
    }
    
    // Since Stores are expensive to copy, and since the STL's sort copies
    // Stores O(N log N) times, let's sort POINTERS to the Stores, then
    // make one final pass to rearrange the Stores according to the
    // reordered pointers.  We'll write some code; you write the rest.
    
    // Set stores to the original unsorted sequence
    stores = unorderedStores;
    
    // Start the timing
    timer.start();
    
    // Create an auxiliary copy of stores, to faciliate the later reordering.
    // We create it in a local scope so that we also account for the
    // destruction time.
    {
        vector<Store> auxStores(stores);
        
        // TODO:  Create a vector of Store pointers, and set each pointer
        //        to point to the corresponding Store in auxStores.
        vector<Store*> storepointers;
        for (int i = 0; i < auxStores.size(); i++)
        {
            storepointers.push_back(&auxStores[i]);
        }
        
        // TODO:  Sort the vector of pointers using the STL sort algorithm
        //        with compareStorePtr as the ordering relationship.
        sort(storepointers.begin(), storepointers.end(), compareStorePtr);
        
        // TODO:  Using the now-sorted vector of pointers, replace each Store
        //        in stores with the Stores from auxStores in the correct order.
        for (int j = 0; j < storepointers.size(); j++)
        {
            stores[j] = *storepointers[j];
        }
    } // auxStores will be destroyed here
    
    // Report the timing and verify that the sort worked
    report("STL sort of pointers", timer.elapsed(), stores);
    assert(isSorted(stores));
}