#include <thread>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <atomic>

struct Spinlock
{
    Spinlock()
    : isLocked_(false)
    {}

    ~Spinlock()
    {}

    // this is blocking
    void lock()
    {
        bool expected = false;
        while( !isLocked_.compare_exchange_weak(expected, true, std::memory_order_acquire) )
        {
            // if compare_exchange_weak failed to change the value, it will load isLocked_'s current value into expected
            // therefore, we need to reset expected
            // also, compare_exchange_weak requires a lvalue there; that is why we are using expected variable instead of false
            expected = false;
        }
    }

    //
    void unlock()
    {
        isLocked_.store(false, std::memory_order_release);
        // what does memory order mean? check here - https://stackoverflow.com/questions/12346487/what-do-each-memory-order-mean
    }

private:
    std::atomic<bool> isLocked_;
    // here, there is a difference between std::atomic<bool> and std::atomic_flag
    // std::atomic_flag is guaranteed to be lock free and implemented with hardware architecture
    // whereas std::atomic<> could be implemented with inner lock to achieve the atomicity
};

Spinlock sp;

void output(char *& tname)
{
    sp.lock();
    std::cout << "Thread " << tname;
    sleep(1);
    std::cout << tname << " starting\n";
    sp.unlock();
}

int main(int argc, char * argv[])
{
    std::vector<std::thread> tpool;
    for( int i = 1; i < argc; ++i )
    {
        std::thread t(output, std::ref(argv[i]));
        tpool.push_back(std::move(t));
    }
    
    for( int i = 0; i < tpool.size(); ++i )
    {
        tpool[i].join();
    }

    return 0;
}

/**
OUTPUT
I. WITHOUT SPINLOCK
[bzhang165@nylxdev1 thread (master)]$ ./a.out t1 t2 t3
Thread t1Thread t2Thread t3t1 starting
t2 starting
t3 starting

II. WITH SPINLOCK
[bzhang165@nylxdev1 thread (master)]$ ./a.out t1 t2 t3
Thread t1t1 starting
Thread t3t3 starting
Thread t2t2 starting

REFERENCE
https://cis.temple.edu/~giorgio/old/cis307f05/readings/spinsem.html
https://stackoverflow.com/questions/26583433/c11-implementation-of-spinlock-using-atomic
https://stackoverflow.com/questions/12346487/what-do-each-memory-order-mean
*/
