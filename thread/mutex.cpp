#include <thread>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <mutex>

std::mutex mtx;

void output(char *& tname)
{
    mtx.lock(); // mutex is needed for synchronizing print from each thread
    std::cout << "Thread " << tname;
    sleep(1);
    std::cout << tname << " starting\n";
    mtx.unlock();// mutex is needed for synchronizing print from each thread
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
I. WITHOUT MUTEX L11 L15
[bzhang165@nylxdev1 thread (master)]$ ./a.out t1 t2 t3
Thread t1Thread t2Thread t3t1 starting
t2 starting
t3 starting

II. WITH MUTEX L11 L15
[bzhang165@nylxdev1 thread (master)]$ ./a.out t1 t2 t3
Thread t1t1 starting
Thread t2t2 starting
Thread t3t3 starting
*/
