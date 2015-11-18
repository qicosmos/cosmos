#include <iostream>
#include <string>
#include "thread_pool.hpp"

void test_pool()
{
   thread_pool pool;
   for(size_t i = 0; i < 1000; i++)
   {
       pool.add_task([]{
               std::this_thread::sleep_for(std::chrono::microseconds(50));
               });
   }

   std::string s;
   std::cin>>s;
 
}

int main()
{
   test_pool();

   return 0;
}
