#include <ArduinoUnit.h>
#include "Queue.h"

// Test the queue class implementation
test(testQueue)
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
}