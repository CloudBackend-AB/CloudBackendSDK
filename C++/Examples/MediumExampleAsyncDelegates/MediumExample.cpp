#include <algorithm>
#include <fstream>
#include <iostream>

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Container.h"
#include "cbe/Stream.h"
#include "cbe/Object.h"
#include "cbe/QueryChain.h"

#include "cbe/util/ErrorInfo.h"

#include "MyDelegates.cpp"
#include "../user_credentials.cpp"

int main(int argc, const char** argv) {
  static constexpr const char testObject[] = "testObject";
  static constexpr const char testContainer[] = "testContainer";

  cbe::Container rootContainer{cbe::DefaultCtor{}};
  cbe::Container myContainer{cbe::DefaultCtor{}};
// - - - - - - - - - - - - - - - login_async - - - - - - - - - - - - - - - - - -
  std::shared_ptr<MyLogInDelegate> myLogInDelegate =
                                            std::make_shared<MyLogInDelegate>();
  cbe::CloudBackend cloudBackend = cbe::CloudBackend::logIn(username,
                                                            password,
                                                            tenant,
                                                            client,
                                                            myLogInDelegate);
  myLogInDelegate->waitForRsp();

  if (myLogInDelegate->error) {
    std::cout << "Failed to login: error={" << myLogInDelegate->errorInfo << '}'
              << std::endl;
    cloudBackend.terminate();
    return 1;
  }
  cloudBackend = myLogInDelegate->cloudBackend;
  rootContainer = cloudBackend.account().rootContainer();
  myLogInDelegate.reset();
// - - - - - - - - - - - - check for existing container - - - - - - - - - - - -
  std::shared_ptr<MyQueryDelegate> myQueryDelegate =
                                            std::make_shared<MyQueryDelegate>();
  rootContainer.query(myQueryDelegate).getQueryResult();
  myQueryDelegate->waitForRsp();
  cbe::QueryResult resultSet = myQueryDelegate->queryResult;
  for (auto entry : resultSet.getItemsSnapshot()){
    if (entry.type() == cbe::ItemType::Container && entry.name() == testContainer){
      std::cout << "Using existing container!" << std::endl;
      myContainer = cbe::CloudBackend::castContainer(entry);
    }
  }
  if (!myContainer){
    std::cout << "Creating a new container!" << std::endl;
    std::shared_ptr<MyCreateContainerDelegate> myCreateContainerDelegate =
                                  std::make_shared<MyCreateContainerDelegate>();
    rootContainer.createContainer(testContainer, myCreateContainerDelegate);
    myCreateContainerDelegate->waitForRsp();
    myContainer = myCreateContainerDelegate->container;
  }
// - - - - - - - - - - - - - - -upload_async - - - - - - - - - - - - - - - - - -
  constexpr const char* const uploadPath = "/tmp/";
  constexpr const char* const myObjectFileName = "myObject";
  const std::string           qualFile1Name = std::string{uploadPath} +
                                                          myObjectFileName;
  cbe::Object object{cbe::DefaultCtor{}};
  cbe::Object::Streams streams;
  std::ofstream ofs{qualFile1Name};
  ofs << "Line 11\n"  << "Line 12\n" << "Line 13\n" << std::flush;
  ofs.close();
  std::cout << "Created file " << qualFile1Name << std::endl;
  std::cout << "Uploading object!" << std::endl;
  std::cout << "from: " << qualFile1Name << std::endl;

  std::shared_ptr<MyUploadDelegate> myUploadDelegate =
                                           std::make_shared<MyUploadDelegate>();
  myContainer.upload(qualFile1Name, myUploadDelegate);
  object = myUploadDelegate->waitForRsp();

  if (!object) {
    std::cout << "Error! " << myUploadDelegate->errorInfo << std::endl;
  }
  std::shared_ptr<MyGetStreamsDelegate> getStreamsDelegate =
                                       std::make_shared<MyGetStreamsDelegate>();
  object.getStreams(getStreamsDelegate);
  getStreamsDelegate->waitForRsp();
  if (getStreamsDelegate->errorInfo) {
    std::cout << "Error! " << myLogInDelegate->errorInfo << std::endl;
  }
  streams = *getStreamsDelegate->streams;
// - - - - - - - - - - - - - - - uploadStream_async - - - - - - - - - - - - - -
std::cout << "Uploading stream!" << std::endl;
  constexpr const char* const myFile2Name = "myObject_Stream2";
  const std::string           qualFile2Name = std::string{uploadPath} +
                                                          myFile2Name;

  std::ofstream ofs2{qualFile2Name};
  ofs2 << "Line 21\n"  << "Line 22\n" << "Line 23\n" << "Line 24\n"
       << std::flush;
  ofs2.close();

  const auto nextStreamId =
              std::max_element(std::begin(streams), std::end(streams),
                               [](const cbe::Stream& stream1,
                                  const cbe::Stream& stream2) {
                                 return stream1.streamId < stream2.streamId;
                               })->streamId + 1;

  std::cout << "from: " << qualFile2Name << std::endl;
  std::shared_ptr<MyUploadStreamDelegate> myUploadStreamDelegate =
                                  std::make_shared<MyUploadStreamDelegate>();
  object.uploadStream(qualFile2Name, nextStreamId, myUploadStreamDelegate);
  myUploadStreamDelegate->waitForRsp();
  if (myUploadStreamDelegate->errorInfo) {
    std::cout << "Error! " << myLogInDelegate->errorInfo << std::endl;
  }
  object.getStreams(getStreamsDelegate);
  getStreamsDelegate->waitForRsp();
  if (getStreamsDelegate->errorInfo) {
    std::cout << "Error! " << myLogInDelegate->errorInfo << std::endl;
  }
  streams = *getStreamsDelegate->streams;
// - - - - - - - - - - - - - - - downloadStream_async - - - - - - - - - - - - - 
  std::cout << "Downloading stream!" << std::endl;
  std::shared_ptr<MyDownloadDelegate> myDownloadDelegate =
                                         std::make_shared<MyDownloadDelegate>();
  constexpr const char* const downloadPath = "/tmp/download_stream_";

  for (const auto& stream : streams) {
    const auto path = std::string{downloadPath} +
                                  std::to_string(stream.streamId) + "_";
    std::cout << "to: " << path << std::endl;
    object.downloadStream(path, stream, myDownloadDelegate);
    myDownloadDelegate->waitForRsp();
    if (!myDownloadDelegate) {
      std::cout << "Error! " << myDownloadDelegate->errorInfo << std::endl;
    }
  }
// - - - - - - - - - - - - - - cloudbackend_query_async - - - - - - - - - - - -
  std::cout << "Querying!" << std::endl;
  const cbe::ContainerId myContainerId =
                                    cloudBackend.account().rootContainer().id();
  cloudBackend.query(myContainerId, myQueryDelegate);
  myQueryDelegate->waitForRsp();
  if (myQueryDelegate->errorInfo) {
    std::cout << "Error! " << myQueryDelegate->errorInfo << std::endl;
  } else {
    cbe::QueryResult::ItemsSnapshot itemsSnapshot =
                                myQueryDelegate->queryResult.getItemsSnapshot();
    std::cout << "---Table---" << std::endl;
    for (auto& item : itemsSnapshot) {
      std::cout << "  " << item.name() << std::endl;
    }
    std::cout << "-----------" << std::endl;
  }

  std::cout << "Example done!" << std::endl;
  cloudBackend.terminate();
} // int main() 