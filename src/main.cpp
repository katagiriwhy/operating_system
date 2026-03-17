#include "schedular.hpp"
#include <iostream>

int main()
{
    Scheduler s;

    s.addSemaphore(1);

    s.addTask(Task(0, 10, 3, 0));

    s.addInterrupt(2, [&s]() {
        size_t new_id = 100;
        int deadline = 4;
        int exec = 2;

        s.addTask(Task(new_id, deadline, exec, 0));

        std::cout << "INTERRUPT: new task added\n";
    });

    for (int i = 0; i < 10; ++i)
    {
        std::cout << "tick " << i << "\n";
        s.tick();
    }
}