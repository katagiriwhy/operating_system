#include "schedular.hpp"

void Scheduler::addTask(Task task)
{
    if (tasks_.size() >= 32)
        return;

    tasks_.push_back(task);

    if (task.state == State::READY)
        ready_queue_.push(&tasks_.back());
}

void Scheduler::addSemaphore(int count)
{
    if (semaphores_.size() >= 16)
        return;

    semaphores_.emplace_back(count);
}

Task* Scheduler::getTaskById(size_t id)
{
    for (auto& t : tasks_)
        if (t.id == id)
            return &t;

    return nullptr;
}

void Scheduler::tick()
{
    if (ready_queue_.empty())
        return;

    Task* task = ready_queue_.top();
    ready_queue_.pop();

    if (task->state != State::READY)
        return;

    task->state = State::RUNNING;

    if (task->required_resource != -1)
    {
        auto& sem = semaphores_[task->required_resource];

        if (!sem.try_acquire(task->id))
        {
            task->state = State::BLOCKED;
            return;
        }
    }

    task->remaining_time--;

    if (task->remaining_time == 0)
    {
        task->state = State::DONE;

        if (task->required_resource != -1)
        {
            auto& sem = semaphores_[task->required_resource];

            if (!sem.waiting().empty())
            {
                size_t next_id = sem.waiting().front();
                sem.waiting().pop();

                Task* next = getTaskById(next_id);
                if (next)
                {
                    next->state = State::READY;
                    ready_queue_.push(next);
                }
            }
            else
            {
                sem.release();
            }
        }

        return;
    }

    task->state = State::READY;
    ready_queue_.push(task);
}