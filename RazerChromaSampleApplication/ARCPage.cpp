// KeypadPage.cpp : implementation file
//

#include <string.h>
#include <iostream>
#include "stdafx.h"
#include "RazerChromaSampleApplication.h"
#include "ARCPage.h"
#include "afxdialogex.h"
#include "windows.h"

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Text;

IMPLEMENT_DYNAMIC(CARCPage, CPropertyPage)

HANDLE hThread;

CARCPage::CARCPage()
	: CPropertyPage(CARCPage::IDD)
{
	//InitializeThread();
}

CARCPage::~CARCPage()
{
}

BOOL CARCPage::OnInitDialog()
{
	BOOL Result = CPropertyPage::OnInitDialog();
	
	CheckDlgButton(IDC_RADIO_ALL, BST_CHECKED);
	InitializeThread();

	return Result;
}

/*BEGIN_MESSAGE_MAP(CARCPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE_ARC, &CARCPage::OnBnClickedButtonEnableARC)
END_MESSAGE_MAP()*/

struct ThreadParameters
{
	CWnd* mainFrame;
	CChromaSDKImpl m_ChromaSDKImpl;
};

/*void CARCPage::OnBnClickedButtonEnableARC()
{

	if(GetThreadId(hThread))
	{
		//Set the button text to Enable
		GetDlgItem(IDC_BUTTON_ENABLE_ARC)->
			SendMessage(WM_SETTEXT, 0, (LPARAM)L"Enable");
		TerminateThread(hThread, 0);
		//CloseHandle(hThread);
	}
	else
	{
		//Set the button text to Disable
		GetDlgItem(IDC_BUTTON_ENABLE_ARC)->
			SendMessage(WM_SETTEXT, 0, (LPARAM)L"Disable");
		
		InitializeThread();
	}
}*/

void CARCPage::InitializeThread()
{
	//Pass the thread parameters
	ThreadParameters* tp = new ThreadParameters; //Struct allocated on the heap
	tp->mainFrame = GetDlgItem(IDD_RAZERCHROMASAMPLEAPPLICATION_DIALOG);
	tp->m_ChromaSDKImpl = m_ChromaSDKImpl;
	DWORD qThreadID;

	//start the thread
	hThread = CreateThread(0, 0, ThreadFn, tp, 0, &qThreadID);
}

DWORD WINAPI ThreadFn(LPVOID vpParam)
{
	Socket^ socket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
	IPEndPoint^ ipep = gcnew IPEndPoint(IPAddress::Any, 30303);
	ThreadParameters tp = *((ThreadParameters*) vpParam);

	CWnd* mainFrame = tp.mainFrame;
	CChromaSDKImpl m_ChromaSDKImpl = tp.m_ChromaSDKImpl;

	socket->Bind(ipep);

	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char D = 0;

	while (true)
	{
		array<unsigned char>^ message = gcnew array<unsigned char>(12);
		EndPoint^ Remote = (EndPoint^) gcnew IPEndPoint(IPAddress::Any, 0);

		int recv = socket->ReceiveFrom(message, Remote);

		//disregard any other message than RBG values
		if (message[0] == 'R')
		{
			R = ((unsigned char)message[1]);
			G = ((unsigned char)message[3]);
			B = ((unsigned char)message[5]);
			D = ((unsigned char)message[7]);

			//Build the string of the status
			CString str("R: " + R + " G: " + G + " B: " + B);

			//Display the status

			//GetDLGItem(IDC_OUTPUT)->
				//SendMessage(WM_SETTEXT, 0, (LPARAM)((LPCTSTR)str));
			switch (D)
			{
			case 1:
				//CheckDlgButton(IDC_RADIO_KB, BST_CHECKED);
				break;
			}

			//apply the status
			m_ChromaSDKImpl.ShowColor(MOUSE_DEVICES, RGB(R, G, B));
			m_ChromaSDKImpl.ShowColor(KEYBOARD_DEVICES, RGB(R, G, B));
			m_ChromaSDKImpl.ShowColor(MOUSEMAT_DEVICES, RGB(R, G, B));
		}

		//socket->SendTo(message, recv, SocketFlags::None, Remote);
	}
	return 0;
}
