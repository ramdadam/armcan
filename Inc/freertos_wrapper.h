/**
 * @file    FreeRTOS_wrapper.h
 * @brief   Comfortable versions of FreeRTOS's functions
 * @author  Dr. Klaus Schaefer schaefer@eit.h-da.de
 */
 #ifndef FREERTOSWRAPPER_H_
 #define FREERTOSWRAPPER_H_
 
 #include "FreeRTOS.h"
 #include "task.h"
 #include "semphr.h"
 #include "queue.h"
 #include "timers.h"
 #include "event_groups.h"
 #include "my_assert.h"
 #include "memory.h"
 #include "string.h"
 
 //! Fast and easy initialization of auto variables
 template<typename x> void wipe(x &that)
 {
     for (uint8_t *ptr = (uint8_t *) &that; ptr < (uint8_t *) &that + sizeof(x);
             ++ptr)
         *ptr = 0;
 }
 
 #define STANDARD_TASK_PRIORITY (3 + tskIDLE_PRIORITY)
 
 #define INFINITE_WAIT portMAX_DELAY
 #define NO_WAIT  (( TickType_t )0)
 
 extern "C" BaseType_t xPortRaisePrivilege(void);
 #define drop_privileges() portSWITCH_TO_USER_MODE()
 #define acquire_privileges() xPortRaisePrivilege()
 
 //! Suspend execution of this task
 inline void suspend(void)
 {
     vTaskSuspend(0);
 }
 
 //! Resume task execution
 //! \param task_handle handle of FreeRTOS's task to be resumed
 inline void resume(const TaskHandle_t task_handle)
 {
     vTaskResume(task_handle);
 }
 
 //! Simple name for FreeRTOS's start function
 inline void Start_FreeRTOS()
 {
     vTaskStartScheduler();
 }
 
 //! Template for a queue for arbitrary data
 template<typename items>
 class Queue
 {
 public:
 //!  Queue constructor
 //! \param  length Number of items that can be stored
     Queue(unsigned length) :
             the_queue(0)
     {
         the_queue = xQueueCreate(length, sizeof(items));
         ASSERT(the_queue != 0);
     }
 protected:
     // support for Semaphore wanting item size = 0
     //!  protected alternate Queue constructor, only for use by semaphores
     Queue(unsigned length, unsigned size) :
             the_queue(0)
     {
         the_queue = xQueueCreate(length, size);
         ASSERT(the_queue != 0);
     }
 
 public:
     //!  Queue send method
     //! \param  item object to be sent
     //! \param TicksToWait maximum time to wait (optional)
     inline bool send(const items &item,
             unsigned TicksToWait = INFINITE_WAIT) const
     {
         return xQueueSend(the_queue, &item, TicksToWait) != pdFALSE;
     }
 
     //!  Queue receive method for use within ISR's
     //! \param  item object to be received
     inline bool receive_from_ISR(items &item) const
     {
         bool success;
         BaseType_t task_woken;
         success = xQueueReceiveFromISR(the_queue, &item, &task_woken);
         portEND_SWITCHING_ISR(task_woken);
         return success;
     }
 
     //!  Queue send method for use within ISR's
     //! \param  item object to be sent
     inline bool send_from_ISR(const items &item) const
     {
         bool success;
         BaseType_t task_woken;
         success = xQueueSendFromISR(the_queue, &item, &task_woken);
         portEND_SWITCHING_ISR(task_woken);
         return success;
     }
 
     //!  Queue receive method
     //! \param  item reference to an object to be received, will be overwritten
     //! \param TicksToWait maximum time to wait (optional ,default infinite wait)
     inline bool receive(items &item, unsigned TicksToWait = INFINITE_WAIT)
     {
         return xQueueReceive(the_queue, &item, TicksToWait) != pdFALSE;
     }
     //!  Queue messages waiting method
     inline uint16_t messages_waiting(void)
     {
         return uxQueueMessagesWaiting(the_queue);
     }
 protected:
     QueueHandle_t the_queue; //!< freeRTOS's Queue handle
 };
 
 //!  Counting and binary Semaphores
 class Semaphore: private Queue<char> // type char is a dummy
 {
 public:
 //!  Semaphore constructor
 //! \param  length counting semaphore size (optional), length=1 or missing: create binary semaphore
     Semaphore(unsigned length = 1) :
             Queue(length, 0)
     {
     }
 
     //!  signal method for use within tasks
     //! \param  TicksToWait maximum wait time (ticks)
     inline bool signal(unsigned TicksToWait = NO_WAIT)
     {
         return xQueueSend(the_queue, 0, TicksToWait) != pdFALSE;
     }
 
     //!  signal method for use within ISR's
     inline void signal_from_ISR(void) const
     {
         send_from_ISR(0);
     }
 
     //!  wait method
     //! \param  TicksToWait maximum wait time (ticks)
     inline bool wait(unsigned TicksToWait = INFINITE_WAIT)
     {
         return xQueueReceive(the_queue, 0, TicksToWait) != pdFALSE;
     }
 };
 
 //! Mutex class
 class Mutex
 {
 public:
     //!  Mutex constructor
     Mutex(void) :
             the_mutex(0)
     {
         the_mutex = xSemaphoreCreateMutex();
         ASSERT(the_mutex != 0);
     }
     //!  Lock method for Mutex
 //! \param TicksToWait maximum time to wait to gain access (optional)
     inline bool lock(unsigned TicksToWait = INFINITE_WAIT)
     {
         return xSemaphoreTake(the_mutex, TicksToWait) != pdFALSE;
     }
 //!  Release/Unlock method for Mutex
     inline void release(void)
     {
         xSemaphoreGive(the_mutex);
     }
 //!  Release/Unlock method for Mutex
     inline void unlock(void)
     {
         xSemaphoreGive(the_mutex);
     }
 private:
     SemaphoreHandle_t the_mutex; //!< FreeRTOS's SemaphoreHandle_t for the Mutex
 };
 
 //! Task class
 class Task
 {
 public:
     //! Task constructor
     //! \param test_task code to be executed (TaskFunction_t)
     //! \param name tasks name (for debugging)
     //! \param stacksize size of stack in 32bit units
     //! \param parameters generic pointer to data
     //! \param priority tasks priority, task will always created privileged
     Task(TaskFunction_t code, char const * name = "TSK", uint16_t stack_size =
             configMINIMAL_STACK_SIZE, void * parameters = 0,
             unsigned priority = STANDARD_TASK_PRIORITY)
     : task_handle(0)
     {
         xTaskCreate(code, name, stack_size, parameters,
                 priority | portPRIVILEGE_BIT, &task_handle);
         ASSERT(task_handle != 0);
     }
 
     //! Task handle getter function
     inline TaskHandle_t get_handle(void) const
     {
         return task_handle;
     }
 //! Task handle getter function
     operator TaskHandle_t(void) const
     {
         return task_handle;
     }
 
     inline void suspend(void) const
     {
         vTaskSuspend(task_handle);
     }
     //! Resume task execution, callable ONLY from a task !
     inline void resume(void) const
     {
         vTaskResume(task_handle);
     }
     //! Resume task execution, callable ONLY from ISR !
     inline void resume_from_ISR(void) const
     {
         xTaskResumeFromISR(task_handle);
     }
     static inline uint32_t notify_take( bool ClearCountOnExit=false, TickType_t TicksToWait=INFINITE_WAIT)
     {
         return ulTaskNotifyTake( ClearCountOnExit, TicksToWait );
     }
     //! Wait for task notification
     //!
     //! \see xTaskNotifyWait
     static inline bool notify_wait( uint32_t BitsToClearOnEntry, uint32_t BitsToClearOnExit,
                                          uint32_t &NotificationValue,
                                          TickType_t TicksToWait=INFINITE_WAIT)
     {
         return xTaskNotifyWait( BitsToClearOnEntry, BitsToClearOnExit,
                                 &NotificationValue,
                                 TicksToWait ) != 0;
     }
     //! Notify task: Increment task notification value
     //!
     //! \see xTaskNotifyGive
     inline void notify_give( void)
     {
           (void)xTaskNotifyGive( task_handle);
     }
     //! Notify task: Increment task notification value from ISR
     //!
     //! \see vTaskNotifyGiveFromISR
     void notify_give_from_ISR( void)
     {
         BaseType_t HigherPriorityTaskWoken=0;
         vTaskNotifyGiveFromISR( task_handle, &HigherPriorityTaskWoken);
         portEND_SWITCHING_ISR(HigherPriorityTaskWoken);
     }
     //! Notify task: set event bits
     //!
     //! \see xTaskNotify
     inline bool notify( uint32_t value, eNotifyAction eAction )
     {
      return xTaskNotify( task_handle, value, eAction);
     }
     //! Notify task: Set event bits from ISR
     //!
     //! \see xTaskNotifyFromISR
     inline bool notify_from_ISR( uint32_t value, eNotifyAction eAction)
     {
         BaseType_t HigherPriorityTaskWoken=0;
         BaseType_t ret = xTaskNotifyFromISR( task_handle, value, eAction, &HigherPriorityTaskWoken);
         portEND_SWITCHING_ISR(HigherPriorityTaskWoken); // sets pend_SV flag
         return ret != 0;
     }
     //! Clear task notification bits
     //!
     //! \see xTaskNotifySateClear
     inline void notify_clear( void)
     {
         xTaskNotifyStateClear( task_handle);
     }
 protected:
     Task(void) :
             task_handle(0)
     {
     }
     TaskHandle_t task_handle; //!< FreeRTOS's internal task handle
 };
 
 //! Resume task execution
 //! \param thatone Task to be resumed
 inline void resume(const Task &thatone)
 {
     vTaskResume(thatone.get_handle());
 }

 //! helper function to compute a size that expressible as 2^n
 inline unsigned fit_size( unsigned wanted_size)
 {
     unsigned tile_size=32;
     while( tile_size < wanted_size) // compute smallest MPU section size fitting demand
         tile_size <<=1;
     return tile_size;
 }
 
