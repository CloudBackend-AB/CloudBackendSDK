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
  cbe::CloudBackend::LogInError logInError{};
  cloudBackend = cbe::CloudBackend::logIn(username, 
                                          password, 
                                          tenant, 
                                          client, 
                                          logInError);
  if (logInError) {
    std::cout << "Failed to login: logInError={" << logInError << '}'
              << std::endl;
    return 1;
  }
  rootContainer = cloudBackend.account().rootContainer();
  std::cout << "login: " << cloudBackend.account().username() << std::endl;

  // - - - - - - - - - - - - check for existing container - - - - - - - - - - -
  cbe::CloudBackend::QueryError queryError;
  cbe::QueryResult resultSet = rootContainer.query(queryError);
  if (queryError) {
    std::cout << "Error! " << queryError << std::endl;
    cloudBackend.terminate();
    return 2;
  }
  for (auto entry : resultSet.getItemsSnapshot()){
    if (entry.type() == cbe::ItemType::Container && 
        entry.name() == testContainer){
      std::cout << "Using existing container!" << std::endl;
      myContainer = cbe::CloudBackend::castContainer(entry);
    }
  }
  if (!myContainer){
    std::cout << "Creating a new container!" << std::endl;
    cbe::Container::CreateContainerError createContainerError;
    myContainer = 
            *rootContainer.createContainer(testContainer, createContainerError);
    if (createContainerError) {
      std::cout << "Error! " << createContainerError << std::endl;
      cloudBackend.terminate();
      return 3;
    }
  }
// - - - - - - - - - - - - - - - upload_syncExcept - - - - - - - - - - - - - - -
  constexpr const char* const uploadPath = "/tmp/";
  constexpr const char* const myObjectFileName = "Medium_C_a_message";
  const std::string           qualFile1Name = std::string{uploadPath} + 
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
  cbe::Container::UploadError uploadError;
  std::cout << "Uploading object from "
            << qualFile1Name
            << std::endl;
  object = *myContainer.upload(qualFile1Name, uploadError);
  if (uploadError) {
    std::cout << "Error! " << uploadError << std::endl;
    cloudBackend.terminate();
    return 4;
  }
  streams = object.getStreams();
  // - - - - - - - - - - - - - - - uploadStream_syncExcept - - - - - - - - - - -
  constexpr const char* const myFile2Name = "Medium_C_an_attachment";
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
  cbe::Object::UploadError uploadStreamError;
  object.uploadStream(qualFile2Name, nextStreamId, uploadStreamError);
  if (uploadStreamError) {
    std::cout << "Error! " << uploadStreamError << std::endl;
    cloudBackend.terminate();
    return 5;
  }
    std::cout << "home://"
              << myContainer.name() 
              << "/"
              << object.name() 
              << " now has two streams."
              << std::endl;
  streams = object.getStreams();
  // - - - - - - - - - - - - - - downloadStream_syncExcept - - - - - - - - - - -
  constexpr const char* const downloadPath = "/tmp/Medium_C_download_stream_";
  for (const auto& stream : streams) {
    const auto path = std::string{downloadPath} + 
                      std::to_string(stream.streamId) + "_of_";
    cbe::Object::DownloadError downloadStreamError;
    std::cout << stream.streamId
              << " to: "
              << path
              << object.name()
              << std::endl;
    auto result = *object.downloadStream(path, stream, downloadStreamError);
    if (downloadStreamError) {
      std::cout << "Error! " << downloadStreamError << std::endl;
      cloudBackend.terminate();
      return 6;
    }
  }
// - - - - - - - - - - - cloudbackend_query_syncExcept - - - - - - - - - - - - -
  std::cout << "Content of "
            << myContainer.name()
            << std::endl;
  auto queryResult = myContainer.query(objectFilter, queryError).getQueryResult();
  if (queryError) {
    std::cout << "Error! " << queryError << std::endl;
    cloudBackend.terminate();
    return 7;
  } else {
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
  std::cout << "Example Medium done" << std::endl;
  cloudBackend.terminate();
  return 0;
} // int main() 