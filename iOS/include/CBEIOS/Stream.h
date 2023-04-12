/*
  Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_STREAM_H_
#define INCLUDE_CBE_STREAM_H_

#import <Foundation/Foundation.h>
#include "CBEIOS/Types.h"

/**
 * @brief binary data attached to Object
 * 
 */
@interface CBEStream : NSObject {

}
/**
 * the id number of the stream
 */
- (CBEStreamId) _streamId;
/** 
 * the size in Bytes 
 */
- (UInt64) _length;

-(id) initWith:(void*) cbiPeerPtr;
@end
#endif  // INCLUDE_CBE_STREAM_H_
