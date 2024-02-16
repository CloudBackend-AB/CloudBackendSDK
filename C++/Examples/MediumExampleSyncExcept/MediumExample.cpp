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
  static constexpr const char testContainer[] = "z-MediumExample";

  cbe::Container rootContainer{cbe::DefaultCtor{}};
  cbe::Container myContainer{cbe::DefaultCtor{}};
  cbe::Filter objectFilter;
  objectFilter.setDataType(cbe::ItemType::Object);

  // - - - - - - - - - - - - - - - login - - - - - - - - - - - - - - - - - - - -
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
  rootContainer = cloudBackend.account().rootContainer();
  std::cout << "login: " << cloudBackend.account().username() << std::endl;

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
    myContainer = rootContainer.createContainer(testContainer);
  }
  // - - - - - - - - - - - - - - - upload_syncExcept - - - - - - - - - - - - - -
  constexpr const char* const uploadPath = "/tmp/";
  constexpr const char* const myObjectFileName = "Medium_B_a_message";
  const std::string qualFile1Name = std::string{uploadPath} + 
                                                myObjectFileName;
  cbe::Object anObject{cbe::DefaultCtor{}};
  cbe::Object object{cbe::DefaultCtor{}};
  cbe::Object::Streams streams;
  std::cout << "Create local file " << qualFile1Name << std::endl;
  std::ofstream ofs{qualFile1Name};
  ofs << "Line a\n"
      << "Line b\n"
      << "Line c\n"
      << std::flush;
  ofs.close();
  try
  {
  std::cout << "Uploading object from "
            << qualFile1Name
            << std::endl;
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
  // - - - - - - - - - - - - - - - uploadStream_syncExcept - - - - - - - - - - -
  constexpr const char* const myFile2Name = "Medium_B_an_attachment";
  const std::string qualFile2Name = std::string{uploadPath} + myFile2Name;

  std::ofstream ofs3{qualFile2Name};
  ofs3  << "line 21\n"
        << "line 22\n"
        << "line 23\n"
        << "line 24\n"
        << std::flush;
  ofs3.close();
  std::cout << "Uploading additional stream from: " 
            << qualFile2Name 
            << std::endl;
  const auto nextStreamId =
              std::max_element(std::begin(streams), std::end(streams),
                               [](const cbe::Stream& stream1,
                                  const cbe::Stream& stream2) {
                                 return stream1.streamId < stream2.streamId;
                              })->streamId + 1;
  try
  {
    object.uploadStream(qualFile2Name, nextStreamId);
    std::cout << "home://"
              << myContainer.name() 
              << "/"
              << object.name() 
              << " now has two streams."
              << std::endl;
    streams = object.getStreams();
  }
  catch (const cbe::util::Exception& e)
  {
    std::cout << "Error!\n" << e.what() 
              << "\nType: " << e.typeAsString() << std::endl;
    cloudBackend.terminate();
    return 3;
  }
  // - - - - - - - - - - - - downloadStream_syncExcept - - - - - - - - - - - - -
  std::cout << "Downloading stream:" << std::endl;
  constexpr const char* const downloadPath = "/tmp/Medium_B_download_stream_";
  for (const auto& stream : streams) {
    const auto path =  std::string{downloadPath} + 
                       std::to_string(stream.streamId) + "_of_";
    try
    {
    std::cout << stream.streamId
              << " to: "
              << path
              << object.name()
              << std::endl;
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
  // - - - - - - - - - - - cloudbackend_query_syncExcept - - - - - - - - - - - -
  try
  {
    std::cout << "Content of "
              << myContainer.name()
              << std::endl;
    auto queryResult = myContainer.query(objectFilter).getQueryResult();
    cbe::QueryResult::ItemsSnapshot itemsSnapshot = 
                                          queryResult.getItemsSnapshot();
    std::cout << "--------- table content -----------" << std::endl;
    for (auto& item : itemsSnapshot) {
      anObject = cloudBackend.castObject(item);
      std::cout << " sum "
                << anObject.length()
                << " Bytes\t"
                << anObject.name()
                << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
  }
  catch (const cbe::util::Exception& e)
  {
    std::cout << "Error!\n" << e.what() 
              << "\nType: " << e.typeAsString() << std::endl;
    cloudBackend.terminate();
    return 5;
  }
  std::cout << "Example Medium done" << std::endl;
  cloudBackend.terminate();
  return 0;
} // int main()