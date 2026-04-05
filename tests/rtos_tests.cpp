#include "schedular.hpp"
#include "semaphore/semaphore.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

static_assert(Scheduler::kMaxTasks == 32);
static_assert(Scheduler::kMaxResources == 16);

int g_failures = 0;

void check_fail(const char* file, int line, const char* expr)
{
    std::fprintf(stderr, "FAIL %s:%d (%s)\n", file, line, expr);
    ++g_failures;
}

#define REQUIRE(expr) \
    do { \
        if (!(expr)) \
            check_fail(__FILE__, __LINE__, #expr); \
    } while (0)

void test_max_tasks()
{
    Scheduler s;
    for (std::size_t i = 0; i < Scheduler::kMaxTasks; ++i)
        s.addTask(Task(i, 100 - i, 1, -1));
    REQUIRE(s.task_count() == Scheduler::kMaxTasks);

    s.addTask(Task(999, 1, 1, -1));
    REQUIRE(s.task_count() == Scheduler::kMaxTasks);
    REQUIRE(s.task_state(999) == std::nullopt);
}

void test_max_semaphores()
{
    Scheduler s;
    for (std::size_t i = 0; i < Scheduler::kMaxResources; ++i)
        s.addSemaphore(1);
    REQUIRE(s.semaphore_count() == Scheduler::kMaxResources);

    s.addSemaphore(1);
    REQUIRE(s.semaphore_count() == Scheduler::kMaxResources);
}

void test_flat_ready_queue()
{
    Scheduler s;
    s.addTask(Task(1, 50, 2, -1));
    s.addTask(Task(2, 50, 2, -1));
    REQUIRE(s.ready_queue_size() == 2);

    s.tick();
    REQUIRE(s.ready_queue_size() == 2);
}

void test_edf_earlier_deadline_first()
{
    Scheduler s;
    s.addTask(Task(1, 100, 1, -1));
    s.addTask(Task(2, 10, 1, -1));

    s.tick();
    REQUIRE(s.task_state(2) == State::DONE);
    REQUIRE(s.task_state(1) == State::READY);

    s.tick();
    REQUIRE(s.task_state(1) == State::DONE);
}

void test_edf_tie_break_by_id()
{
    Scheduler s;
    s.addTask(Task(5, 20, 1, -1));
    s.addTask(Task(3, 20, 1, -1));

    s.tick();
    REQUIRE(s.task_state(3) == State::DONE);
    REQUIRE(s.task_state(5) == State::READY);
}

void test_semaphore_try_acquire_blocks_second()
{
    Semaphore sem(1);
    REQUIRE(sem.try_acquire(10));
    REQUIRE(!sem.try_acquire(20));
    REQUIRE(sem.waiting().size() == 1);
}

void test_semaphore_release_then_retry_acquire()
{
    Semaphore sem(1);
    REQUIRE(sem.try_acquire(1));
    REQUIRE(!sem.try_acquire(2));
    REQUIRE(sem.waiting().size() == 1);

    sem.release();
    REQUIRE(sem.waiting().front() == 2);
    sem.waiting().pop();

    REQUIRE(sem.try_acquire(2));
    REQUIRE(sem.waiting().empty());
}

void test_scheduler_semaphore_serializes_tasks()
{
    Scheduler s;
    s.addSemaphore(1);
    s.addTask(Task(1, 10, 1, 0));
    s.addTask(Task(2, 10, 1, 0));

    s.tick();
    REQUIRE(s.task_state(1) == State::DONE);
    REQUIRE(s.task_state(2) == State::READY);

    s.tick();
    REQUIRE(s.task_state(2) == State::DONE);
}

void test_scheduler_task_blocked_on_semaphore()
{
    Scheduler s;
    s.addSemaphore(1);
    s.addTask(Task(1, 100, 3, 0));
    s.addInterrupt(1, [&s]() { s.addTask(Task(2, 1, 1, 0)); });

    s.tick();
    REQUIRE(s.task_state(1) == State::READY);

    s.tick();
    REQUIRE(s.task_state(2) == State::BLOCKED);
    REQUIRE(s.task_state(1) == State::READY);

    s.tick();
    REQUIRE(s.task_state(1) == State::READY);

    s.tick();
    REQUIRE(s.task_state(1) == State::DONE);
    REQUIRE(s.task_state(2) == State::READY);

    s.tick();
    REQUIRE(s.task_state(2) == State::DONE);
}

void test_interrupt_fires_at_time()
{
    Scheduler s;
    int fired = 0;
    s.addInterrupt(2, [&fired]() { ++fired; });

    s.tick();
    REQUIRE(s.current_time() == 1);
    REQUIRE(fired == 0);

    s.tick();
    REQUIRE(s.current_time() == 2);
    REQUIRE(fired == 0);

    s.tick();
    REQUIRE(s.current_time() == 3);
    REQUIRE(fired == 1);
}

void test_interrupt_local_variables()
{
    Scheduler s;
    bool saw_local_ok = false;
    s.addInterrupt(0, [&saw_local_ok]() {
        int local = 7;
        std::string buf = "ok";
        saw_local_ok = (local == 7 && buf == "ok");
    });

    s.tick();
    REQUIRE(saw_local_ok);
}

void test_no_event_subsystem_documented()
{
    Scheduler s;
    (void)s;
}

}

int main()
{
    test_max_tasks();
    test_max_semaphores();
    test_flat_ready_queue();
    test_edf_earlier_deadline_first();
    test_edf_tie_break_by_id();
    test_semaphore_try_acquire_blocks_second();
    test_semaphore_release_then_retry_acquire();
    test_scheduler_semaphore_serializes_tasks();
    test_scheduler_task_blocked_on_semaphore();
    test_interrupt_fires_at_time();
    test_interrupt_local_variables();
    test_no_event_subsystem_documented();

    if (g_failures != 0)
    {
        std::fprintf(stderr, "%d test(s) failed\n", g_failures);
        return EXIT_FAILURE;
    }

    std::puts("All tests passed.");
    return EXIT_SUCCESS;
}
