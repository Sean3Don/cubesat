/*
 * File: RoachHSM.h
 * Author: Gabriel H Elkaim
 *
 * Simple flat state machine framework for the Roach project for CMPE-118 to get
 * you started on the project. The FSM simply ping pongs between two states on a
 * timer.
 */

#ifndef RoachHSM_H  
#define RoachHSM_H  


/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/

// typedefs for the states, which are used for the internal definition and also for
// the return of the query function. Be careful that all state names must be unique
// if they are exposed in the header file.

typedef enum {
    InitPState, Moving, Stopped
} RoachState_t; 


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function InitRoachHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunRoachHSM function.
 *        Returns TRUE if successful, FALSE otherwise
 * @author Gabriel H Elkaim, 2013.09.26 15:33 */
uint8_t InitRoachHSM(uint8_t Priority);

/**
 * @Function PostRoachHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Returns TRUE if successful, FALSE otherwise
 * @author Gabriel H Elkaim, 2013.09.26 15:33 */
uint8_t PostRoachHSM(ES_Event ThisEvent);

/**
 * @Function QueryRoachSM(void)
 * @param none
 * @return Current state of the state machine
 * @brief This function is a wrapper to return the current state of the state
 *        machine. Return will match the ENUM above.
 * @author Gabriel H Elkaim, 2013.09.26 15:33 */
RoachState_t QueryRoachSM(void);

/**
 * @Function RunRoachHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This is the function that implements the actual state machine; in the 
 *        roach case, it is a simple ping pong state machine that will form the 
 *        basis of your more complicated exploration. This function will be called
 *        recursively to implement the correct order for a state transition to be:
 *        exit current state -> enter next state using the ES_EXIT and ES_ENTRY
 *        events.
 * @author Gabriel H Elkaim, 2013.09.26 15:33 */
ES_Event RunRoachHSM(ES_Event ThisEvent);

#endif /* RoachHSM_H */

