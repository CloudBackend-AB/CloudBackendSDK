//
//  Logic1.swift
//  testSDK
//
//  Created by CloudBackend on 2023-03-16.
//

import Foundation

class Logic1: NSObject,
              LogInDelegatePtr
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
    
    /* Functions for Exercise 3 */
    
    // --- Success Callbacks ---
    func onLog(inSuccess cloudBackend: CloudBackend!) {
        NSLog("LogIn success ")
        cbe1 = cloudBackend
        rootContainer = cbe1.account().rootContainer()
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

    // --- Driver Code ---
    func runTests()  {
        lock.lock()
        /* Exercise 1 */
        let myUsername : String = "***"
        let myPassword : String = "~~~"
        let myTenant : String = "***"
        // Login
        CloudBackend.login(myUsername, password:myPassword, tenant:myTenant, delegate:self)
        waitForRsp()
        NSLog("Logged in as: %@ SDK version %@", cbe1.account().username(), cbe1.version())
        
        /* Exercise 2 */
        
        /* Exercise 3 */
        
        NSLog("Exercise Completed!")
    }
}
