/*
 * File:   TemplateEventChecker.c
 * Author: Gabriel Hugh Elkaim (Max Dunne)
 *
 * Template file to set up Testing for Simple Services in the 
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 * Use a global search and replace to replace "ServiceInterfaceFunctionofInterest" 
 * with public access function from service of choice.  Adding TestAService to 
 * ES_Configure Event Checkers will allow you to see when a particular service 
 * is changing states.
 * 
 * This is provided as an example and a good place to start.
 *
 * Created on September 27, 2013, 8:37 AM
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "TemplateEventChecker.h"
#include "ES_Events.h"
#include "TemplateFSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this EventChecker. They should be functions
   relevant to the behavior of this particular state machine */

extern uint16_t ReadSensor(void); // placeholder for real functions

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */



/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
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
uint8_t TestAService(void)
{
    static int32_t prevSensorVal = -1;
    uint8_t returnVal = FALSE;
    int32_t currentSensorVal;

    if (prevSensorVal == -1) { // first call to CheckAnEvent()
        prevSensorVal = ServiceInterfaceFunctionofInterest(); // init the sensor memory
        return FALSE;
    }

    currentSensorVal = ServiceInterfaceFunctionofInterest()();
    if (currentSensorVal != prevSensorVal) { // if simple change is what you are looking for
        printf("Access Function %s changed values from %d to %d\r\n","ServiceInterfaceFunctionofInterest",prevSensorVal,currentSensorVal);
    }

    prevSensorVal = currentSensorVal; //
    return (returnVal);
}
