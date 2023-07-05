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

  cbe::Container rootContainer{cbe::DefaultCtor{}};
  cbe::Container myContainer{cbe::DefaultCtor{}};
// - - - - - - - - - - - - - - - login - - - - - - - - - - - - - - - - - - - - -
  cbe::CloudBackend cloudBackend{ cbe::DefaultCtor{} };
  cbe::CloudBackend::LogInError error{};

  cloudBackend = cbe::CloudBackend::logIn(username, password, tenant, error);
  if (!cloudBackend) {
    std::cout << "Failed to login: error={" << error << '}'
              << std::endl;
  }
  rootContainer = cloudBackend.account().rootContainer();
  const cbe::ContainerId myContainerId = rootContainer.id();
// - - - - - - - - - - - - check for existing container - - - - - - - - - - - -
  cbe::CloudBackend::QueryError queryError;
  cbe::QueryResult resultSet = rootContainer.query(queryError);
  if (queryError) {
    std::cout << "Error! " << queryError << std::endl;
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
    }
  }
// - - - - - - - - - - - - - - - upload_syncExcept - - - - - - - - - - - - - - -
  constexpr const char* const uploadPath = "/tmp/upload/";
  constexpr const char* const myObjectFileName = "myObject";
  const std::string           qualFile1Name = std::string{uploadPath} + 
                                                          myObjectFileName;
  cbe::Object object{cbe::DefaultCtor{}};
  cbe::Object::Streams streams;
  std::ofstream ofs{qualFile1Name};
  ofs << "Line 11\n"  << "Line 12\n" << "Line 13\n" << std::flush;
  ofs.close();
  cbe::Container::UploadError uploadError;
  std::cout << "Uploading!" << std::endl;
  object = *myContainer.upload(qualFile1Name, uploadError);
  streams = object.getStreams();
  if (uploadError) {
    std::cout << "Error! " << uploadError << std::endl;
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
  cbe::Object::UploadError uploadStreamError;
  std::cout << "Uploading stream!" << std::endl;
  object.uploadStream(qualFile2Name, nextStreamId, uploadStreamError);
  if (uploadStreamError) {
    std::cout << "Error! " << uploadStreamError << std::endl;
  }
// - - - - - - - - - - - - - - downloadStream_syncExcept - - - - - - - - - - - -
  constexpr const char* const downloadPath = "/tmp/download/";
  for (const auto& stream : streams) {
    const auto path =  std::string{downloadPath} + 
                                   std::to_string(stream.streamId);
    cbe::Object::DownloadError downloadStreamError;
    std::cout << "Downloading stream!" << std::endl;
    auto result = *object.downloadStream(path, stream, downloadStreamError);
    if (downloadStreamError) {
      std::cout << "Error! " << downloadStreamError << std::endl;
    }
  }
// - - - - - - - - - - - cloudbackend_query_syncExcept - - - - - - - - - - - - -
  std::cout << "Querying!" << std::endl;
  auto queryResult = cloudBackend.query(myContainerId).getQueryResult();
  if (queryError) {
    std::cout << "Error! " << queryError << std::endl;
  } else {
    cbe::QueryResult::ItemsSnapshot itemsSnapshot = 
                                                 queryResult.getItemsSnapshot();
    std::cout << "---Table---" << std::endl;
    for (auto& item : itemsSnapshot) {
      std::cout << "  " << item.name() << std::endl;
    }
    std::cout << "-----------" << std::endl;
  }
  std::cout << "Example done!" << std::endl;
  cloudBackend.terminate();
} // int main() 
