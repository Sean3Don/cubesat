/*
 * File:   TemplateEventChecker.c
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
 * @Function CheckAnEvent(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This is the function that checks if an event has occured. In general,
 *        it keeps a static variable to track a change from before. Make sure that
 *        you account for the first time it will be called so as not to generate
 *        an unecessary event. Returns TRUE if there was an event, FALSE otherwise.
 * @note Remember to rename this to something appropriate. This particular example
 *       simply looks for a change in the sensor value.
 * @author Gabriel H Elkaim, 2013.09.27 09:18 */
uint8_t CheckAnEvent(void)
{
    static uint16_t prevSensorVal = -1;
    uint8_t returnVal = FALSE;
    uint16_t currentSensorVal;

    if (prevSensorVal == -1) { // first call to CheckAnEvent()
        prevSensorVal = ReadSensor(); // init the sensor memory
        return FALSE;
    }

    currentSensorVal = ReadSensor();
    if (currentSensorVal != prevSensorVal) { // if simple change is what you are looking for
        ES_Event thisEvent;
        thisEvent.EventType = MY_EVENT; // replace with your event and parameter
        thisEvent.EventParam = currentSensorVal;
        PostTemplateFSM(thisEvent); // post to the correct function
        returnVal = TRUE;
    }

    prevSensorVal = currentSensorVal; //
    return (returnVal);
}

/**
 * @Function CheckDifferentEvent(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This is the function that checks if an event has occured. In general,
 *        it keeps a static variable to track a change from before. Make sure that
 *        you account for the first time it will be called so as not to generate
 *        an unecessary event. Returns TRUE if there was an event, FALSE otherwise.
 * @note Remember to rename this to something appropriate. This particular example
 *       looks for a change in threshold and keeps track of its state internally.
 * @author Gabriel H Elkaim, 2013.09.27 09:18 */
uint8_t CheckDifferentEvent(void)
{

    static enum {NO_STATE, LOW_STATE, HIGH_STATE} myState = NO_STATE;
    uint8_t returnVal = FALSE;
    uint16_t currentVal;

    currentVal = ReadSensor();

    switch (myState) {
    case NO_STATE: // handle the initialization
        if (currentVal < LOW_THRESHOLD) {
            myState = LOW_STATE;
        } else {
            myState = HIGH_STATE;
        }
        break;

    case LOW_STATE:
        if (currentVal > HI_THRESHOLD) {
            myState = HIGH_STATE;
            returnVal = TRUE;
            ES_Event thisEvent;
            thisEvent.EventType = WAS_LOW_NOW_HIGH;
            thisEvent.EventParam = currentVal;
            PostTemplateFSM(thisEvent);
        }
        break;

    case HIGH_STATE:
        if (currentVal < LOW_THRESHOLD) {
            myState = LOW_STATE;
            returnVal = TRUE;
            ES_Event thisEvent;
            thisEvent.EventType = WAS_HIGH_NOW_LOW;
            thisEvent.EventParam = currentVal;
            PostTemplateFSM(thisEvent);
        }

        break;
    }
    return (returnVal);
}