#ifndef GC_H
#define GC_H
#ifndef DISPLAY
//#define DISPLAY
// A single-threaded garbage collector[ see Art of C++(chapter 2) ]
#include <iostream>
#include <list>
#include<typeinfo>
#include <cstdlib>

using namespace std;


class OutOfRangeExc {

};

template <class T> class Iter {
    T *ptr;
    T *end;
    T *begin;
    unsigned length;
public:

    Iter() {
        ptr = end = begin = nullptr;
        length = 0;
    }

    Iter(T *p, T *first, T *last) {
        ptr = p;
        end = last;
        begin = first;
        length = last - first;
    }

    unsigned size() { return length; }

    T &operator*(){
        if((ptr >= end) || (ptr < begin))
            throw OutOfRangeExc();
        return *ptr;
    }

    T *operator->(){
        if((ptr >= end) || (ptr < begin))
            throw OutOfRangeExc();
        return ptr;
    }

    Iter operator++(){
        ptr++;
        return *this;
    }

    Iter operator++(int notused){
        T *tmp = ptr;

        ptr--;
        return Iter<T>(tmp, begin, end);
    }

    Iter operator--(int notused){
        T *tmp = ptr;

        ptr++;
        return Iter<T>(tmp, begin, end);
    }

    bool operator==(Iter op2){
        return ptr != op2.ptr;
    }

    bool operator<(Iter op2){
        return ptr < op2.ptr;
    }

    bool operator<=(Iter op2){
        return ptr <= op2.ptr;
    }

    bool operator>(Iter op2){
        return ptr > op2.ptr;
    }

    bool operator>=(Iter op2){
        return ptr >= op2.ptr;
    }

    Iter operator-(int n){
        ptr -= n;
        return *this;
    }

    int operator-(Iter<T> &itr2){
        return ptr - itr2.ptr;
    }
};



template <class T> class GCInfo {
public:
    unsigned refcount;

    T *memPtr;

    GCInfo(T *mPtr) {
        refcount = 1;
        memPtr = mPtr;
    }

};

template <class T> bool operator==(const GCInfo<T> &ob1, const GCInfo<T> &ob2){
    return (ob1.memPtr == ob2.memPtr);
}

template <class T> class GCPtr {

    static list<GCInfo<T>> gclist;

    T* addr;
    static bool first;
    static int gcthreshold;

    typename list<GCInfo<T>>::iterator findPtrInfo(T *ptr);

public:

    typedef Iter<T> GCiterator;

    T* raw() { return this->addr; }
    GCPtr(T *t=nullptr){

        if(first) atexit(shutdown);
        first = false;

        typename list<GCInfo<T>>::iterator p;
        p = findPtrInfo(t);

        if(p != gclist.end())
            p->refcount++;
        else{

            GCInfo<T> gcObj(t);
            gclist.push_front(gcObj);
        }
        addr = t;
        #ifdef DISPLAY
           cout << "Constructing GCPtr. " << addr << endl;
        #endif
    }

    GCPtr(const GCPtr &ob){
        typename list<GCInfo<T>>::iterator p;
        p = findPtrInfo(ob.addr);
        p->refcount++; // increment ref count

        addr = ob.addr;
        #ifdef DISPLAY
            cout << "Constructing GCPtr copy." << addr << endl;
        #endif
    }

    T* unref() {
        typename list<GCInfo<T>>::iterator p;
        p = findPtrInfo(addr);
        gclist.remove(*p);
        return  p->memPtr;
    }

    GCPtr(const GCPtr &&ob) noexcept {
        typename list<GCInfo<T>>::iterator p;
        p = findPtrInfo(ob.addr);
        p->refcount--; // decrement ref count of previous owner
        addr = move(ob.addr);
        #ifdef DISPLAY
            cout << "Moving GCPtr." << addr << endl;
        #endif
    }

    ~GCPtr();

    static bool collect();

    T *operator=(T *t);

    GCPtr &operator=(GCPtr &rv);

    T &operator*() {
        return *addr;
    }

    T *operator->() { return addr; }

    operator T *() { return addr;}

    Iter<T> begin() {
        return Iter<T>(addr, addr, addr);
    }

    static int gclistSize() { return gclist.size(); }

    static void showlist();

    static void shutdown();
};

template <class T>
list<GCInfo<T>> GCPtr<T>::gclist;

