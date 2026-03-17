#include "task.hpp"

Task::Task(size_t id, size_t deadline, size_t exec_time, int resource)
        : id(id),
          deadline(deadline),
          remaining_time(exec_time),
          required_resource(resource)
{}