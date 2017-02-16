#pragma once

#include "ChromaSDKImpl.h"

// CARCPage dialog

DWORD WINAPI ThreadFn(LPVOID vpParam);

class CARCPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CARCPage)

public:
	CARCPage();
	virtual ~CARCPage();

	// Dialog Data
	enum { IDD = IDD_ARC_PAGE };

private:
	CChromaSDKImpl m_ChromaSDKImpl;


protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

/*DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonEnableARC();*/

public:
	void InitializeThread();

};
