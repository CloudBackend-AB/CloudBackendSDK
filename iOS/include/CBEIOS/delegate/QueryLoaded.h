#ifndef CBE__delegate__QueryLoaded_h__
#define CBE__delegate__QueryLoaded_h__

#include "CBE/QueryResult.h"

namespace cbe {
    namespace delegate {

struct QueryLoaded {
  virtual ~QueryLoaded();

  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  virtual void onQuerySuccess(cbe::QueryResult&& queryResult) = 0;
}; // struct QueryLoaded

  } // namespace delegate
} // namespace cbe


#endif // #ifndef CBE__delegate__QueryLoaded_h__
