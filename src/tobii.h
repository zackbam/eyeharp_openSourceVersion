#ifndef _MYTOBII
#define _MYTOBII

#include <EyeX.h>
#include <Windows.h>
//#include <conio.h>
#include <assert.h>
class tobii
{
    public:
        void setup();
		~tobii();
		TX_CONTEXTHANDLE hContext;
		TX_TICKET hConnectionStateChangedTicket;
		TX_TICKET hEventHandlerTicket;
		//TX_HANDLE gdata;
		TX_ASYNCDATACALLBACK cb;
		TX_USERPARAM prm;
		BOOL success;
		static TX_GAZEPOINTDATAEVENTPARAMS eventParams;
		TX_HANDLE hBehavior;
};
BOOL InitializeGlobalInteractorSnapshot(TX_CONTEXTHANDLE hContext);
void TX_CALLCONVENTION OnSnapshotCommitted(TX_CONSTHANDLE hAsyncData, TX_USERPARAM param);
void TX_CALLCONVENTION OnEngineConnectionStateChanged(TX_CONNECTIONSTATE connectionState, TX_USERPARAM userParam);
void TX_CALLCONVENTION HandleEvent(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam);
void OnGazeDataEvent(TX_HANDLE hGazeDataBehavior);
#endif