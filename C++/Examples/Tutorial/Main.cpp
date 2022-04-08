#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "Logic.h"

Logic* logic = new Logic();

void waitUntilFinished() {
  while (!logic->finished) {
   std::this_thread::sleep_for(std::chrono::seconds(1));
 }
}

int main(void) {
  printf("Hello! Main program started\n");
  logic->start();
  waitUntilFinished();
  delete logic;
  return 0;
}
