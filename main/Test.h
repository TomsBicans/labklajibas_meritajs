#include "HardwareSerial.h"
#include <assert.h>
#include "LinkedList.h"
#include "Queue.h"
#include "Timer.h"
#include "Measurement.h"

void test_print_entry(measurement::entry &e){
  Serial.print("time: ");
  Serial.println(e.time);
  Serial.print("atm_temperature: ");
  Serial.println(e.atm_temperature);
  Serial.print("atm_humidity: ");
  Serial.println(e.atm_humidity);
  Serial.print("atm_air_pressure: ");
  Serial.println(e.atm_air_pressure);
  Serial.print("atm_air_particle: ");
  Serial.println(e.atm_air_particle);
  Serial.print("atm_air_smoke: ");
  Serial.println(e.atm_air_smoke);
  Serial.print("atm_CO2_ammount: ");
  Serial.println(e.atm_CO2_ammount);
  Serial.print("atm_sound_pressure: ");
  Serial.println(e.atm_sound_pressure);
  Serial.print("light_intensity: ");
  Serial.println(e.light_intensity);
  Serial.print("UV_intensity: ");
  Serial.println(e.UV_intensity);
  Serial.print("quality_rating: ");
  Serial.println(e.quality_rating);
  Serial.print("user_likes: ");
  Serial.println(e.user_likes);
  delay(3000);
}

bool test_float_compare(float x, float y, float tolerance) {
    if (abs(x - y) < tolerance) {
      return true;
    } else {
      return false;
    }
  }

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
  measurement::entry e = measurement::entry{};
  Serial.print("test_entry_struct ");
  // Serial.print("time: ");
  // Serial.println(e.time);
  // Serial.print("atm_temperature: ");
  // Serial.println(e.atm_temperature);
  // Serial.print("atm_humidity: ");
  // Serial.println(e.atm_humidity);
  // Serial.print("atm_air_pressure: ");
  // Serial.println(e.atm_air_pressure);
  // Serial.print("atm_air_particle: ");
  // Serial.println(e.atm_air_particle);
  // Serial.print("atm_air_smoke: ");
  // Serial.println(e.atm_air_smoke);
  // Serial.print("atm_CO2_ammount: ");
  // Serial.println(e.atm_CO2_ammount);
  // Serial.print("atm_sound_pressure: ");
  // Serial.println(e.atm_sound_pressure);
  // Serial.print("light_intensity: ");
  // Serial.println(e.light_intensity);
  // Serial.print("UV_intensity: ");
  // Serial.println(e.UV_intensity);
  // Serial.print("quality_rating: ");
  // Serial.println(e.quality_rating);
  // Serial.print("user_likes: ");
  // Serial.println(e.user_likes);
  // delay(3000);

  // check that the fields are set to the expected default values
  assert(e.atm_temperature == measurement::NOVAL);
  assert(e.atm_air_pressure == measurement::NOVAL);
  assert(e.atm_air_particle == measurement::NOVAL);
  assert(e.atm_CO2_ammount == measurement::NOVAL);
  Serial.print(".");
  assert(e.atm_sound_pressure == measurement::NOVAL);
  assert(e.UV_intensity == measurement::NOVAL);
  assert(e.quality_rating == 100);
  // assert(e.user_likes == measurement::LIKES::UNKNOWN);
  Serial.print(".");
  return true;
}

void test_entry_struct_values() {
  // create an entry struct
  measurement::entry e = measurement::entry{};

  // set the values of some of the fields
  e.atm_temperature = 15.5;
  e.atm_air_pressure = 1013.2;
  e.atm_air_particle = 0.8;
  e.atm_CO2_ammount = 400;
  e.atm_sound_pressure = 70;

  // check that the fields have the expected values
  assert(test_float_compare(e.atm_temperature, 15.5, 1));
  assert(test_float_compare(e.atm_air_pressure, 1013.2, 1));
  assert(test_float_compare(e.atm_air_particle, 0.8, 1));
  assert(test_float_compare(e.atm_CO2_ammount, 400, 1));
  assert(test_float_compare(e.atm_sound_pressure, 70, 1));
  // Serial.print(".");

  // check that the other fields still have the default values
  assert(test_float_compare(e.UV_intensity, measurement::NOVAL, 1));
  assert(test_float_compare(e.quality_rating, 100, 1));
  // assert(e.user_likes == measurement::UNKNOWN);
  return true;
}

void test_entry_struct_addition() {
  // create an entry struct
  measurement::entry e1 = measurement::entry{};
  measurement::entry e2 = measurement::entry{};
  measurement::entry e3 = measurement::entry{};
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
  // print_entry(e3);

  // check that the fields have the expected values

  assert(test_float_compare(e3.atm_temperature, 100, 1));
  assert(test_float_compare(e3.atm_air_pressure, 100, 1));
  assert(test_float_compare(e3.atm_air_particle, 100, 1));
  assert(test_float_compare(e3.atm_CO2_ammount, 100, 1));
  assert(test_float_compare(e3.atm_sound_pressure, 100, 1));
  return true;
}

