#include <ArduinoUnit.h>
#include "LinkedList.h"


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
}
