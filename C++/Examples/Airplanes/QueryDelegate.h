#ifndef QueryDelegate_h__
#define QueryDelegate_h__

#include "CBE/Protocols/ItemEventProtocol.h"

#include <condition_variable>
#include <mutex>
#include <sstream>

struct QueryDelegate : public CBE::ItemEventProtocol {
  CBE::QueryResultPtr     queryResult{};
  std::string             errorInfo{};
  bool                    finished{};

  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  void onQueryLoaded(CBE::QueryResultPtr queryResult) final {
    {
      std::lock_guard<std::mutex> lock(mutex);
      this->queryResult = queryResult;
      finished = true;
    }
    conditionVariable.notify_one();
  }
  void onLoadError(CBE::Filter filter, uint32_t operation, uint32_t code,
                   std::string reason, std::string message) final {
    std::ostringstream oss;
    oss << "LoadError: code=" << code << ", reason=\"" << reason
        << "\", message=\"" << message << "\"";
    {
      std::lock_guard<std::mutex> lock(mutex);
      this->errorInfo = oss.str();
      finished = true;
    }
    conditionVariable.notify_one();
  }

  CBE::QueryResultPtr waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    conditionVariable.wait(lock, [this]{ return finished; });
    if (!errorInfo.empty()) {
      throw std::runtime_error{errorInfo};
    }
    return queryResult;
  }
}; // struct QueryDelegate


#endif
