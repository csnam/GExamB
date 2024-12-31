
// GExamBDlg.h: 헤더 파일
//

#pragma once

#include "CScreen.h"

// CGExamBDlg 대화 상자
class CGExamBDlg : public CDialogEx
{
// 생성입니다.
public:
	CGExamBDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.


	CScreen* m_pScreen;
	CRect	 m_scrRect;
	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GEXAMB_DIALOG };
#endif
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.

private:
	bool CheckInputValue();

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		
	void OnBnClickedBtndraw();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnload();
	
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedGroupA();



	
};
