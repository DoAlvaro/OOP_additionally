#include <array>
#include <iostream>
class QueueOverflowException : public std::runtime_error {
public:
   QueueOverflowException() : std::runtime_error("Queue overflow") {}
};

class QueueUnderflowException : public std::runtime_error {
public:
   QueueUnderflowException() : std::runtime_error("Queue underflow") {}
};

template <std::size_t N, typename T, bool ForceMode = false>
class LimitedQueue {
private:
   std::array<T, N> data;
   std::size_t start = 0;
   std::size_t end = 0;

public:
   LimitedQueue() = default;
   LimitedQueue(std::initializer_list<T> init) {
       std::copy(init.begin(), init.end(), data.begin());
       end = init.size();
   }

   void push_back(const T& value) {
       if (full()) {
           if (ForceMode) {
               pop_front();
           } else {
               throw QueueOverflowException();
           }
       }
       data[end++] = value;
   }

   T pop_front() {
       if (empty()) {
           throw QueueUnderflowException();
       }
       T value = data[start++];
       if (start == N) {
           start = 0;
       }
       return value;
   }

   T& front() {
       if (empty()) {
           throw QueueUnderflowException();
       }
       return data[start];
   }

   bool empty() const {
       return start == end;
   }

   bool full() const {
       return (end + 1) % (N+1) == start;
   }

   void clear() {
       start = 0;
       end = 0;
   }

   std::size_t size() const {
       if (end >= start) {
           return end - start;
       } else {
           return N - start + end;
       }
   }
};

int main() {
   LimitedQueue<3, int> queue;

   queue.push_back(1);
   queue.push_back(2);
   queue.push_back(3);
   // Выводим первый элемент очереди
   std::cout << "First: " << queue.front() << std::endl;  // First: 1

   // Удаляем первый элемент из очереди
   queue.pop_front();

   // Выводим первый элемент очереди после удаления
   std::cout << "First after pop: " << queue.front() << std::endl;  // First after pop: 2

   // Проверяем, что очередь не пуста
   if (!queue.empty()) {
       std::cout << "Queue is not empty!" << std::endl;
   }

   // Выводим текущий размер очереди
   std::cout << "Size: " << queue.size() << std::endl;  // Size: 2

   // Очищаем очередь
   queue.clear();

   // Проверяем, что очередь пуста
   if (queue.empty()) {
       std::cout << "Queue is empty after clear!" << std::endl;
   }

   return 0;
}