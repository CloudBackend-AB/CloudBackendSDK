#ifndef CBE__util__Context_h__
#define CBE__util__Context_h__

#include <functional>
#include <iosfwd>
#include <string>

namespace cbe {
  namespace util {

struct Context {
  using ReportFn = std::function<void(std::ostream&)>;

  Context(ReportFn&& reportFn, const char fnName[]);
  Context();

  std::string report() const;

  friend std::ostream& operator<<(std::ostream& os, const Context& context);
private:
  ReportFn reportFn{};
public:
  std::string fnName{};
};

  } // namespace util
} // namespace cbe


#endif // #ifndef CBE__util__Context_h__
