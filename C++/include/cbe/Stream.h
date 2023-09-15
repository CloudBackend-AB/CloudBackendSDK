/*
  Copyright © CloudBackend AB 2020-2023.
*/

#ifndef INCLUDE_CBE_STREAM_H_
#define INCLUDE_CBE_STREAM_H_

#include "cbe/Types.h"

#include <string>
#include <vector>

namespace CBI {
class Stream;
} // namespace CBI

namespace cbe {

/**
 * @brief A data file attached to Object.
 * 
 * An object kan have zero, one or many streams.
 */
class Stream {
public:
  //Future get functions and other functionality will be added here.
  /**
   * the id number of the stream
   */
  cbe::StreamId streamId{};

  /**
   * the size in Bytes
   */
  std::size_t   length{};

  Stream();
  ~Stream();
private:
  friend class Object;
  Stream(const CBI::Stream& cbiStream);
  explicit operator CBI::Stream () const;
  //fix cbiPeer pointer
}; // class Stream

/**
 * @brief Collection of @ref cbe::Stream "Stream" objects. 
 */
using Streams = std::vector<cbe::Stream>;

} // namespace cbe

#endif  // INCLUDE_CBE_STREAM_H_
