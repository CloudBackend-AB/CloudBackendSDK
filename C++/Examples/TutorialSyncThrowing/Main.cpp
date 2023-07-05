#include <iostream>
#include <chrono>
#include <thread>

#include "Logic.h"

int main(int argc, char *argv[])
{
  // Use argv to indicate which binary is ran
  std::string progName = std::string(argv[0]);
  std::cout << "Started program " << progName << std::endl;

  // Try to run program
  try {
    // Instead of putting the code in the main it is split into exercises
    Exercise exercise{};
    exercise.logic();
  }
  // If something fails we should catch that exception and print it
  catch (std::exception& e){
    std::cout << "Got exception: \"" << e.what() << "\"" << std::endl;
  }
  // If something fails that is not an exception we get the following case
  catch (...)
  {
    std::cerr << "Error caused termination of program" << std::endl;
  }
  std::cout << "Ending program " << progName << std::endl;
  return 0;
}
