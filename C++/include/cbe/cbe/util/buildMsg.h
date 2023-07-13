#ifndef CBE__impl__buildMsg_h__
#define CBE__impl__buildMsg_h__

#include <sstream>

namespace cbe {
  namespace util {
    namespace impl {

template <typename T, typename... Ts>
void buildMsgImpl(std::ostream& os, T&& arg, Ts&&... restArgs);

    } // namespace impl


template <typename T, typename... Ts>
std::ostream& buildMsg(std::ostream& os, T&& arg, Ts&&... restArgs) {
  impl::buildMsgImpl(os, std::forward<T>(arg), std::forward<Ts>(restArgs)...);
  return os;
}

template <typename T, typename... Ts>
typename std::enable_if<!std::is_base_of<std::ostream,
                                         typename std::remove_reference<T>::type
              >::value, std::string>::type
buildMsg(T&& arg, Ts&&... restArgs) {
  std::ostringstream oss;
  buildMsg(oss /* os */, std::forward<T>(arg), std::forward<Ts>(restArgs)...);
  return oss.str();
}

namespace impl {

void buildMsgImpl(std::ostream&);

template <typename T, typename... Ts>
  void buildMsgImpl(std::ostream& os, T&& arg, Ts&&... restArgs) {
  os << std::forward<T>(arg);
  buildMsgImpl(os, std::forward<Ts>(restArgs)...);
}

    } // namespace impl
  } // namespace util
} // namespace cbe


#endif // #ifndef CBE__impl__buildMsg_h__
