#ifndef CBE__util__ErrorInfo_h__
#define CBE__util__ErrorInfo_h__

#include <iosfwd>
#include <string>

namespace cbe {
  namespace util {

struct Context;

struct ErrorInfo {
  std::string contextStr{};

  virtual void printError(std::ostream& os) const = 0;

  explicit operator bool () const;

  virtual ~ErrorInfo();
protected:
  ErrorInfo(Context&& context);
  ErrorInfo();
};
std::ostream& operator<<(std::ostream& os, const ErrorInfo& ei);


template <class ErrorT>
struct ErrorInfoImpl : ErrorInfo {
  ErrorInfoImpl();
  ErrorInfoImpl(Context&& context, ErrorT&& error);

  template <class ErrorT2>
  ErrorInfoImpl& operator=(ErrorInfoImpl<ErrorT2>&& rh);

  using Base = ErrorInfoImpl<ErrorT>;

  ErrorT       error{};

  void printError(std::ostream& os) const final;
};

template <class ErrorT>
ErrorInfoImpl<ErrorT>::ErrorInfoImpl(Context&& context, ErrorT&& error)
  : ErrorInfo{std::move(context)}, error{std::move(error)} {}

template <class ErrorT>
ErrorInfoImpl<ErrorT>::ErrorInfoImpl() = default;

template <class ErrorT>
  template <class ErrorT2>
ErrorInfoImpl<ErrorT>&
ErrorInfoImpl<ErrorT>::operator=(ErrorInfoImpl<ErrorT2>&& rh) {
  ErrorInfo::operator=(std::move(rh));
  error = std::move(rh.error);
  return *this;
}

template <class ErrorT>
void ErrorInfoImpl<ErrorT>::printError(std::ostream& os) const {
  os << error;
}

  } // namespace util
} // namespace cbe


#endif // #ifndef CBE__util__ErrorInfo_h__
