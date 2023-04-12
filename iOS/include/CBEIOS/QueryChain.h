/*
Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_QUERYCHAIN_H_
#define INCLUDE_CBE_QUERYCHAIN_H_
#import <Foundation/Foundation.h>

#include "CBEIOS/delegate/JoinDelegate.h"
#include "CBEIOS/QueryResult.h"

/**
 * @brief to do a search for Object combining more than one (CBEContainer*)table.
 *
 */
@interface CBEQueryChain : NSObject {
}
/**
 * Performs a join request in conjunction with a query, or previous join,
 * to get related items from another container.<br>
 * <b>Asynchronous</b> version of this service function.
 *
 * @param containerToQuery
 *          The (CBEContainer*)of which the join shall be done with.
 * @param key1
 *          The key from the previous query on which to perform the join.
 * @param key2
 *          The key on objects in \p containerToQuery on which to perform the
 *          join
 * @param joinDelegate
 *          Pointer to a @ref delegate::JoinDelegate "JoinDelegate"
 *          instance, implemented by the user, that receives the response of
 *          this query request.<br>
 *          I.e., either of the JoinDelegate callback functions
 *          @ref delegate::JoinDelegate::onJoinSuccess()
 *               "onJoinSuccess()" or
 *          @ref delegate::JoinDelegate::onJoinError()
 *               "onJoinError()"
 *          will be called.
 */
-(CBEQueryChain*) join:(CBEContainer*)      containerToQuery
                  key1:(NSString*)     key1
                  key2:(NSString*)     key2
                  joinDelegate:(id<JoinDelegatePtr>) joinDelegate;
/**
 * Same as join:(Container,(NSString*),(NSString*),delegate::JoinDelegatePtr),
 * but with an additional Filter parameter \p constraints .
 *
 * @param constraints A filter that provides any constraints for the query.
 */
-(CBEQueryChain*) join:(CBEContainer*)      containerToQuery
                  key1:(NSString*)     key1
                  key2:(NSString*)     key2
                  constraints:(CBEFilter*)          constraints
                  joinDelegate:(id<JoinDelegatePtr>) joinDelegate;
/**
 * Same as join:(Container,(NSString*),(NSString*),delegate::JoinDelegatePtr),
 * but with an additional (CBEContainer*)parameter \p containerForResults .
 *
 * @param containerForResults Can be the (CBEContainer*)from the previous query if
 *                            desired items should be from that container.
 */
-(CBEQueryChain*) join:(CBEContainer*)      containerToQuery
                  key1:(NSString*)     key1
                  key2:(NSString*)     key2
                  containerForResults:(CBEContainer*)      containerForResults
                  joinDelegate:(id<JoinDelegatePtr>) joinDelegate;
/**
 * Same as join:(Container,(NSString*),(NSString*),Container,delegate::JoinDelegatePtr),
 * but with an additional Filter parameter \p constraints .
 *
 * @param constraints A filter that provides any constraints for the query.
 */
-(CBEQueryChain*) join:(CBEContainer*) containerToQuery
                  key1:(NSString*)     key1
                  key2:(NSString*)     key2
                  constraints:(CBEFilter*)          constraints
                  containerForResults:(CBEContainer*)      containerForResults
                  joinDelegate:(id<JoinDelegatePtr>) joinDelegate;

/**
 * Returns the QueryResult provided to the delegate after the the last query()
 * or join:() call.
 * If this method is called before the delegate call, an empty QueryResult is
 * returned.
 */
-(CBEQueryResult*) getQueryResult;

- (id) initWith:(void*) cbiPeerPtr;

@end

/**
 * @brief Extension of class QueryChain
 *
 * Extension of class QueryChain offering join:() methods that do not require
 * a delegate.<br>
 * Instead, the delegate passed as argument in previous invocation in the
 * call chain - i.e.:
 * <ul>
 *   <li> CloudBackend::query(ContainerId,delegate::QueryJoinDelegatePtr) and
 *        overloads.
 *   <li> Container::query(delegate::QueryJoinDelegatePtr) and overloads.
 *   <li> QueryChain::join:(join:(Container,(NSString*),(NSString*),JoinDelegatePtr)
 *        and overloads.
 * </ul>
 * will be used.
 */

@interface CBEQueryChainExt : CBEQueryChain {
}

/**
 * Performs a join request in line with function
 * @ref QueryChain::join:(Container,(NSString*),(NSString*),delegate::JoinDelegatePtr)
 * "join:()" in class QueryChain.<br>
 * If a call to this join function is chained with a previous call to a query,
 * only such a query functions accepting a
 * @ref delegate::QueryJoinDelegate "QueryJoinDelegate" can be used as:
 * <ul>
 * <li> CloudBackend::query(ContainerId,delegate::QueryJoinDelegatePtr)
 * <li> CloudBackend::query(ContainerId,(CBEFilter*),delegate::QueryJoinDelegatePtr)
 * <li> Container::query(delegate::QueryJoinDelegatePtr)
 * <li> Container::query((CBEFilter*),delegate::QueryJoinDelegatePtr)
 * </ul>
 * <b>Asynchronous</b> version of this service function.
 * \see QueryChain::join:(Container,(NSString*),(NSString*),delegate::JoinDelegatePtr)
 */

-(CBEQueryChain*) join:(CBEContainer*) containerToQuery
                  key1:(NSString*)     key1
                  key2:(NSString*)     key2;
/**
 * \see QueryChain::join:(Container,(NSString*),(NSString*),(CBEFilter*),delegate::JoinDelegatePtr)
 */
-(CBEQueryChain*) join:(CBEContainer*) containerToQuery
                  key1:(NSString*)     key1
                  key2:(NSString*)     key2
                  constraints:(CBEFilter*)          constraints;
/**
 * \see QueryChain::join:(Container,(NSString*),(NSString*),Container,delegate::JoinDelegatePtr)
 */

-(CBEQueryChain*) join:(CBEContainer*) containerToQuery
                  key1:(NSString*)     key1
                  key2:(NSString*)     key2
                  containerForResults:(CBEContainer*)      containerForResults;
/**
 * \see QueryChain::join:(Container,(NSString*),(NSString*),(CBEFilter*),Container,delegate::JoinDelegatePtr)
 */

-(CBEQueryChain*) join:(CBEContainer*)                containerToQuery
                  key1:(NSString*)                    key1
                  key2:(NSString*)                    key2
                  constraints:(CBEFilter*)            constraints
                  containerForResults:(CBEContainer*) containerForResults;

@end

#endif // INCLUDE_CBE_ACCOUNT_H_