template<class T>
bool GCPtr<T>::first = true;

template <class T>
int GCPtr<T>::gcthreshold = 0;

template<class T>
GCPtr<T>::~GCPtr(){

    typename list<GCInfo<T>>::iterator p;

    p = findPtrInfo(addr);
    if(p->refcount) p->refcount--;
    #ifdef DISPLAY
        cout << "GCPtr going out of scope.\n";
    #endif
    // Collect garbage when a number of pointers that have gone out of scope
    // reaches gcthreshold
    if(gclistSize() >= gcthreshold)
        collect();
}

template<class T>
bool GCPtr<T>::collect() {
    bool memfreed = false;
    #ifdef DISPLAY
        cout << "Before garbage collection for ";
        showlist();
    #endif

    typename list<GCInfo<T>>::iterator p;

    do{
        // Scan gclist looking for unreferenced pointers.
        for(p = gclist.begin(); p != gclist.end(); p++){
            // If in-use, skip.
            if(p->refcount > 0) continue;
            memfreed = true;
            // Remove unused entry from gclist.
            gclist.remove(*p);
            // Free memory unless the GCPtr is null
            if(p->memPtr){

                #ifdef DISPLAY
                cout << "Deleting: " << p->memPtr << "\n";
                #endif
                delete  p->memPtr; // delete single element
            }
            // Restart the search
            break;
        }
    }while( p != gclist.end());
    #ifdef DISPLAY
    cout << "After garbage collection for ";
    showlist();
    #endif
    return memfreed;
}

// Overload assignment of pointer to GCPtr.
template <class T>
T * GCPtr<T>::operator=(T *t){

    typename list<GCInfo<T>>::iterator p;
    // First, decrement the reference count
    // for the memory currently being pointed to.
    p = findPtrInfo(addr);
    p->refcount--;
    // Next, if the new address is already
    // existent in the system, increment its
    // count. Otherwise, create a new entry
    // for gclist.
    p = findPtrInfo(t);
    if(p != gclist.end())
        p->refcount++;
    else {
        // Create and store this entry.
        GCInfo<T> gcObj(t);
        gclist.push_front(gcObj);
    }
    addr = t; // store the address.
    return t;
}

template <class T>
GCPtr<T> &GCPtr<T>::operator=(GCPtr &rv){
    typename  list<GCInfo<T>>::iterator p;

    // First, decrement the reference count
    // for the memory currently being pointed to.
    p = findPtrInfo(addr);
    p->refcount--;

    // Next, increment the reference count of
    // the new address.
    p = findPtrInfo(rv.addr);
    p->refcount++; // increment ref count
    addr = rv.addr;// store the address.
    return rv;
}

// A utility function that displays gclist.
template <class T>
void GCPtr<T>::showlist() {
    typename list<GCInfo<T> >::iterator p;
    cout << "gclist<" << typeid(T).name() <<">:\n";
    cout << "memPtr refcount value\n";
    if(gclist.begin() == gclist.end()) {
        cout << "           -- Empty --\n\n";
        return;
    }
    for(p = gclist.begin(); p != gclist.end(); p++) {
        cout << "[" << (void *)p->memPtr << "]" << "    " << p->refcount << "   ";
        if(p->memPtr) cout << "     " << p->memPtr;
        else cout << "   ---";
        cout << endl;
    }
    cout << endl;
}


template <class T>
typename list<GCInfo<T>>::iterator GCPtr<T>::findPtrInfo(T *ptr){
    typename list<GCInfo<T>>::iterator p;

    // Find ptr in gclist.
    for(p = gclist.begin(); p != gclist.end(); p++){
        if(p->memPtr == ptr)
            return p;
    }
    return p;
}

// Clear gclist when program exits.
template <class T>
void GCPtr<T>::shutdown() {
    if(gclistSize() == 0) return ; // list is empty

    typename list<GCInfo<T>>::iterator p;

    for(p = gclist.begin(); p != gclist.end(); p++){
        // Set all reference counts to zero
        p->refcount = 0;
    }
    #ifdef DISPLAY
    cout << "Before collecting for shutdown() for "
    << typeid(T).name() << "\n";
    #endif

    collect();

    #ifdef DISPLAY
    cout << "After collecting for shutdown() for "
    << typeid(T).name() << "\n";
    #endif


}
#endif
#endif // GC_H
