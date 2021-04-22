/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_STREAM_H_
#define INCLUDE_CBE_STREAM_H_

#include <string>
#include "CBE/Types.h"

namespace CBE {

class Stream {
 public:
    //Future get functions and other functionality will be added here.
    uint64_t _streamId;
    size_t _length;
 protected:
    Stream() : _length{0}, _streamId{0} {}
    Stream(uint64_t streamId, size_t length);
     
};
}  // namespace CBE
// namespace CBE

#endif  // INCLUDE_CBE_STREAM_H_
