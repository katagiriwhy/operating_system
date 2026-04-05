#include "schedular.hpp"

void Scheduler::addTask(Task task)
{
    if (tasks_.size() >= kMaxTasks)
        return;

    tasks_.push_back(task);

    if (task.state == State::READY)
        ready_queue_.push(&tasks_.back());
}

void Scheduler::addSemaphore(int count)
{
    if (semaphores_.size() >= kMaxResources)
        return;

    semaphores_.emplace_back(count);
}

std::optional<State> Scheduler::task_state(std::size_t id) const
{
    for (const auto& t : tasks_)
        if (t.id == id)
            return t.state;
    return std::nullopt;
}

Task* Scheduler::getTaskById(size_t id)
{
    for (auto& t : tasks_)
        if (t.id == id)
            return &t;

    return nullptr;
}

void Scheduler::addInterrupt(size_t time, std::function<void()> handler)
{
    interrupts_[time].push_back(handler);
}

void Scheduler::tick()
{
    if (interrupts_.count(current_time_))
    {
        for (auto& handler : interrupts_[current_time_])
        {
            handler();
        }
    }

    current_time_++;

    if (ready_queue_.empty())
        return;

    Task* task = ready_queue_.top();
    ready_queue_.pop();

    if (task->state != State::READY)
        return;

    task->state = State::RUNNING;

    if (task->required_resource != -1)
    {
        auto& sem = semaphores_[static_cast<std::size_t>(task->required_resource)];

        if (!task->resource_acquired)
        {
            if (!sem.try_acquire(task->id))
            {
                task->state = State::BLOCKED;
                return;
            }
            task->resource_acquired = true;
        }
    }

    task->remaining_time--;

    if (task->remaining_time == 0)
    {
        task->state = State::DONE;

        if (task->required_resource != -1)
        {
            auto& sem = semaphores_[static_cast<std::size_t>(task->required_resource)];

            task->resource_acquired = false;

            sem.release();

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
        }

        return;
    }

    task->state = State::READY;
    ready_queue_.push(task);
}