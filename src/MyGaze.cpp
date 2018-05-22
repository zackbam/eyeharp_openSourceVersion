#include "MyGaze.h"


void MyGaze::setup(){
    // Connect to the server in push mode on the default TCP port (6555)
	if (!m_api.connect()) {
		cout << "Could not connect to EyeTribe" << endl;
		m_api.connect();
	}
}
MyGaze::~MyGaze()
{
    m_api.disconnect();
}

/*void MyGaze::on_gaze_data( gtl::GazeData const & gaze_data )
{
    if( gaze_data.state & gtl::GazeData::GD_STATE_TRACKING_GAZE )
    {
        gtl::Point2D const & smoothedCoordinates = gaze_data.avg;
		avg.x=gaze_data.avg.x;avg.y=gaze_data.avg.y;
        // Move GUI point, do hit-testing, log coordinates, etc.
    }
}
*/