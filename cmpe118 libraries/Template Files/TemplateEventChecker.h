/* 
 * File:   TemplateEventChecker.h
 * Author: Gabriel Hugh Elkaim
 *
 * Template file to set up typical EventCheckers for the  Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 *
 * This EventCheckers file will work with both FSM's and HSM's.
 *
 * Remember that EventCheckers should only return TRUE when an event has occured,
 * and that the event is a TRANSITION between two detectable differences. They
 * should also be atomic and run as fast as possible for good results.
 *
 * This is provided as an example and a good place to start.
 *
 * Created on September 27, 2013, 8:37 AM
 */

#ifndef TEMPLATEEVENTCHECKER_H
#define	TEMPLATEEVENTCHECKER_H

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

#define LOW_THRESHOLD 100   // placeholder for sensor thresholds, rename
#define HI_THRESHOLD 300    // this to something appropriate

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
 * @Function CheckAnEvent(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This is the function that checks if an event has occured. In general,
 *        it keeps a static variable to track a change from before. Make sure that
 *        you account for the first time it will be called so as not to generate
 *        an unecessary event. Returns TRUE if there was an event, FALSE otherwise.
 * @note Remember to rename this to something appropriate.
 * @author Gabriel H Elkaim, 2013.09.27 09:18 */
uint8_t CheckAnEvent(void);

/**
 * @Function CheckDifferentEvent(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This is the function that checks if an event has occured. In general,
 *        it keeps a static variable to track a change from before. Make sure that
 *        you account for the first time it will be called so as not to generate
 *        an unecessary event. Returns TRUE if there was an event, FALSE otherwise.
 * @note Remember to rename this to something appropriate.
 * @author Gabriel H Elkaim, 2013.09.27 09:18 */
uint8_t CheckDifferentEvent(void);

#endif	/* TEMPLATEEVENTCHECKER_H */

