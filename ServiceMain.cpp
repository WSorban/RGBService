#include <Windows.h>
#include <tchar.h>
#include "ChromaSDKImpl.h"

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;


SERVICE_STATUS        g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler (DWORD);
DWORD WINAPI ServiceWorkerThread (LPVOID vpParam);

CChromaSDKImpl m_ChromaSDKImpl;

#define SERVICE_NAME  _T("RzrService")


int _tmain (int argc, TCHAR *argv[])
{
	//Sleep(10000);
    OutputDebugString(_T("My Sample Service: Main: Entry"));

    SERVICE_TABLE_ENTRY ServiceTable[] = 
    {
        {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
        {NULL, NULL}
    };

    if (StartServiceCtrlDispatcher (ServiceTable) == FALSE)
    {
       OutputDebugString(_T("My Sample Service: Main: StartServiceCtrlDispatcher returned error"));
       return GetLastError ();
    }

    OutputDebugString(_T("My Sample Service: Main: Exit"));
    return 0;
}


VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv)
{
    DWORD Status = E_FAIL;

    OutputDebugString(_T("My Sample Service: ServiceMain: Entry"));

    g_StatusHandle = RegisterServiceCtrlHandler (SERVICE_NAME, ServiceCtrlHandler);

    if (g_StatusHandle == NULL) 
    {
        OutputDebugString(_T("My Sample Service: ServiceMain: RegisterServiceCtrlHandler returned error"));
        goto EXIT;
    }

    // Tell the service controller we are starting
    ZeroMemory (&g_ServiceStatus, sizeof (g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE) 
    {
        OutputDebugString(_T("My Sample Service: ServiceMain: SetServiceStatus returned error"));
    }

    /* 
     * Perform tasks neccesary to start the service here
     */
    OutputDebugString(_T("My Sample Service: ServiceMain: Performing Service Start Operations"));

    // Create stop event to wait on later.
    g_ServiceStopEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL) 
    {
        OutputDebugString(_T("My Sample Service: ServiceMain: CreateEvent(g_ServiceStopEvent) returned error"));

        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwWin32ExitCode = GetLastError();
        g_ServiceStatus.dwCheckPoint = 1;

        if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
	    {
		    OutputDebugString(_T("My Sample Service: ServiceMain: SetServiceStatus returned error"));
	    }
        goto EXIT; 
    }    

    // Tell the service controller we are started
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
    {
	    OutputDebugString(_T("My Sample Service: ServiceMain: SetServiceStatus returned error"));
    }
	DWORD qThreadID;
    // Start the thread that will perform the main task of the service
    HANDLE hThread = CreateThread (0, 0, ServiceWorkerThread, &m_ChromaSDKImpl, 0, &qThreadID);

    OutputDebugString(_T("My Sample Service: ServiceMain: Waiting for Worker Thread to complete"));

    // Wait until our worker thread exits effectively signaling that the service needs to stop
    WaitForSingleObject (hThread, INFINITE);
    
    OutputDebugString(_T("My Sample Service: ServiceMain: Worker Thread Stop Event signaled"));
    
    
    /* 
     * Perform any cleanup tasks
     */
    OutputDebugString(_T("My Sample Service: ServiceMain: Performing Cleanup Operations"));

    CloseHandle (g_ServiceStopEvent);

    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 3;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
    {
	    OutputDebugString(_T("My Sample Service: ServiceMain: SetServiceStatus returned error"));
    }
    
    EXIT:
    OutputDebugString(_T("My Sample Service: ServiceMain: Exit"));

    return;
}


VOID WINAPI ServiceCtrlHandler (DWORD CtrlCode)
{
    OutputDebugString(_T("My Sample Service: ServiceCtrlHandler: Entry"));

    switch (CtrlCode) 
	{
     case SERVICE_CONTROL_STOP :

        OutputDebugString(_T("My Sample Service: ServiceCtrlHandler: SERVICE_CONTROL_STOP Request"));

        if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
           break;

        /* 
         * Perform tasks neccesary to stop the service here 
         */
        
        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        g_ServiceStatus.dwWin32ExitCode = 0;
        g_ServiceStatus.dwCheckPoint = 4;

        if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			OutputDebugString(_T("My Sample Service: ServiceCtrlHandler: SetServiceStatus returned error"));
		}

        // This will signal the worker thread to start shutting down
        SetEvent (g_ServiceStopEvent);

        break;

     default:
         break;
    }

    OutputDebugString(_T("My Sample Service: ServiceCtrlHandler: Exit"));
}

