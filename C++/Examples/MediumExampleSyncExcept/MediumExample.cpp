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

int main(int argc, const char** argv) {
  static constexpr const char username[] = "zxcvzxcv";
  static constexpr const char password[] = "zxcvzxcv";
  static constexpr const char tenant[] = "cbe_githubtesters";

  static constexpr const char testContainer[] = "testContainer";
  cbe::Container myContainer{cbe::DefaultCtor{}};

// - - - - - - - - - - - - - - - login - - - - - - - - - - - - - - - - - - - - -
  cbe::CloudBackend cloudBackend{ cbe::DefaultCtor{} };
  try {
    cloudBackend = cbe::CloudBackend::logIn(username, password, tenant);
  }
  catch (cbe::CloudBackend::LogInException& e) {
    std::cout << "Failed to login: error={" << e.what() << '}'
              << std::endl;
  }
  catch (std::runtime_error& e) {
    std::cout << "Got runtime exception: {" << e.what() << '}'
              << std::endl;
  }
  catch (...) {
    std::cout << "Got other exception!"
              << std::endl;
  }

// - - - - - - - - - - - - check for existing container - - - - - - - - - - - -
  cbe::QueryResult resultSet = cloudBackend.account().rootContainer().query();
  for (auto entry : resultSet.getItemsSnapshot()){
    if (entry.type() == cbe::ItemType::Container && entry.name() == testContainer){
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
  constexpr const char* const uploadPath = "/tmp/upload/";
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
    object = myContainer.upload(qualFile1Name);
    streams = object.getStreams();
  }
  catch (const cbe::Container::UploadException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
  catch (const cbe::Object::GetStreamsException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
// - - - - - - - - - - - - - - - uploadStream_syncExcept - - - - - - - - - - - -
  constexpr const char* const myFile2Name = "myStream2";
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
    object.uploadStream(qualFile2Name, nextStreamId);
  }
  catch (const cbe::Object::UploadException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
  catch (const cbe::Object::GetStreamsException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
// - - - - - - - - - - - - downloadStream_syncExcept - - - - - - - - - - - - - -
  constexpr const char* const downloadPath = "/tmp/download/";
  for (const auto& stream : streams) {
    const auto path =  std::string{downloadPath} + 
                                   std::to_string(stream.streamId);
    try
    {
      std::cout << "Downloading stream!" << std::endl;
      auto result = object.downloadStream(path, stream);
    }
    catch (const cbe::Object::DownloadException& e)
    {
      std::cout << "Error!" << std::endl << e.what() << std::endl;
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
  catch (const cbe::CloudBackend::QueryException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
  std::cout << "Example done!" << std::endl;
  cloudBackend.terminate();
} // int main()
