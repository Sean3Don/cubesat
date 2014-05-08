#include <BOARD.h>
#include "RoachFrameworkEvents.h"
#include "ES_ServiceHeaders.h"
#include "ES_Events.h"
#include "roach.h"


#define DARK_THRESHOLD 234
#define LIGHT_THRESHOLD 88



uint8_t CheckLightLevel(void) {
    static lightstate_t lastLightState =LIGHT;
    lightstate_t currentLightState=lastLightState;
    unsigned int currentLightValue;
    uint8_t returnVal = FALSE;
    // check the light level and assign LIGHT or DARK
    currentLightValue = Roach_LightLevel();
    if (currentLightValue > DARK_THRESHOLD) {
        currentLightState = DARK;
    }
    if (currentLightValue < LIGHT_THRESHOLD) {
        currentLightState = LIGHT;
    }
    if (currentLightState != lastLightState) { //event detected
        ES_Event ThisEvent;
        ThisEvent.EventType = LIGHTLEVEL;
        ThisEvent.EventParam = (uint16_t) currentLightState;
        PostRoachFSM(ThisEvent);
        returnVal = TRUE;
    }
    lastLightState = currentLightState;
    return returnVal;
}

uint8_t CheckBumps(void) {
    static uint8_t lastBumperState = 0;
    uint8_t currentBumperState;
    uint8_t returnVal = FALSE;
    currentBumperState = Roach_ReadBumpers();
    if (currentBumperState != lastBumperState) {  //event detected
        ES_Event ThisEvent;
        ThisEvent.EventType = BUMPCHANGE;
        ThisEvent.EventParam =  currentBumperState;
        PostRoachFSM(ThisEvent);
        returnVal = TRUE;
    }
    lastBumperState=currentBumperState;
    return returnVal;
}