DWORD WINAPI ServiceWorkerThread(LPVOID vpParam)
{
	Socket^ socket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
	IPEndPoint^ ipep = gcnew IPEndPoint(IPAddress::Any, 30303);
	bool asusGPUInitialized;
	bool asusMOBOInitialized;

	CChromaSDKImpl m_ChromaSDKImpl = *((CChromaSDKImpl*)vpParam);

	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char DeviceType = 6;
	unsigned char EffectType = 0;
	unsigned char DirectionType = 0;
	unsigned char MessageType = 0;

	m_ChromaSDKImpl.Initialize();
	asusGPUInitialized = m_ChromaSDKImpl.initASUSGPU();
	asusMOBOInitialized = m_ChromaSDKImpl.initASUSMOBO();

	//Add initialization with a welcome!!!
	String^ message = "welcome";
	m_ChromaSDKImpl.PlayMessage(message, 750);

	//apply the initial status
	m_ChromaSDKImpl.SetRazerDeviceColor(DeviceType, GREEN);

	socket->Bind(ipep);
	socket->ReceiveTimeout = 500;

	while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{
		array<unsigned char>^ message = gcnew array<unsigned char>(12);
		EndPoint^ Remote = (EndPoint^) gcnew IPEndPoint(IPAddress::Any, 0);

		try
		{
			int recv = socket->ReceiveFrom(message, Remote);
		}
		catch (SocketException^ e) 
		{
			Sleep(1000);
		}

		MessageType = message[0];

		//if first byte is R, it's a change color command
		if (MessageType == 'R')
		{
			//Valid values are 8 bits, 0-255
			R = ((unsigned char)message[1]);
			G = ((unsigned char)message[3]);
			B = ((unsigned char)message[5]);
			
			//Valid values:
			//1 - Keyboard devices
			//2 - Mousepad devices
			//3 - Mouse devices
			//4 - Headset devices
			//5 - Keypad devices
			//6 - All devices
			DeviceType = ((unsigned char)message[7]);
			
			//apply the status
			m_ChromaSDKImpl.SetRazerDeviceColor(DeviceType, RGB(R, G, B));
			if (asusGPUInitialized)
			{
				m_ChromaSDKImpl.SetGPULights(R, G, B);
			}
			if (asusMOBOInitialized)
			{
				m_ChromaSDKImpl.SetMOBOLights(R, G, B);
			}
		}
		//if first byte is 3, then it's effect mode
		else if (MessageType == 3)
		{
			DeviceType = ((unsigned char)message[1]);
			EffectType = ((unsigned char)message[2]);
			DirectionType = ((unsigned char)message[3]);
			R = ((unsigned char)message[5]);
			G = ((unsigned char)message[7]);
			B = ((unsigned char)message[9]);

			switch (EffectType)
			{
			//Breathing
			case 0:
				m_ChromaSDKImpl.SetRazerDeviceBreathingEffect(DeviceType, RGB(R, G, B));
				break;
			//Reactive
			case 1:
				m_ChromaSDKImpl.SetRazerDeviceReactiveEffect(DeviceType, DirectionType, RGB(R, G, B));
				break;
			//Wave
			case 2:
				m_ChromaSDKImpl.SetRazerDeviceWaveEffect(DeviceType, DirectionType);
				break;
			//Spectrum
			case 3:
				m_ChromaSDKImpl.SetRazerDeviceSpectrumEffect(DeviceType);
				break;
			}
		}
	}
	socket->Close();
	OutputDebugString(_T("My Sample Service: ServiceWorkerThread: Exit"));

	return ERROR_SUCCESS;
}
