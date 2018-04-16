  #include <iostream>
  #include <thread>
  #include <mutex>
  #include <condition_variable>
  #include <unistd.h>
  #include <ctime>
  #include <algorithm>
  #include <vector>

  #include "semaphore.h"

  std::mutex m;
  #define Log(x) m.lock(); std::cout << x << std::endl; m.unlock();


  // initially barber is busy
  Binary_Semaphore barberReady(0);
  // if 1, the number of seats in the waiting room can be incremented or decremented
  Binary_Semaphore accessWRSeats(1);
  // the number of customers currently in the waiting room, ready to be served
  Primitive_Semaphore custReady(0);
 // total number of seats in the waiting room
  int numberOfFreeWRSeats = 4;

  void Customer() {
    Log("********** Customer **********");

    // lock waiting room
    Log("Customer TRYS TO LOCK the waiting room");
    accessWRSeats.wait();
    Log("Customer LOCKED the waiting room");

    Log("Customer DECREASES the number of available seats by one");
    if (numberOfFreeWRSeats > 0) {
      numberOfFreeWRSeats--;

      // customer is ready
      Log("Customer is READY");
      custReady.signal();

      Log("Customer UNLOCKS the waiting room");
      // unlock waiting room
      accessWRSeats.signal();

      // wait until barber is ready
      Log("Customer is WAITING FOR BARBER");
      barberReady.wait();

      // have haircut
      Log("Customer ...is having haircut");
    } else {
      // release the waiting room lock and leave
      Log("Customer RELEASES the waiting room lock and LEAVE");
      accessWRSeats.signal();
    }
  };

  void Barber() {
  Log("********** Barber **********");

    // run in an infinite loop
    while (true) {
      // wait for  a customer
      Log("Barber IS WAITING for customer");
      custReady.wait();
      Log("Barber HAS GOT a customer");

      // lock the waiting room
      Log("Barber TRYS TO LOCK the waiting room");
      accessWRSeats.wait();
      Log("Barber LOCKED the waiting room");

      // free one waiting room chair
      Log("Barber INCREASES the number of available seats by one");
      numberOfFreeWRSeats++;

      // now barber is ready
      Log("Barber is READY");
      barberReady.signal();

      // Barber the waiting roomwhile (true) {
      Log("Barber UNLOCKS the waiting room");
      accessWRSeats.signal();

      // cut hair
      Log("Barber ...is cutting hair");
    };
  };

  int main() {
    srand(time(NULL));
    const int numberOfTotalCustomers = rand() % 101;
    Log(numberOfTotalCustomers);
    std::vector<std::thread> threadList;
    for(unsigned int i = 0; i < numberOfTotalCustomers; i++)
    {
        threadList.push_back( std::thread( Customer ) );
    }

    std::thread barber(Barber);

    for(unsigned int i = 0; i < numberOfTotalCustomers; i++)
    {
      threadList[i].join();
    }

    barber.join();

    std::cin.get();
  }
