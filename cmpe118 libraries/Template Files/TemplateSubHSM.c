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
 *
 * History
 * When           Who     What/Why
 * -------------- ---     --------
 * 09/13/13 15:17 ghe      added tattletail functionality and recursive calls
 * 01/15/12 11:12 jec      revisions for Gen2 framework
 * 11/07/11 11:26 jec      made the queue static
 * 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 * 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "TemplateHSM.h"
#include "TemplateSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/

/* You will need the state variable; you may need others as well.
 * The type of state variable should match that of enum. If all of the enums are
 * unique, then you can move it to the header file and add a query function. */

typedef enum {
    InitPSubState, FirstState, OtherState, YetAnotherState
} TemplateSubState_t; // <- Change this name along with your state names

static TemplateSubState_t CurrentState = InitPSubState; // <- change name to match ENUM


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
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
uint8_t InitTemplateSubHSM(void)
{
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunTemplateSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

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
ES_Event RunTemplateSubHSM(ES_Event ThisEvent)
{
    uint8_t makeTransition = FALSE; // use to flag transition
    TemplateSubState_t nextState;

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
    case InitPSubState: // If current state is initial Psedudo State
        if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
        {
            // this is where you would put any actions associated with the
            // transition from the initial pseudo-state into the actual
            // initial state

            // now put the machine into the actual initial state
            CurrentState = FirstState;
            ThisEvent.EventType = ES_NO_EVENT;
        }
        break;

    case FirstState: // in the first state, replace this with correct names
        ThisEvent = RunSubFirstStateHSM(ThisEvent); // you can keep going down levels
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                // this is where you would put any actions associated with the
                // entry to this state
                break;

            case ES_EXIT:
                // this is where you would put any actions associated with the
                // exit from this state
                break;

            case KEEP_EVENTS:
                // this is an example where the state does NOT transition
                // do things you need to do in this state
                // event consumed
                ThisEvent.EventType = ES_NO_EVENT;
                break;

            case OTHER_EVENTS:
                // create the case statement for all other events that you are
                // interested in responding to.
                nextState = OtherState;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
                break;

            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;

    case OtherState: // If current state is state OtherState
        switch (ThisEvent.EventType) {
        case ES_ENTRY:
            // this is where you would put any actions associated with the
            // entry to this state
            break;

        case ES_EXIT:
            // this is where you would put any actions associated with the
            // exit from this state
            break;

        case KEEP_EVENTS:
            // this is an example where the state posts an even to the higher
            // level state machine, in order to see how it is done.
        {
            ES_Event postEvent;
            postEvent.EventType = DONE_EVADING;
            postEvent.EventParam = 0x0000;
            PostTemplateHSM(postEvent);
            ThisEvent.EventType = ES_NO_EVENT;
        }
            break;

        case OTHER_EVENTS:
            // create the case statement for all other events that you are
            // interested in responding to. This does a transition
            nextState = YetAnotherState;
            makeTransition = TRUE;
            ThisEvent.EventType = ES_NO_EVENT;
            break;

        default: // all unhandled events pass the event back up to the next level
            break;
        }
        break;

    case YetAnotherState: // example of a state without a sub-statemachine
        switch (ThisEvent.EventType) {
        case ES_ENTRY:
            // this is where you would put any actions associated with the
            // entry to this state
            break;

        case ES_EXIT:
            // this is where you would put any actions associated with the
            // exit from this state
            break;

        case KEEP_EVENTS:
            // this is an example where the state does NOT transition
            // do things you need to do in this state
            // event consumed
            ThisEvent.EventType = ES_NO_EVENT;
            break;

        case OTHER_EVENTS:
            // create the case statement for all other events that you are
            // interested in responding to. This one does a transition
            nextState = FirstState;
            makeTransition = TRUE;
            ThisEvent.EventType = ES_NO_EVENT;
            break;

        default: // all unhandled events pass the event back up to the next level
            break;
        }
        break;

    default: // all unhandled states fall into here
        break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunTemplateSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunTemplateSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/

/* Note that you cannot test the SubHSM directly as you need the call to sub
 * state machine from the higher level state machine if you want to run it from
 * the ES_Framework.
 *
 * You can  create a simple HSM template that has one state that simply
 * drops events to the sub-state machine in this file, and run its test harness.
 *
 * The other alternative is to create a simple test harness that simply calls
 * you functions and exercises them directly. This will be demonstrated below. */

#ifdef TEMPLATESUBHSM_TEST // <-- change this name and define it in your MPLAB-X
                           //     project to run the test harness

#define NOPCOUNT 220000
#define DELAY() for(i=0; i< NOPCOUNT; i++) __asm("nop")

#include <stdio.h>

void main(void)
{
    int i;
    uint8_t result;
    ES_Event testEvent, returnEvent;
    BOARD_Init();
    // When doing testing, it is useful to annouce just which program
    // is running.

    printf("Starting the Hierarchical Sub State Machine Test Harness \r\n");
    printf("testing blind without the ES_Framework\n\r");

    DELAY();
    // Your hardware initialization function calls go here

    // now initialize the sub-HSM
    result = InitTemplateSubHSM();
    printf("\nInitialization ");
    if (result == TRUE) {
        printf("succeeded");
    } else {
        printf("failed");
    }

    DELAY();
    // inject events and see what happens
    testEvent.EventType = ES_INIT;
    testEvent.EventParam = 0x0000;

    returnEvent = RunTemplateSubHSM(testEvent);
    printf("\nRunTemplateSubHSM with ES_INIT returned event: [%u] ", returnEvent.EventType);
    printf("Current State is: %d", CurrentState);
    DELAY();

    testEvent.EventType = ES_TIMEOUT; // this is an example, change to suit your needs
    testEvent.EventParam = 9;

    returnEvent = RunTemplateSubHSM(testEvent);
    printf("\nRunTemplateSubHSM with ES_TIMEOUT returned event: [%u] ", returnEvent.EventType);
    printf("Current State is: %d", CurrentState);
    DELAY();

    // change as required

    while(1) {
        ;
    }
}

#endif // TEMPLATESUBHSM_TEST

