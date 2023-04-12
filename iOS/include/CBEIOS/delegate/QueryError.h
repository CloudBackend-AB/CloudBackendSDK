#ifndef CBE__delegate__QueryError_h__
#define CBE__delegate__QueryError_h__

#include "CBE/Filter.h"
#include "CBE/Types.h"

#include "CBE/delegate/Error.h"


#include <iosfwd>

namespace cbe {
  namespace delegate {

using Error = delegate::Error;
/**
 * Contains error information delivered from %CloudBackend SDK i connection with
 * a failed query, or join
 */ 
class QueryError : public Error {
public:
  QueryError();
  QueryError(ErrorCode     errorCode,
             std::string&& reason,
             std::string&& message);
  QueryError(Filter&&      filter,
             ErrorCode     errorCode,
             std::string&& reason,
             std::string&& message);

  bool          hasFilter() const;
  const Filter& getFilter() const;

  friend std::ostream& operator<<(std::ostream&     os,
                                  const QueryError& error);           
  private:
    bool   hasFilter_{}; // false
    Filter filter{};
}; // class QueryError

  } // namespace delegate
} // namespace cbe


#endif // #ifndef CBE__delegate__QueryError_h__
