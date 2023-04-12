#ifndef CBE__util__Context_h__
#define CBE__util__Context_h__

//#include <functional>
//#include <iosfwd>
//#include <string>
#import <Foundation/Foundation.h>

//namespace cbe {
//  namespace util {

@interface CBEContext : NSObject {
//struct CBEContext {
//  using ReportFn = std::function<void(std::ostream&)>;

//  Context(ReportFn&& reportFn, const char fnName[]);
  //Context();

 // - (NSString*) report;

//  friend std::ostream& operator<<(std::ostream& os, const Context& context);
//private:
//  ReportFn reportFn{};
@public
}
@property(nonatomic, copy)		NSString* report;
@property(nonatomic, copy)		NSString*	fnName;

//  } // namespace util
//} // namespace cbe

@end
#endif // #ifndef CBE__util__Context_h__
