#ifndef INCLUDE_cbe_SOLUTION_LOGIC_BODY_H_
#define INCLUDE_cbe_SOLUTION_LOGIC_BODY_H_

#include <iostream>
#include <condition_variable>
#include <memory>
#include <mutex>

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Types.h"
#include "cbe/QueryChain.h"
#include "cbe/QueryChainSync.h"
#include "cbe/QueryResult.h"

class Exercise {
public:
  // Logic is the main program 
  void logic();

  // Is used when code should terminate with a unique code
  void exitProgram(int errorCode);

  // General member variables to use in the code
  // The central object owning the login session.
  cbe::CloudBackend myCloudBackend{cbe::DefaultCtor{}};
  // Top root container - home://
  cbe::Container    rootContainer{cbe::DefaultCtor{}};

private:
  cbe::QueryResult  qResult{cbe::DefaultCtor{}};

#if 1
  // Exercise 2
  void loadContainerContents(cbe::Container container);
  void printContainerContents(cbe::QueryResult q);
  void createContainer(cbe::Container parentContainer);
  cbe::Container myContainer{cbe::DefaultCtor{}}; 

  // Exercise 3
  void loadContainerObjects(cbe::Container container);
  void printObjects(cbe::QueryResult q);
  cbe::Container selectContainer(const std::string& prompt);
  cbe::Object createObject(cbe::Container inContainer);
  cbe::Object object{cbe::DefaultCtor{}}; 
#endif

  // Generic functions
  static const bool  noBoolDefaultVal;
  static bool inquireBool(const std::string& prompt,
                          const bool&        defaultVal = noBoolDefaultVal);

  static const int noIntDefaultVal;
  static int inquireInt(const std::string& prompt,
                        const int&         defaultVal = noIntDefaultVal);

  static const std::string noStringDefaultVal;
  static std::string inquireString(const std::string& prompt,
                                   const std::string& defaultVal =
                                                            noStringDefaultVal);

  static std::string trimString(const std::string& str);

  static const char* itemTypeString(cbe::ItemType itemType);
  static void printItem(const cbe::Item& item, bool printParentId = false);

  static std::string containerName(cbe::Container  container,
                                   bool            temporary = false);
  static std::string objectName(cbe::Object  object,
                                bool         temporary = false);
};  //  class Exercise

#endif  // INCLUDE_cbe_SOLUTION_LOGIC_BODY_H_
