//
//  Logic2.swift
//  testSDK
//
//  Created by CloudBackend.
//

import Foundation

class Logic2: NSObject,
              LogInDelegatePtr, QueryDelegatePtr,
              CreateContainerDelegatePtr, RemoveContainerDelegatePtr
{
    // --- Global Variables ---
    var cbe1: CloudBackend!
    var rootContainer: CBEContainer!
    var newContainer: CBEContainer!
    var qR: CBEQueryResult!

    // Since the asynchronus code lives in the wrapped C++ code locks are used
    // to ensure that no race conditions are happening.
    let lock = NSLock()
    // --- Functions ---
    func waitForRsp() -> Void {
        lock.lock()
    }
    
    /* Functions for Exercise 2 */
    func removeContainer() {
        var count:Int = 0
        let items:[CBEItem] = qR.getItemsSnapshot() as! [CBEItem]
        while count < items.count {
            let item:CBEItem = items[count] //as! CBEItem
            if item.name() == "newTestContainer" {
                let c = item as! CBEContainer
                c.remove(self)
                waitForRsp()
            }
            count+=1
        }
    }
    func createContainer(container: CBEContainer) -> Void  {
        print("---Creating container")
        let containerName = "newTestContainer"
        rootContainer.createContainer(containerName, delegate:self)
    }
    func loadContainerContents(container: CBEContainer) {
        print("---Loading container contents")
        let filter1 = CBEFilter()
        filter1.setDataType(CBEItemTypeContainer)
        container.query(filter1, delegate:self)
    }
    func printContainerContents(qR: CBEQueryResult) -> Void {
        print("---Printing container contents")
        let items:[CBEItem] = qR.getItemsSnapshot() as! [CBEItem]
        var count:Int = 0
        while count < items.count {
            let item:CBEItem = items[count] //as! CBEItem
            print("testSDK item  \(count) \(String(describing: item)) \(String(describing: item.name()))")
            count+=1
        }
        lock.unlock()
    }
    /* Functions for Exercise 3 */

    // --- Success Callbacks ---
    func onLog(inSuccess cloudBackend: CloudBackend!) {
        NSLog("LogIn success ")
        cbe1 = cloudBackend
        rootContainer = cbe1.account().rootContainer()
        lock.unlock()
    }
    func onQuerySuccess(_ queryResult: CBEQueryResult!) {
        qR = queryResult
        lock.unlock()
    }
    func onCreateContainerSuccess(_ container: CBEContainer!) {
        newContainer = container
        lock.unlock()
    }
    func onRemoveContainerSuccess(_ containerId: CBEItemId, name: String!) {
        print("Removed old container...")
        lock.unlock()
    }

    // --- Error Callbacks ---
    func onLog(inError error: CBEError!, context: CBEContext!) {
        NSLog("LogIn error ")
        if let a = error, let b = context {
            NSLog("ErrorCode: %u, reason: %@, message: %@", a.errorCode, a.reason, a.message)
            NSLog("Function: %@, reason: %@", b.fnName, b.report)
        }
    }
    func onQueryError(_ error: CBEError!, context: CBEContext!) {
        if let a = error, let b = context {
            NSLog("ErrorCode: %u, reason: %@, message: %@", a.errorCode, a.reason, a.message)
            NSLog("Function: %@, reason: %@", b.fnName, b.report)
        }
    }
    func onCreateContainerError(_ error: CBEError!, context: CBEContext!) {
        if let a = error, let b = context {
            NSLog("ErrorCode: %u, reason: %@, message: %@", a.errorCode, a.reason, a.message)
            NSLog("Function: %@, reason: %@", b.fnName, b.report)
        }
    }
    func onRemoveContainerError(_ error: CBEError!, context: CBEContext!) {
        if let a = error, let b = context {
            NSLog("ErrorCode: %u, reason: %@, message: %@", a.errorCode, a.reason, a.message)
            NSLog("Function: %@, reason: %@", b.fnName, b.report)
        }
    }
    
    // --- Driver Code ---
    func runTests()  {
        lock.lock()
        /* Exercise 1 */
        let myUsername : String = "~~~"
        let myPassword : String = "***"
        let myTenant : String = "~~~"
        // Login
        CloudBackend.login(myUsername, password:myPassword, tenant:myTenant, delegate:self)
        waitForRsp()
        NSLog("Logged in as: %@ SDK version %@", cbe1.account().username(), cbe1.version())
        
        /* Exercise 2 */
        // Load container contents ie. query it to get its contents
        loadContainerContents(container: rootContainer)
        waitForRsp()
        
        // Remove container if it already exists
        removeContainer()
        
        // After loading the container, print its contents
        printContainerContents(qR:qR)
        waitForRsp()
        // Given the existing containers, should we create a new?
        createContainer(container:rootContainer)
        waitForRsp()
        
        /* Exercise 3 */
        
        NSLog("Exercise Completed!")
    }
}
