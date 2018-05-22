#ifndef _MYGAZE
#define _MYGAZE

#include <gazeapi.h>
#include <iostream>
#include "ofMain.h"

class MyGaze
{
    public:
        void setup();
        ~MyGaze();
        // IGazeListener
        //void on_gaze_data( gtl::GazeData const & gaze_data );
        gtl::GazeApi m_api;

};

#endif