#include <algorithm>
#include <fstream>
#include <iostream>

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Container.h"
#include "cbe/Stream.h"
#include "cbe/Object.h"
#include "cbe/QueryChainSync.h"
#include "cbe/delegate/DownloadSuccess.h"

#include "cbe/util/Exception.h"
#include "cbe/util/ErrorInfo.h"

#include "../user_credentials.cpp"

int main(int argc, const char** argv) {
  static constexpr const char testContainer[] = "testContainer";
  cbe::Container myContainer{cbe::DefaultCtor{}};

// - - - - - - - - - - - - - - - login - - - - - - - - - - - - - - - - - - - - -
  cbe::CloudBackend cloudBackend{ cbe::DefaultCtor{} };
  try {
    cloudBackend = cbe::CloudBackend::logIn(username, password, tenant, client);
  }
  // Catching specific LogInException
  catch (cbe::CloudBackend::LogInException& e) {
    std::cout << "Failed to login: error={" << e.what() << '}'
              << std::endl;
    return 1;
  }
  catch (...) {
    std::cout << "Got other exception!"
              << std::endl;
    return 9;
  }

// - - - - - - - - - - - - check for existing container - - - - - - - - - - - -
  cbe::QueryResult resultSet = cloudBackend.account().rootContainer().query();
  for (auto entry : resultSet.getItemsSnapshot()){
    if (entry.type() == cbe::ItemType::Container && 
        entry.name() == testContainer){
      std::cout << "Using existing container!" << std::endl;
      myContainer = cbe::CloudBackend::castContainer(entry);
    }
  }
  if (!myContainer){
    std::cout << "Creating a new container!" << std::endl;
    myContainer = 
          cloudBackend.account().rootContainer().createContainer(testContainer);
  }
// - - - - - - - - - - - - - - - upload_syncExcept - - - - - - - - - - - - - - -
  constexpr const char* const uploadPath = "/tmp/";
  constexpr const char* const myObjectFileName = "myObject";
  const std::string           qualFile1Name = 
                                     std::string{uploadPath} + myObjectFileName;
  cbe::Object object{cbe::DefaultCtor{}};
  cbe::Object::Streams streams;
  std::ofstream ofs{qualFile1Name};
  ofs << "Line 11\n"  << "Line 12\n" << "Line 13\n" << std::flush;
  ofs.close();
  try
  {
    std::cout << "Uploading file!" << std::endl;
    std::cout << "from: " << qualFile1Name << std::endl;
    object = myContainer.upload(qualFile1Name);
    streams = object.getStreams();
  }
  // Using generic Exception for convenience
  catch (const cbe::util::Exception& e)
  {
    // typeAsString is used to print what type of exception was thrown
    std::cout << "Error!\n" << e.what() 
              << "\nType: " << e.typeAsString() << std::endl;
    cloudBackend.terminate();
    return 2;
  }
// - - - - - - - - - - - - - - - uploadStream_syncExcept - - - - - - - - - - - -
  constexpr const char* const myFile2Name = "myObject_Stream2";
  const std::string qualFile2Name = std::string{uploadPath} + myFile2Name;
  std::ofstream ofs3{qualFile2Name};
  ofs3 << "Line 21\n"  << "Line 22\n" << "Line 23\n" << "Line 24\n"
       << std::flush;
  ofs3.close();

  const auto nextStreamId = std::max_element(std::begin(streams), 
                                             std::end(streams),
                                  [](const cbe::Stream& stream1,
                                     const cbe::Stream& stream2) {
                                    return stream1.streamId < stream2.streamId;
                                  })->streamId + 1;

  try
  {
    std::cout << "Uploading stream!" << std::endl;
    std::cout << "from: " << qualFile2Name << std::endl;
    object.uploadStream(qualFile2Name, nextStreamId);
    streams = object.getStreams();
  }
  catch (const cbe::util::Exception& e)
  {
    std::cout << "Error!\n" << e.what() 
              << "\nType: " << e.typeAsString() << std::endl;
    cloudBackend.terminate();
    return 3;
  }
// - - - - - - - - - - - - downloadStream_syncExcept - - - - - - - - - - - - - -
  constexpr const char* const downloadPath = "/tmp/download_stream_";
  for (const auto& stream : streams) {
    const auto path =  std::string{downloadPath} + 
                                   std::to_string(stream.streamId) + "_";
    try
    {
      std::cout << "Downloading stream!" << std::endl;
      std::cout << "to: " << path << std::endl;
      auto result = object.downloadStream(path, stream);
    }
    catch (const cbe::util::Exception& e)
    {
      std::cout << "Error!\n" << e.what() 
                << "\nType: " << e.typeAsString() << std::endl;
      cloudBackend.terminate();
      return 4;
    }
  }
// - - - - - - - - - - - cloudbackend_query_syncExcept - - - - - - - - - - - - -
  try
  {
    std::cout << "Querying!" << std::endl;
    const cbe::ContainerId myContainerId = 
                                    cloudBackend.account().rootContainer().id();
    auto queryResult = cloudBackend.query(myContainerId).getQueryResult();
    cbe::QueryResult::ItemsSnapshot itemsSnapshot = 
                                                 queryResult.getItemsSnapshot();
    std::cout << "---Table---" << std::endl;
    for (auto& item : itemsSnapshot) {
      std::cout << "  " << item.name() << std::endl;
    }
    std::cout << "-----------" << std::endl;

  }
  catch (const cbe::util::Exception& e)
  {
    std::cout << "Error!\n" << e.what() 
              << "\nType: " << e.typeAsString() << std::endl;
    cloudBackend.terminate();
    return 5;
  }
  std::cout << "Example done!" << std::endl;
  cloudBackend.terminate();
  return 0;
} // int main()