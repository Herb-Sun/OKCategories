//
//  GCD+OKCategory
//
//  Copyright © 2016年 OK Inc. All rights reserved.
//

#ifndef GCD_OKCategory_h
#define GCD_OKCategory_h

#include <dispatch/dispatch.h>


/// Deadlock warnings may be turned on/off by defining or not defining 
/// DEADLOCK_WARNINGS. It's best to turn these warnings on in debug mode.
#if defined(DEADLOCK_WARNINGS)

/// Issues a warning when a synchronous method is called from the main queue.
/// Put this macro in your sychronous methods that are not safe for the main 
/// queue. Intended for Objective-C.
#define DEADLOCK_SYNC_WARNING() \
if (dispatch_get_current_queue() == dispatch_get_main_queue()) \
NSLog(\
@"WARNING: Deadlock possible. "\
"Calling sychronous method from main queue in method: "\
"%@ file: %s line: %d.", \
NSStringFromSelector(_cmd), \
__FILE__, \
__LINE__);

/// Issues a warning when a synchronous method is called from the main queue.
/// Put this macro in your sychronous methods that are not safe for the main 
/// queue. Intended for C or Objective-C.
#define DEADLOCK_SYNC_WARNING_C() \
if (dispatch_get_current_queue() == dispatch_get_main_queue()) \
NSLog(\
@"WARNING: Deadlock possible. "\
"Calling sychronous function from main queue in "\
"%@ file: %s line: %d.", \
__FILE__, \
__LINE__);

#else
#define DEADLOCK_SYNC_WARNING()
#define DEADLOCK_SYNC_WARNING_C() 
#endif


/// Race condition warnings may be turned on/off by defining or not defining 
/// RACE_CONDITION_WARNINGS. It's best to turn these warnings on in debug mode.
#if defined(RACE_CONDITION_WARNINGS)

/// Issues a warning when the current queue is not the given queue. Useful for
/// when you want to make sure a certain method is only run with the given 
/// queue. Intended to be used with Objective-C. 
/// q must be of type dispatch_queue_t.
#define RACE_CONDITION_WARNING(q) \
if (dispatch_get_current_queue() != q) \
NSLog(\
@"WARNING: Race condition possible. " \
"Running method from wrong queue in method: "\
"%@ file: %s line: %d. Expected queue: %s but running in: %s",\
NSStringFromSelector(_cmd), \
__FILE__, \
__LINE__, \
dispatch_queue_get_label(q), \
dispatch_queue_get_label(dispatch_get_current_queue()));

/// Issues a warning when the current queue is not the given queue. Useful for
/// when you want to make sure a certain method is only run with the given 
/// queue. Intended to be used with Objective-C.
/// q must be of type dispatch_queue_t.
#define RACE_CONDITION_WARNING_C(q) \
if (dispatch_get_current_queue() != q) \
NSLog(\
@"WARNING: Race condition possible. " \
"Running method from wrong queue in method: "\
"%@ file: %s line: %d. Expected queue: %s but running in: %s",\
NSStringFromSelector(_cmd), \
__FILE__, \
__LINE__, \
dispatch_queue_get_label(q), \
dispatch_queue_get_label(dispatch_get_current_queue()));

/// Issues a warning when the current queue is not the main queue. Useful for
/// when you want to make sure a certain method is only run with the main queue.
/// Intended to be used with Objective-C.
#define RACE_CONDITION_WARNING_MAIN() RACE_CONDITION_WARNING(dispatch_get_main_queue())

/// Issues a warning when the current queue is not the main queue. Useful for
/// when you want to make sure a certain method is only run with the main queue.
/// Intended to be used with C or Objective-C.
#define RACE_CONDITION_WARNING_MAIN_C() RACE_CONDITION_WARNING_C(dispatch_get_main_queue())

#else
#define RACE_CONDITION_WARNING(q) 
#define RACE_CONDITION_WARNING_C(q) 
#define RACE_CONDITION_WARNING_MAIN()
#define RACE_CONDITION_WARNING_MAIN_C() 
#endif

/// A convenient block type to use with dispatch_stride.
typedef void(^StrideBlock)(size_t idx);

/**
 * Convenience method for asynchronously dispatching to the main queue.
 *
 * @param block The block to asynchrously dispatch. Must not be NULL.
 */
void dispatch_async_main(dispatch_block_t block);

/**
 * Convenience method for asynchronously dispatching to the low priority queue.
 *
 * @param block The block to asynchrously dispatch. Must not be NULL.
 */
void dispatch_async_low(dispatch_block_t block);

/**
 * Convenience method for asynchronously dispatching to the default priority 
 * queue.
 *
 * @param block The block to asynchrously dispatch. Must not be NULL.
 */
void dispatch_async_default(dispatch_block_t block);

/**
 * Convenience method for asynchronously dispatching to the high priority queue.
 *
 * @param block The block to asynchrously dispatch. Must not be NULL.
 */
void dispatch_async_high(dispatch_block_t block);

/**
 * Convenience method for using the striding pattern. Runs the given block on 
 * the given queue in parallel. The block will be run 'iterations' number of 
 * times and 'stride' number of iterations will run per thread.
 *
 * @param stride The number of times to run 'block' per thread.
 * @param iterations The total number of times to run 'block'.
 * @param queue The queue to run the block on. This must be a concurrent queue 
 * to take advantage of parallelization.
 * @param block The block of code to run in parallel. Must be thread safe and 
 * independent of other iterations. Must not be NULL.
 */
void dispatch_stride(size_t stride, size_t iterations, dispatch_queue_t queue, StrideBlock block);

#endif
