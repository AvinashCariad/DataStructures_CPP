#ifndef GUARD_Vec
#define GUARD_Vec

#include <memory>

template <class T> class Vec {
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;

    Vec() { create(); }
    explicit Vec(size_type n, const T& t = T()) { create(n, t); }
    Vec(const Vec& v) { create(v.begin(), v.end()); }
    Vec& operator=(const Vec&);
    ~Vec() { uncreate(); }

    template <class In> Vec(In b, In e) { create(b, e); } // Q10

    T& operator[](size_type i) { return data[i]; }
    const T& operator[](size_type i) const { return data[i]; }

    void push_back(const T& t) {
        if (avail == limit)
            grow();
        unchecked_append(t);
    }

    size_type size() const { return avail - data; }

    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    iterator end() { return avail; }
    const_iterator end() const { return avail; }

    void clear() { uncreate(); }
    iterator erase(iterator);
    iterator erase(iterator, iterator);
    bool empty() const { return data == avail; }

    template <class In> iterator insert(iterator, In, In); // Q12

    void assign(iterator, iterator); // Q13

private:
    iterator data;  // first element in the Vec
    iterator avail; // (one past) the last element in the Vec
    iterator limit; // (one past) the allocated memory

    // facilities for memory allocation
    std::allocator<T> alloc; // object to handle memory allocation

    // allocate and initialize the underlying array
    void create();
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);
    template <class In> void create(In, In); // Q14

    // destroy the elements in the array and free the memory
    void uncreate();

    // support functions for push_back
    void grow();
    void unchecked_append(const T&);
};

// code is taken from §11.3.2/197
template <class T>
Vec<T>& Vec<T>::operator=(const Vec& rhs)
{
    // check for self-assignment
    if (&rhs != this) {

        // free the array in the left-hand side
        uncreate();

        // copy elements from the right-hand to the left-hand side
        create(rhs.begin(), rhs.end());
    }
    return *this;
}

// code is taken from §11.5.1/207
template <class T> void Vec<T>::create()
{
    data = avail = limit = 0;
}

// code is taken from §11.5.1/207
template <class T> void Vec<T>::create(size_type n, const T& val)
{
    data = alloc.allocate(n);
    limit = avail = data + n;         // set limit and avail 
    std::uninitialized_fill(data, limit, val);
}

// code is taken from §11.5.1/207
template <class T>
void Vec<T>::create(const_iterator i, const_iterator j)
{
    data = alloc.allocate(j - i);
    limit = avail = std::uninitialized_copy(i, j, data);
}

template <class T>
template <class In>
void Vec<T>::create(In i, In j)
{
    data = alloc.allocate(j - i);
    limit = avail = std::uninitialized_copy(i, j, data);
}

// code is taken from §11.5.1/208
template <class T> void Vec<T>::uncreate()
{
    if (data) {
        // destroy (in reverse order) the elements that were constructed
        iterator it = avail;
        while (it != data)
            alloc.destroy(--it);

        // return all the space that was allocated
        alloc.deallocate(data, limit - data);
    }
    // reset pointers to indicate that the Vec is empty again
    data = limit = avail = 0;
}

// code is taken from §11.5.1/207
template <class T> void Vec<T>::grow()
{
    // when growing, allocate twice as much space as currently in use
    size_type new_size = std::max(2 * (limit - data), ptrdiff_t(1));

    // allocate new space and copy existing elements to the new space
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(data, avail, new_data);

    // return the old space
    uncreate();

    // reset pointers to point to the newly allocated space
    data = new_data;
    avail = new_avail;
    limit = data + new_size;
}

// code is taken from §11.5.1/207
// assumes avail points at allocated, but uninitialized space
template <class T> void Vec<T>::unchecked_append(const T& val)
{
    alloc.construct(avail++, val);
}

