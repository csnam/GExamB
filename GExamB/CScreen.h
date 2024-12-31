#pragma once


// CScreen
#define MAX_POINT	20
#include <iostream>
class CScreen : public CWnd
{
	DECLARE_DYNAMIC(CScreen)

private:

	CImage  m_image;

	CPoint  m_startXY;
	CPoint  m_endXY;
	int		m_nRadius;
	int		m_nImageIndex;
	int		m_nCircleCnt;

	
	int		m_nThresHold;

	
	HANDLE	m_hThread;

	
	void	CreateImage(int nWith, int nHeight);
	void	DrawCircleData(int x, int y, int nRadius, int nGray);
	void	SaveCircleData(int nImageNo);
	bool	IsFolderExist(CString folderPath);
	bool	CreateFolder(CString folderPath);
	
	void	DrawXShape(CDC* pDC, float dCenterX, float dCenterY);
	void	ImageDisplay();
	bool    IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);

	void	DrawCenterDisplay();
	
	
public:
	CScreen();
	virtual ~CScreen();
;
	void	MakeCircle(int nWidth, int nHeight, CPoint startPos, CPoint endPos);
	void	LoadCircleData(CString strFileName);
	void	MoveCircleAndSaveThread(int nCircleCount);
	void	MoveCircleAndSave();;


protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


