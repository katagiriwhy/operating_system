#include <iostream>
#include <queue>
#include <vector>

#include "task.hpp"

int main() {
  std::priority_queue<Task, std::vector<Task>, Compare> pq;
  Task t1(1,20);
  Task t2(2,5);
  Task t3(3,10);
  pq.push(t1);
  pq.push(t2);
  pq.push(t3);
  const size_t& size = pq.size();
  for (size_t i = 0; i < size; i++) {
    auto tmp = pq.top();
    std::cout << tmp.id << " " << tmp.deadline << '\n';
    pq.pop();
  }
}