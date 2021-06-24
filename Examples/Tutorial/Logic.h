
#ifndef INCLUDE_CBE_SOLUTIONLOGICBEODY_H_
#define INCLUDE_CBE_SOLUTIONLOGICBEODY_H_
#include <iostream>
#include <mutex>
#include "CBE/CloudBackend.h"

class Logic {
 public:
  void start();
  void logic();
  void programFinished();

  bool finished;
  CBE::ContainerPtr rootContainer;

  void saveQueryResultContinue(CBE::QueryResultPtr qR);


 private:
  int logicInstances = 0;
  int step = 1;
  std::recursive_mutex logicMutex;
  CBE::CloudBackendPtr cloudBackend; 
  CBE::QueryResultPtr qResult;

#if 1
  // Exercise 2
  void loadContainerContents(CBE::ContainerPtr container);
  void printContainerContents(CBE::QueryResultPtr q);
  CBE::ContainerPtr createContainer(CBE::ContainerPtr container);
  CBE::ContainerPtr container;

  // Exercise 3
  void printObjects(CBE::QueryResultPtr q);
  CBE::ContainerPtr selectContainer(const std::string& prompt);
  CBE::ObjectPtr createObject(CBE::ContainerPtr inContainer);
  CBE::ObjectPtr object;
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

  static const char* itemTypeString(CBE::item_t itemType);
  static void printItem(const CBE::Item& item, bool printParentId = false);

  static std::string containerName(CBE::ContainerPtr  container,
                                   bool               temporary = false);
  static std::string objectName(CBE::ObjectPtr  object,
                                bool            temporary = false);
};

#endif
