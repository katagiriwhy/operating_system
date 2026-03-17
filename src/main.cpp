#include "schedular.hpp"
#include <iostream>

int main()
{
  Scheduler s;

  s.addSemaphore(1);

  s.addTask(Task(0, 10, 3, 0));
  s.addTask(Task(1, 5, 2, 0));
  s.addTask(Task(2, 7, 1));

  for (int i = 0; i < 10; ++i)
  {
    std::cout << "tick " << i << "\n";
    s.tick();
  }

  return 0;
}