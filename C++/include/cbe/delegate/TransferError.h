#ifndef CBE__delegate__TransferError_h__
#define CBE__delegate__TransferError_h__

#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include <iosfwd>

namespace cbe {
  namespace delegate {

using Error = delegate::Error;
/**
 * Contains error information delivered from %CloudBackend SDK in connection 
 * with a failed transfer, i.e., upload/download.
 */ 
class TransferError : public Error {
public:
  std::string           name{};
  cbe::ObjectId         objectId{};
  cbe::ContainerId      parentId{};
  TransferError();
  TransferError(Error&&           error,
                std::string       name,
                cbe::ObjectId     objectId,
                cbe::ContainerId  parentId);

  friend std::ostream& operator<<(std::ostream&         os,
                                  const TransferError&  transferError);
}; // class TransferError

  } // namespace delegate
} // namespace cbe

#endif // #ifndef CBE__delegate__TransferError_h__