//! Delay task execution
 //! \param time delay time in ticks
 inline void delay(TickType_t time)
 {
     vTaskDelay(time);
 }
 
 //! \brief wrapper around FreeRTOS's timers
 //!
 //! Objects of this class allow spawning timed callback functions
 class timer
 {
 public:
     timer(TickType_t period, TimerCallbackFunction_t callback) :
             timer_ID(0)
     {
         timer_ID = xTimerCreate(0, period, 0, 0, callback);
         ASSERT(timer_ID != 0);
     }
     void start(void)
     {
         (void) xTimerStart(timer_ID, INFINITE_WAIT);
     }
     void stop(void)
     {
         (void) xTimerStop(timer_ID, INFINITE_WAIT);
     }
     //! \brief start a timer from ISR context
     //!
     //! attention: This function may never return if a task has been woken up!
     void start_from_ISR(void)
     {
         BaseType_t HigherPriorityTaskWoken = 0;
         (void) xTimerStartFromISR(timer_ID, &HigherPriorityTaskWoken);
         portEND_SWITCHING_ISR(HigherPriorityTaskWoken);
     }
 private:
     TimerHandle_t timer_ID;
 };
 
 //! \brief wrapper around vTaskDelayUntil(...)
 //!
 //! Objects of this class allow precise periodic and long-term clock-synchronous activities
 class synchronous_timer
 {
 public:
     synchronous_timer(TickType_t period = 0) :
             TimeIncrement(period), PreviousWakeTime( xTaskGetTickCount())
     {
     }
     //! synchronize to periodic timer
     inline void sync(void)
     {
         ASSERT(TimeIncrement != 0);
         // make sure there is a time greater than zero to wait
         ASSERT( xTaskGetTickCount() < (PreviousWakeTime + TimeIncrement));
         vTaskDelayUntil(&PreviousWakeTime, TimeIncrement);
     }
     //! re-synchronize to RTOS system time
     inline void reset(void)
     {
         PreviousWakeTime = xTaskGetTickCount();
     }
     //! synchronous delay using DelayUntil(...)
     inline void delay(TickType_t duration)
     {
         vTaskDelayUntil(&PreviousWakeTime, duration);
     }
 private:
     TickType_t TimeIncrement;
     TickType_t PreviousWakeTime;
 };
 
 //! \brief Class event_group
 //!
 //! \see EventGroup
 class event_group
 {
 public:
     //! \brief Default constructor for an empty event_group object
     event_group( void)
     : EventGroup( xEventGroupCreate())
     {};
     //! \brief wait for a bit or group of bits to become set
     //! \param BitsToWaitFor bitmask for bits to wait for
     //! \param ClearOnExit if true: clear all bits from BitsToWaitFor on exit, otherwise don't clear any bits
     //! \param WaitForAllBits if true: wait for all bits, otherwise wait for a single bit
     //! \return the bits that are presently set on function return
     inline EventBits_t wait_bits(
             const EventBits_t BitsToWaitFor,
             bool ClearOnExit,
             bool WaitForAllBits,
             TickType_t TicksToWait = INFINITE_WAIT)
         {
             return xEventGroupWaitBits(
                 EventGroup,
                 BitsToWaitFor,
                 ClearOnExit,
                 WaitForAllBits,
                 TicksToWait );
         }
     //! \brief set bits, not to be called from ISR's
     inline EventBits_t set_bits( const EventBits_t BitsToSet)
         {
             return xEventGroupSetBits( EventGroup, BitsToSet);
         }
     //! \brief set bits, only to be called from ISR's
     void set_bits_from_ISR( const EventBits_t BitsToSet)
     {
         BaseType_t HigherPriorityTaskWoken=0;
         bool success = xEventGroupSetBitsFromISR(
                    EventGroup,
                    BitsToSet,
                    &HigherPriorityTaskWoken );
         ASSERT( success);
         portEND_SWITCHING_ISR( HigherPriorityTaskWoken);
     }
     //! \brief return current value
     inline EventBits_t get_bits( void)
     {
         return xEventGroupGetBits( EventGroup);
     }
     //! \brief clear a bit or a set of bits
     inline EventBits_t clear_bits( const EventBits_t BitsToclear)
     {
         return xEventGroupClearBits( EventGroup, BitsToclear);
     }
     //! \brief Report events and synchronize tasks (also called "rendezvous")
     //!
     //! Atomically set bits (flags) within an RTOS event group,
     //! then wait for a combination of bits to be set
     //! \param uxBitsToSet bits to set when function is called
     //! \param BitsToWaitFor bits to wait for before return
     //! \return the set of bits that are set the moment the function returns
     inline EventBits_t syncronize(
             const EventBits_t BitsToSet,
             const EventBits_t BitsToWaitFor,
             TickType_t xTicksToWait = INFINITE_WAIT)
     {
         return xEventGroupSync( EventGroup,
                 BitsToSet,
                 BitsToWaitFor,
                 xTicksToWait );
     }
 private:
     EventGroupHandle_t EventGroup; //!< FreeRTOS's internal EventGroup handle
 
 };
 
 #endif /* FREERTOSWRAPPER_H_ */
 