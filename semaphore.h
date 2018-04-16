// Primitive Semaphore
class Primitive_Semaphore {
private:
std::mutex mtx;
std::condition_variable conditionVariable;
// count is shared and should be protected
mutable int count;

public:
explicit Primitive_Semaphore(int count)
: count(count) {};

void wait() {
  std::unique_lock<std::mutex> locker(mtx);
  this->count--;
  if (this->count < 0) {
    // block the process
    conditionVariable.wait(locker);
  }
  // ...
}

void signal() {
  std::unique_lock<std::mutex> locker(mtx);
  this->count++;
  if (this->count <= 0) {
    // unblock a process
    conditionVariable.notify_one();
  }
  // ...
  }
};

// Binary Semaphore
 class Binary_Semaphore {
 private:
   std::mutex mtx;
   std::condition_variable conditionVariable;
   int count; // count of blocked processes
   enum Value {
     zero = 0, one = 1
   };
   Value value;

 public:
  explicit Binary_Semaphore(int x)
   : count(0) {
     if (x == 1) {
       value = one;
     } else if (x == 0) {
       value = zero;
     }
   };

   void wait() {
     std::unique_lock<std::mutex> locker(mtx);
     if (this->value == one) {
       this->value = zero;
     } else {
       this->count++;
       // block a process
       conditionVariable.wait(locker);
     }
   };

   void signal() {
     std::unique_lock<std::mutex> locker(mtx);
     if ( count <= 0) {
       this->value = one;
     } else {
        this->count--;
       // unblock a process
       conditionVariable.notify_one();
     }
   };
};
