#include <iostream>
#include <chrono>
#include <thread>

#include "Logic.h"
#include "cbe/util/Exception.h"

int main(int argc, char *argv[])
{
  // Use argv to indicate which binary is ran
  std::string progName = std::string(argv[0]);
  std::cout << "Started program " << progName << std::endl;

  // Try to run program
  Exercise exercise{};
  try {
    // Instead of putting the code in the main it is split into exercises
    exercise.logic();
  }
  // If something fails we should catch that exception and print it
  catch (cbe::util::Exception& e){
    std::cout << "Got exception: \"" << e.what() 
              << "\"\nType: "        << e.typeAsString() << std::endl;
    if (exercise.myCloudBackend) {
      exercise.myCloudBackend.terminate();
    }
    return 1;
  }
  // If something fails that is not an exception we get the following case
  catch (...)
  {
    std::cerr << "Error caused termination of program" << std::endl;
    if (exercise.myCloudBackend) {
      exercise.myCloudBackend.terminate();
    }
    return 9;
  }
  std::cout << "Ending program " << progName << std::endl;
  if (exercise.myCloudBackend) {
    exercise.myCloudBackend.terminate();
  }
  return 0;
}
