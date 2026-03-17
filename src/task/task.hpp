#ifndef OPERATING_SYSTEM_TASK_HPP
#define OPERATING_SYSTEM_TASK_HPP

#include <cstddef>

struct Task final {
    Task(size_t id, size_t deadline);
    size_t id{};
    size_t deadline{};
};

struct Compare final {
    bool operator()(const Task& a, const Task& b) {
        return a.deadline > b.deadline;
    }
};

#endif
