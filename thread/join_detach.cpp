#include <thread>
#include <unistd.h>
#include <iostream>

void print(int * lmtAdr)
{
    std::cout << "print thread starting\n";
    int result = 0;
    for( int i = 0; i < *lmtAdr; ++i )
    {
        result += i;
    }
    std::cout << "printing result: " << result << "\n";
}

void testNoDetachNoJoin()
{
  int LIMIT = 10000;
  std::thread t(print, &LIMIT);
  std::cout << "main thread exiting with t.joinable()=" << t.joinable() << "\n";
  /*
  main thread exiting with t.joinable()=1
  print thread starting
  libc++abi.dylib: terminating
  zsh: abort      ./a.out

  现象原因: 主线程结束时,由于print线程处于joinable状态,所以print线程的destructor直接call terminate()
  导致print线程刚开始就abort
  */
}

void testDetach()
{
  int LIMIT = 10000;
  std::thread t(print, &LIMIT);
  t.detach();
  std::cout << "main thread exiting with t.joinable()=" << t.joinable() << "\n";
  /*
  main thread exiting with t.joinable()=print thread starting
  0
  printing result: 12442566

  现象原因: 由于print线程detach于主线程, 主线程main结束时整个scope移出stack,其中包括LIMIT变量
  可是此时print线程还在继续,并且继续访问LIMIT所在地址,此时得到garbage value
  */
}

int main(int argc, char const *argv[]) {
    //testNoDetachNoJoin();
    testDetach();
    return 0;
}
