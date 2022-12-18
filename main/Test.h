#include <assert.h>
#include "LinkedList.h"
#include "Queue.h"
#include "Timer.h"
#include "Measurement.h"

// Test the queue class implementation
bool testQueue()
{
  // Create a queue with a capacity of 10 elements
  ArduinoQueue<int> queue(10);
  
  // Test the isEmpty() method
  assert(queue.isEmpty() == true);
  
  // Test the enqueue() method
  queue.enqueue(1);
  queue.enqueue(2);
  queue.enqueue(3);
  queue.enqueue(4);
  queue.enqueue(5);
  assert(queue.isEmpty() == false);

  // Test the dequeue() method
  assert(queue.dequeue() == 1);
  assert(queue.dequeue() == 2);
  assert(queue.dequeue() == 3);
  assert(queue.dequeue() == 4);
  assert(queue.dequeue() == 5);
  assert(queue.isEmpty() == true);
  // Test the isFull() method
  queue.enqueue(1);
  queue.enqueue(2);
  queue.enqueue(3);
  queue.enqueue(4);
  queue.enqueue(5);
  queue.enqueue(6);
  queue.enqueue(7);
  queue.enqueue(8);
  queue.enqueue(9);
  queue.enqueue(10);
  assert(queue.isFull() == true);

  // Test the removeDuplicates() method
  queue.enqueue(1);
  queue.enqueue(2);
  queue.enqueue(1);
  queue.enqueue(3);
  queue.enqueue(4);
  queue.enqueue(5);
  queue.enqueue(1);
  queue.enqueue(6);
  queue.enqueue(2);
  queue.enqueue(1);
  // queue.removeDuplicates();
  // assert(queue.dequeue() == 2);
  // assert(queue.dequeue() == 3);
  // assert(queue.dequeue() == 4);
  // assert(queue.dequeue() == 5);
  // assert(queue.dequeue() == 6);
  // assert(queue.dequeue() == 1);
  
  // assert(queue.dequeue() == 1);
  // assert(queue.dequeue() == 2);
  // assert(queue.dequeue() == 3);
  // assert(queue.dequeue() == 4);
  // assert(queue.dequeue() == 5);
  // assert(queue.dequeue() == 6);
  // assert(queue.isEmpty() == true);
  assert(queue.isEmpty() == false);
  return true;
}

bool testLinkedList(){
    // Create a LinkedList of ints
  LinkedList<int> list;

  // Test that the list is initially empty
  assert(list.isEmpty() == true);

  // Test adding elements to the list
  list.add(1);
  assert(list.getSize() == 1);
  list.add(2);
  assert(list.getSize() == 2);

  // Test adding elements to the front of the list
  list.addFirst(0);
  assert(list.getFirst() == 0);
  assert(list.getSize() == 3);

  // Test adding elements to the end of the list
  list.addLast(3);
  assert(list.getLast() == 3);
  assert(list.getSize() == 4);

  // Test getting the first and last elements of the list
  assert(list.getFirst() == 0);
  assert(list.getLast() == 3);

  // Test removing elements from the list
  list.removeFirst();
  assert(list.getFirst() == 1);
  assert(list.getSize() == 3);
  list.removeLast();
  assert(list.getLast() == 2);
  assert(list.getSize() == 2);

  // Test clearing the list
  list.clear();
  assert(list.isEmpty() == true);
  return true;
}

void test_entry_struct() {
  // create an entry struct
  measurement::entry e{};

  // check that the fields are set to the expected default values
  assert(e.atm_temperature == measurement::NOVAL);
  assert(e.atm_air_pressure == measurement::NOVAL);
  assert(e.atm_air_particle == measurement::NOVAL);
  assert(e.atm_CO2_ammount == measurement::NOVAL);
  assert(e.atm_sound_pressure == measurement::NOVAL);
  assert(e.UV_intensity == measurement::NOVAL);
  assert(e.quality_rating == 100);
  assert(e.user_likes == measurement::LIKES::UNKNOWN);
  return true;
}

void test_entry_struct_values() {
  // create an entry struct
  measurement::entry e{};

  // set the values of some of the fields
  e.atm_temperature = 15.5;
  e.atm_air_pressure = 1013.2;
  e.atm_air_particle = 0.8;
  e.atm_CO2_ammount = 400;
  e.atm_sound_pressure = 70;

  // check that the fields have the expected values
  assert(e.atm_temperature == 15.5);
  assert(e.atm_air_pressure == 1013.2);
  assert(e.atm_air_particle == 0.8);
  assert(e.atm_CO2_ammount == 400);
  assert(e.atm_sound_pressure == 70);

  // check that the other fields still have the default values
  assert(e.UV_intensity == measurement::NOVAL);
  assert(e.quality_rating == 100);
  assert(e.user_likes == measurement::LIKES::UNKNOWN);
  return true;
}

void test_entry_struct_addition() {
  // create an entry struct
  measurement::entry e1{};
  measurement::entry e2{};
  measurement::entry e3{};
  // set the values of some of the fields
  e1.atm_temperature = 50;
  e1.atm_air_pressure = 50;
  e1.atm_air_particle = 50;
  e1.atm_CO2_ammount = 50;
  e1.atm_sound_pressure = 50;

  e2.atm_temperature = 50;
  e2.atm_air_pressure = 50;
  e2.atm_air_particle = 50;
  e2.atm_CO2_ammount = 50;
  e2.atm_sound_pressure = 50;

  e3 = e1 + e2;

  // check that the fields have the expected values
  assert(e1.atm_temperature == 100);
  assert(e1.atm_air_pressure == 100);
  assert(e1.atm_air_particle == 100);
  assert(e1.atm_CO2_ammount == 100);
  assert(e1.atm_sound_pressure == 100);
  return true;
}

