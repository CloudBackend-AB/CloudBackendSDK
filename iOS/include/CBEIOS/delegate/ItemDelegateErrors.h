#ifndef CBE__delegate__ItemDelegateErrors_h__
#define CBE__delegate__ItemDelegateErrors_h__

#include "CBE/Types.h"

#include "CBE/util/ErrorInfo.h"
#include "CBE/util/Exception.h"
#include "CBE/Filter.h"
#include "CBE/Item.h"

#include <iosfwd>
#include <string>

namespace cbe {
  namespace util {

struct Context;

  } // namespace util
  namespace delegate {

/**
 * Entity class containing the information passed into method
 * <b><c>%onLoadError()</c></b> in cbe interface
 */
class LoadError {
public:
  class Error {
  public:
    Filter      filter{};
    /**
     * Mimics the general error code encoding in the www.
     * \see [Wikipedia: List of HTTP status codes]
     *      (https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)
     */
    ErrorCode   errorCode{};
    std::string reason{};
    std::string message{};

    Error();
    Error(Filter&&      filter,
          ErrorCode     errorCode,
          std::string&& reason,
          std::string&& message);
  }; // class LoadError::Error

  virtual ~LoadError() = 0;

  /**
   * Called upon a failed query() or join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  virtual void onLoadError(Error&& error, cbe::util::Context&& context) = 0;

  friend std::ostream& operator<<(std::ostream&           os,
                                  const LoadError::Error& error);
}; // struct LoadError
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/**
 * Entity class containing the information passed into method
 * <b><c>%onJoinError()</c></b> in cbe interface
 */
class JoinError {
public:
  class Error {
  public:
    ErrorCode   errorCode{};
    std::string reason{};
    std::string message{};

    Error();
    Error(ErrorCode     errorCode,
          std::string&& reason,
          std::string&& message);
  }; // class JoinError::Error

  virtual ~JoinError() = 0;

  /**
   * Called upon a failed join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  virtual void onJoinError(Error&& error, cbe::util::Context&& context) = 0;

  friend std::ostream& operator<<(std::ostream&           os,
                                  const JoinError::Error& error);
}; // class JoinError::Error
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/**
 * Entity class containing the information passed into method
 * <b><c>%onItemError()</c></b> in cbe interface
 */
class ItemError {
public:
  struct Error {
  public:
    Item        container{cbe::DefaultCtor{}};
    ItemType    type{};
    ErrorCode   errorCode{};
    std::string reason{};
    std::string message{};

    Error();
    Error(Item          container,
          ItemType      type,
          ErrorCode     errorCode,
          std::string&& reason,
          std::string&& message);
  }; // struct ItemError

  virtual ~ItemError() = 0;
  /**
   * Called upon failed service calls like:
   * <ul>
   * <li> move
   * <li> remove
   * <li> rename
   * </ul>
   */
  virtual void onItemError(Error&& error, cbe::util::Context&& context) = 0;

  friend std::ostream& operator<<(std::ostream&           os,
                                  const ItemError::Error& error);
}; // class ItemItemError::Error

  } // namespace delegate
} // namespace cbe


#endif // #ifndef CBE__delegate__ItemDelegateErrors_h__
