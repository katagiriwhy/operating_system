#ifndef OPERATING_SYSTEM_TASK_HPP
#define OPERATING_SYSTEM_TASK_HPP

#include <cstddef>

enum class State {
    READY,
    RUNNING,
    BLOCKED,
    DONE
};

struct Task final {
    Task(size_t id, size_t deadline, size_t exec_time, int resource = -1);

    size_t id{};
    size_t deadline{};
    size_t remaining_time{};

    int required_resource{-1};

    State state{State::READY};
};

struct Compare final {
    bool operator()(const Task* a, const Task* b) const {
        return a->deadline > b->deadline;
    }
};

#endif