/*
 * File:   TemplateEventChecker.c
 * Author: Gabriel Hugh Elkaim (Max Dunne)
 *
 * Template file to set up Testing for Simple Services in the 
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 * Use a global search and replace to replace "ServiceInterfaceFunctionofInterest" 
 * with public access function from service of choice.  adding TestAService to 
 * ES_Configure Event Checkers will allow you to see when a particular service 
 * is changing states.
 * 
 * This is provided as an example and a good place to start.
 *
 * Created on September 27, 2013, 8:37 AM
 */

#ifndef TEMPLATETESTSERVICEEVENTCHECKER_H
#define	TEMPLATETESTSERVICEEVENTCHECKER_H

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT


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
 * @Function TestAService(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This is the function that checks if an event has occured. In general,
 *        it keeps a static variable to track a change from before. Make sure that
 *        you account for the first time it will be called so as not to generate
 *        an unecessary event. Returns TRUE if there was an event, FALSE otherwise.
 * @note Remember to rename this to something appropriate. This particular example
 *       simply looks for a change in the sensor value.
 * @author Gabriel H Elkaim, 2013.09.27 09:18
 * @author Max Dunne, 2013.11.16 13:26 */
uint8_t TestAService(void);


#endif	/* TEMPLATETESTSERVICEEVENTCHECKER_H */

