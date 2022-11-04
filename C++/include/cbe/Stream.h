/*
  Copyright © CloudBackend AB 2020 - 2022.
*/

#ifndef INCLUDE_CBE_STREAM_H_
#define INCLUDE_CBE_STREAM_H_

#include <string>
#include "cbe/Types.h"

namespace CBI {
class Stream;
} // namespace CBI

namespace cbe {

/**
 * @brief binary data attached to Object
 * 
 */
class Stream {
public:
  //Future get functions and other functionality will be added here.
  /**
   * the id number of the stream
   */
  cbe::StreamId _streamId{};

  /**
   * the size in Bytes
   */
  std::size_t   _length{};

  Stream();
  ~Stream();
private:
  friend class Object;
  Stream(const CBI::Stream& cbiStream);
  explicit operator CBI::Stream () const;
  //fix cbiPeer pointer
}; // class Stream

} // namespace cbe

#endif  // INCLUDE_CBE_STREAM_H_
