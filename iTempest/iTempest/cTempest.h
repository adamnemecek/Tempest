//
//  cTempest.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef cTempest_h
#define cTempest_h

#ifdef __cplusplus
extern "C" {
#endif


    typedef int cTempest;
    typedef void *cVectors;

    typedef enum {
        FIRE,
        PLAYER1,
        PLAYER2,
        ZAP
    } cTempestButton;
    
    cTempest cTempestCreate(void);
    cTempest cTempestNull(void);
    void cTempestDispose(cTempest _tempest);
    cVectors cTempestGetVectors(cTempest _tempest);
    void cTempestMoveSpinner(cTempest _tempest, int offset);
    void cTempestSetButtonState(cTempest tempest, cTempestButton button, int state);

    int cVectorsGetNext(
                 cVectors _vectors,
                 int16_t *startX,
                 int16_t *startY,
                 int16_t *endX,
                 int16_t *endY,
                 uint8_t *r,
                 uint8_t *g,
                 uint8_t *b
                 );
    void cVectorsDispose(cVectors _vectors);

    
#ifdef __cplusplus
};
#endif

        
#endif /* cTempest_h */
