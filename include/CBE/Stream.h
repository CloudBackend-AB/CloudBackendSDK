/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_STREAM_H_
#define INCLUDE_CBE_STREAM_H_

#include <string>
#include "CBE/Types.h"

namespace XAPI {
class DocumentStream;
};

namespace CBE {

class ObjectUpdater;

class Stream {
 public:
    Stream() : _id(0), _parentId(0), _userId(0), _length(0), _streamId(0), _name("") {
    }
    Stream(XAPI::DocumentStream& docStream);
    // Stream(CBE::Stream& stream);
    Stream(CBE::item_id_t id, CBE::container_id_t parentId, CBE::user_id_t uid, uint64_t sid, uint64_t length, std::string name) : _id(id), _parentId(parentId), _userId(uid), _streamId(sid), _length(length), _name(name) {
    }

    void setCloudBackend(CBE::ObjectUpdater* cb);

    CBE::item_id_t _id;
    CBE::container_id_t _parentId;
    CBE::user_id_t _userId;
    uint64_t _streamId;
    std::string _type;
    std::string _mime;
    CBE::date_t _created;
    CBE::date_t _updated;
    CBE::date_t _deleted;
    uint64_t _length;
    std::string _name;

    private:
    CBE::ObjectUpdater* cloudBackend;

};
}  // namespace CBE
// namespace CBE

#endif  // INCLUDE_CBE_STREAM_H_
