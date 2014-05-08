/*
 * File: TemplateSubHSM.h
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a sub-Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that 
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * For the sub-state machines (flat or further hierarchical) you will need to post events
 * to the highest level state machine queue using its post function. If all your names are
 * unique within the ENUM, then you can have it in the header and use a query function that
 * returns the state. If not, then put the ENUM inside the .c file
 *
 *
 * This is provided as an example and a good place to start.
 *
 * Created on 23/Oct/2011
 * Updated on 16/Sep/2013
 */

#ifndef SUB_HSM_Template_H  // <- This should be changed to your own guard on both
#define SUB_HSM_Template_H  //    of these lines


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

// In case of repeated state names for different state machines, put the ENUM
// declaration inside the .c file. If all names are unique, then it can be put
// here in the .h, and you can add a query function as well.

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function InitTemplateSubHSM(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This is the init function used to initialize (or reset) your sub-state
 *        machine, which will call the RunTemplateSubHSM with an INIT_EVENT to
 *        go through the initialization. It is up to the higher level state machine
 *        to call this at the appropriate time.
 * @note Remember to rename this to something appropriate.
 *       Returns TRUE if successful, FALSE otherwise
 * @author Gabriel H Elkaim, 2013.17.23 00:18 */
uint8_t InitTemplateSubHSM(void);


/**
 * @Function RunTemplateSubHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the heirarchical state
 *        machine, as this is called any time a new event is passed to the event
 *        queue. This function will be called recursively to implement the correct
 *        order for a state transition to be: exit current state -> enter next state
 *        using the ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       The lower level state machines are run first, to see if the event is dealt
 *       with there rather than at the current level. ES_EXIT and ES_ENTRY events are
 *       not consumed as these need to pass pack to the higher level state machine.
 * @author J. Edward Carryer, 2011.10.23 19:25
 * @author Gabriel H Elkaim, 2011.10.23 19:25 */
ES_Event RunTemplateSubHSM(ES_Event ThisEvent);

#endif /* SUB_HSM_Template_H */

