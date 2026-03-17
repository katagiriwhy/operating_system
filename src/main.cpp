#include <iostream>
#include <queue>
#include <vector>

struct Task final {
  Task(size_t id, size_t deadline):
    this.id()
  size_t id;
  size_t deadline;
};

struct Compare final {
  bool operator()(const Task& a, const Task& b) {
    return a.deadline > b.deadline;
  }
};

int main() {
  std::cout << "HELLO\n";
  std::priority_queue<Task, std::vector<Task>, Compare> pq;
  Task t1{id :  1, deadline :  20};
  Task t2{id:  2, deadline:  5};
  Task t3{id :  3, deadline:  10};
  pq.push(t1);
  pq.push(t2);
  pq.push(t3);
  for (size_t i = 0; i < pq.size(); i++) {
    auto tmp = pq.top();
    std::cout << tmp.id << " " << tmp.deadline << '\n';
    pq.pop();
  }
}