template <class T> typename Vec<T>::iterator Vec<T>::erase(iterator pos)
{
    // create a new vec
    Vec<T> new_vec;
    iterator ret;

    // add elements except the one at pos
    for (iterator it = begin(); it != end(); ++it) {
        if (it != pos)
            new_vec.push_back(*it);
        else
            ret = new_vec.end();
    }

    // return the iterator pointing after the erased element
    return ret;
}

template <class T>
typename Vec<T>::iterator Vec<T>::erase(iterator b, iterator e)
{
    // create a new vec
    Vec<T> new_vec;
    iterator ret;

    // add elements except the one at pos
    for (iterator it = begin(); it != end(); ++it) {
        if (it < begin && it >= end)          // note teh begin and end here
            new_vec.push_back(*it);
        if (it == end)
            ret = new_vec.end();
    }

    // return the iterator pointing after the erased elements
    return ret;
}

template <class T>
template <class In>
typename Vec<T>::iterator Vec<T>::insert(iterator d, In b, In e)
{
    // save the tail and the difference
    Vec<T> end(d, avail);
    ptrdiff_t diff = d - data;

    // destroy the tail
    if (d) {
        // destroy (in reverse order) the elements that were constructed
        while (avail != d)
            alloc.destroy(--avail);
    }
    
    // insert the items
    while (b != e)
        push_back(*b++);

    // add the tail
    for (Vec<T>::const_iterator it = end.begin();
         it != end.end(); ++it)
        push_back(*it);

    return data + diff;
}

template <class T>
void Vec<T>::assign(iterator b, iterator e)
{
    // destroy the Vec
    uncreate();

    // push all the elements denoted by the iterators
    while (b != e)
        push_back(*b++);
}
#endif

#include <iostream>
#include <string>

// Helper function to easily print the elements of our custom Vec
template <class T>
void printVec(const std::string& label, const Vec<T>& v) {
    std::cout << label << ": [ ";
    for (typename Vec<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "] (Size: " << v.size() << ")\n";
}

// ===============================================================================/

// main code harness:
int main(int argc, char** argv) {
    std::cout << "=== Testing Vec Constructors & push_back ===\n";
    
    // 1. Test Default Constructor
    Vec<int> v1;
    std::cout << "v1 empty? " << (v1.empty() ? "Yes" : "No") << "\n";

    // 2. Test push_back (triggers grow internally)
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);
    printVec("v1 after push_back", v1);

    // 3. APPLY WORKAROUND HERE: Force size_type using 4u to avoid template match conflict
    Vec<int> v2(4u, 99); 
    printVec("v2 (sized constructor with default values)", v2);

    // 4. Test Copy Constructor
    Vec<int> v3(v1);
    printVec("v3 (copied from v1)", v3);

    std::cout << "\n=== Testing Assignment and Operators ===\n";
    
    // 5. Test Copy Assignment Operator (=)
    v2 = v1;
    printVec("v2 (after v2 = v1 assignment)", v2);

    // 6. Test Subscript Operator ([]) and element mutation
    v1[0] = 55;
    std::cout << "v1[0] modified to: " << v1[0] << "\n";
    printVec("v1 modified", v1);
    printVec("v2 unchanged (deep copy verification)", v2);

    std::cout << "\n=== Testing Custom Iterator Methods (insert & assign) ===\n";

    // 7. Test range-based Constructor (Q10) & Iterator methods
    int arr[] = {1, 2, 3, 4, 5};
    Vec<int> v4(arr, arr + 5);
    printVec("v4 initialized from raw array pointers", v4);

    // 8. Test custom insert function (Q12)
    std::cout << "Inserting elements into v4..." << "\n";
    v4.insert(v4.begin() + 2, v1.begin(), v1.end());
    printVec("v4 after inserting v1 at index 2", v4);

    // 9. Test assign function (Q13)
    std::cout << "Re-assigning values to v1..." << "\n";
    v1.assign(arr, arr + 3);
    printVec("v1 after assigning first 3 elements of raw array", v1);

    // 10. Test clear functionality
    v1.clear();
    std::cout << "v1 after clear(). Is empty? " << (v1.empty() ? "Yes" : "No") << "\n";

    return 0;
}


