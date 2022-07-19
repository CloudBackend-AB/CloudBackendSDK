#include "Airplanes.h"

#include "QueryDelegate.h"

#include "CBE/CloudBackend.h"
#include "CBE/Protocols/AccountEventProtocol.h"
#include "CBE/Protocols/ItemEventProtocol.h"

#include <algorithm>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <stdexcept>
#include <sstream>


Airplanes::Airplanes(const std::string& credentials)
    : cloudBackend{}, account{} {
  constexpr const auto  username = "githubtester2";
  constexpr const auto  password = "gitHubTester2password";
  constexpr const auto  tenant   = "cbe_githubtesters";

  struct AccountDelegate : public CBE::AccountEventProtocol {
    CBE::CloudBackendPtr  cloudBackend{};
    std::string           errorInfo{};
    bool                  finished{};

    std::mutex              mutex{};
    std::condition_variable conditionVariable{};

    void onLogin(uint32_t atState, CBE::CloudBackendPtr cloudBackend) final {
      {
        std::lock_guard<std::mutex> lock(mutex);
        this->cloudBackend = cloudBackend;
        finished = true;
      }
      conditionVariable.notify_one();
    }
    void onError(uint32_t failedAtState, uint32_t code, std::string reason,
                 std::string message) final {
      std::ostringstream oss;
      oss << "Login error: failedAtState=\"" << failedAtState << "\", "
          << "code=\"" << code << "\", "
          << "reason=\"" << reason << "\", "
          << "message=\"" << message << "\"";
      {
        std::lock_guard<std::mutex> lock(mutex);
        this->errorInfo = oss.str();
        finished = true;
      }
      conditionVariable.notify_one();
    }

    CBE::CloudBackendPtr waitForRsp() {
      std::unique_lock<std::mutex> lock(mutex);
      conditionVariable.wait(lock, [this]{ return finished; });
      if (!errorInfo.empty()) {
        throw std::runtime_error{errorInfo};
      }
      return cloudBackend;
    }
  }; // struct AccountDelegate

  const auto accountDelegate = std::make_shared<AccountDelegate>();
  CBE::CloudBackend::logIn(username, password, tenant, accountDelegate);
  cloudBackend = accountDelegate->waitForRsp();
  account = cloudBackend->account();
} // Airplanes ctor

namespace {


} // Anonymous namespace

CBE::QueryResultPtr Airplanes::query(CBE::Filter         filter,
                                     CBE::container_id_t containerId) {
  const auto delegate = std::make_shared<QueryDelegate>();
  cloudBackend->query(containerId, filter, delegate);
  return delegate->waitForRsp();
}

namespace {

template <typename ResultT>
struct DelegateWithItemError : public CBE::ItemEventProtocol {
  const std::string       context;
  ResultT                 result{};
  std::string             errorInfo{};
  bool                    finished{};

  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  DelegateWithItemError(std::string context) : context(std::move(context)) {}

  void onItemError(CBE::ItemPtr container, CBE::item_t type,
                   uint32_t operation, uint32_t failedAtState, uint32_t code,
                   std::string reason, std::string message) final {
    std::ostringstream oss;
    oss <<  "Error: Operation '" << context << "' failed on container \""
        << container->name() << "\": "
        << "type="           << type
        << ",operation="     << operation
        << ",failedAtState=" << failedAtState
        << "\",code="        << code
        << ",reason=\""      << reason
        << "\",message=\""   << message << "\"";
    {
      std::lock_guard<std::mutex> lock(mutex);
      this->errorInfo = oss.str();
      finished = true;
    }
    conditionVariable.notify_one();
  }
protected:
  void onSuccess(ResultT&& result) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      this->result = std::move(result);
      finished = true;
    }
    conditionVariable.notify_one();
  }
public:
  ResultT waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    conditionVariable.wait(lock, [this]{ return finished; });
    if (!errorInfo.empty()) {
      throw std::runtime_error{errorInfo};
    }
    return result;
  }
}; // struct DelegateWithItemError

} // Anonymous namespace

CBE::ContainerPtr Airplanes::createContainer(
                                            CBE::ContainerPtr   parentContainer,
                                            const std::string&  name) {
  struct Delegate : DelegateWithItemError<CBE::ContainerPtr> {
    // Inherit base class' constructor
    using DelegateWithItemError<CBE::ContainerPtr>::DelegateWithItemError;

    void onContainerAdded(CBE::ContainerPtr container) final {
      onSuccess(std::move(container));
    }
  };

  const auto delegate = std::make_shared<Delegate>("create container");
  parentContainer->create(name, delegate);
  return delegate->waitForRsp();
} // struct Delegate

CBE::ObjectPtr    Airplanes::createObject(CBE::ContainerPtr     parentContainer,
                                          const std::string&    name,
                                          CBE::metadata_type&&  metadata) {
  struct Delegate : DelegateWithItemError<CBE::ObjectPtr> {
    // Inherit base class' constructor
    using DelegateWithItemError<CBE::ObjectPtr>::DelegateWithItemError;

    void onObjectAdded(CBE::ObjectPtr object) final {
      onSuccess(std::move(object));
    }
  }; // struct Delegate

  std::for_each(std::begin(metadata), std::end(metadata),
       [](const CBE::metadata_type::value_type& entry) {
         std::cout << std::setw(14) << entry.second.first;
       });
  std::cout << '\t';
  const auto delegate = std::make_shared<Delegate>("create object");
  parentContainer->createObject(name, delegate, metadata);
  const auto object = delegate->waitForRsp();
  std::cout << "Created object \"" << object->name() << "\" with id " <<
      object->id() << '\n';
  return object;
}
void Airplanes::removeContainer(CBE::ContainerPtr   container) {
  struct Delegate : DelegateWithItemError<std::string> {
    // Inherit base class' constructor
    using DelegateWithItemError<std::string>::DelegateWithItemError;

    void onContainerRemoved(CBE::item_id_t containerId, std::string name) final
    {
      onSuccess(std::move(name));
    }
  }; // struct Delegate

  const auto delegate = std::make_shared<Delegate>("remove container");
  container->remove(delegate);
  (void) delegate->waitForRsp();
}
