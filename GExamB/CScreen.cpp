// CScreen.cpp: 구현 파일
//

#include "pch.h"
#include "GExamB.h"
#include "CScreen.h"
#include <iostream>
#include <direct.h> 
using namespace std;


#define COLOR_RED		RGB(0xff, 0x00, 0x00)
#define COLOR_GREEN		RGB(0x00, 0xff, 0x00)
#define COLOR_BLUE		RGB(0x00, 0x00, 0xff)
#define COLOR_WHITE		RGB(0xff, 0xff, 0xff)
#define COLOR_BLACK		RGB(0x00, 0x00, 0x00)
#define COLOR_YELLOW	RGB(0xff, 0xff, 0x00)

#define  MAX_IMAGE_COUNT   10 
#define  MIN_CIRCLE_SIZE   10 

UINT WINAPI MoveCircleAndSaveProc(void* lpOwner);
// CScreen

IMPLEMENT_DYNAMIC(CScreen, CWnd)

CScreen::CScreen()
{

	m_nThresHold = 0x80;
	m_nCircleCnt = MAX_IMAGE_COUNT;
}

CScreen::~CScreen()
{
}


BEGIN_MESSAGE_MAP(CScreen, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CScreen 메시지 처리기

void CScreen::DrawXShape(CDC* pDC, float dCenterX, float dCenterY)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, COLOR_RED);

	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo((int)(dCenterX - 5), (int)dCenterY - 5);
	pDC->LineTo((int)(dCenterX + 5), (int)dCenterY + 5);
	pDC->MoveTo((int)dCenterX - 5, (int)(dCenterY + 5));
	pDC->LineTo((int)dCenterX + 5, (int)(dCenterY - 5));

	CString szText;
	szText.Format(_T("(%3.0f,%3.0f)"), dCenterX, dCenterY);
	pDC->SetTextColor(COLOR_GREEN);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW((int)dCenterX + 10, (int)dCenterY, szText);
	pDC->SelectObject(pOldPen);

}

void CScreen::MakeCircle(int nWidth, int nHeight,CPoint startPos,CPoint endPos)
{
	m_startXY = startPos;
	m_endXY = endPos;
	//바탕화면 생성
	CreateImage(nWidth, nHeight);

	srand((unsigned int)(time(NULL)));
	// 임의의 크기 생성
	int randomRadius = MIN_CIRCLE_SIZE + rand() % 100;
	m_nRadius = min(min(startPos.x, randomRadius), min(startPos.y, randomRadius));
	DrawCircleData(m_startXY.x, m_startXY.y, m_nRadius, 255);

	ImageDisplay();

}

void CScreen::MoveCircleAndSaveThread(int nCircleCount)
{
	UINT dwThreadID = 0;
	m_nCircleCnt = nCircleCount;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, MoveCircleAndSaveProc, this, 0, &dwThreadID);

}

void CScreen::MoveCircleAndSave()
{
	float x, y;
	if (m_nCircleCnt < 1) return;

	float nMoveX = (float)(m_endXY.x - m_startXY.x) / m_nCircleCnt;
	float nMoveY = (float)(m_endXY.y - m_startXY.y) / m_nCircleCnt;
	//TRACE("%03d, %03d\n",nMoveX, nMoveY);
	m_nImageIndex = 0;

	for (int i = 0; i < m_nCircleCnt +1; i++) {
		x = m_startXY.x + nMoveX*i;
		y = m_startXY.y + nMoveY*i;
		//TRACE("%02d: %03.1f, %03.1f\n",i, x, y);
		DrawCircleData((int)x, (int)y, m_nRadius, 255);
		//DrawCenterDisplay();
		Sleep(100);
		SaveCircleData(m_nImageIndex++);
	}
	
}

int CScreen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CPaintDC dc(this);
	if (m_image) {
		m_image.Draw(dc, 0, 0);
	
	}
	return 0;
}

void CScreen::CreateImage(int ww, int hh)
{
	int nWidth = ww;
	int nHeight = hh;
	int nBpp = 8;

	if (m_image)
		m_image.Destroy();
	m_image.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	memset(fm, 0x00, nWidth * nHeight);

}

void CScreen::SaveCircleData(int nImageNo)
{
	CString imagePath;
	TCHAR currentDir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, currentDir);
	
	imagePath.Format(_T("%s\\image"), currentDir);

	if (!IsFolderExist(imagePath)) {
		CreateFolder(imagePath);
	}
		
	CString strFileName;
	strFileName.Format(_T("%s\\SaveImage%03d.bmp"), imagePath.GetString(), nImageNo);

	m_image.Save(strFileName);
}
bool CScreen::IsFolderExist(CString folderPath)
{

	DWORD fileAttr = GetFileAttributes(folderPath);
	if (fileAttr == INVALID_FILE_ATTRIBUTES) {
		return false; 
	}
	return (fileAttr & FILE_ATTRIBUTE_DIRECTORY) != 0;

}
bool CScreen::CreateFolder(CString folderPath)
{
	CT2A asciiFolderPath(folderPath);
	if (_mkdir(asciiFolderPath) == 0) {
		return true;
	}
	return false; 
}

void CScreen::LoadCircleData(CString strFileName)
{
	CPaintDC dc(this);
	
	if (m_image) {
		m_image.Destroy();
	}

	m_image.Load(strFileName);
	DrawCenterDisplay();

}
void CScreen::DrawCenterDisplay()
{
	int nTh = 0;
	int nSumX = 0;
	int nSumY = 0;
	int nCount = 0;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();

	BYTE* fm = (BYTE*)m_image.GetBits();
	CRect rect(0, 0, nWidth, nHeight);
	
	for (int j = rect.top; j < rect.bottom; j++)
	{
		for (int i = rect.left; i < rect.right; i++) {
			if (fm[j * nPitch + i] > nTh)
			{
				nSumX += i;
				nSumY += j;
				nCount++;
			}
		}
	}
	float dCenterX = static_cast<float>(nSumX) / nCount;
	float dCenterY = static_cast<float>(nSumY) / nCount;
	//cout << "X: " << dCenterX << "  Y:" << dCenterY << endl;
	
	ImageDisplay();

	CClientDC dc(this);
	DrawXShape(&dc, dCenterX, dCenterY);
}
void CScreen::ImageDisplay()
{
	CClientDC dc(this);
	if (m_image) {
		m_image.Draw(dc, 0, 0);

	}
}
void CScreen::DrawCircleData(int x, int y, int nRadius, int nGray)
{
	int nIndex = 0;

	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0x00, nWidth * nHeight);

	int nCenterX = x ;
	int nCenterY = y ;
	
	for (int j = y - nRadius; j < y + nRadius ; j++)
	{
		for (int i = x -nRadius; i < x + nRadius ; i++)
		{
			if (IsInCircle(i, j, nCenterX, nCenterY, nRadius)) {
				fm[j * nPitch + i] = nGray;
			}

		}
	}

	ImageDisplay();
}


bool CScreen::IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dX = i - nCenterX;
	double dY = j - nCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius * nRadius) {
		bRet = true;
	}

	return bRet;
}



void CScreen::OnDestroy()
{
	CWnd::OnDestroy();

	m_image.Destroy();
}


BOOL CScreen::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBrush bgBrush(RGB(0, 0, 0));
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->FillRect(&rect, &bgBrush);
	return CWnd::OnEraseBkgnd(pDC);
}

UINT WINAPI MoveCircleAndSaveProc(void* lpOwner)
{

	CScreen* pScreen = (CScreen*)lpOwner;
	pScreen->MoveCircleAndSave();

	return 0;
}

void CScreen::OnPaint()
{

	
}


void CScreen::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonUp(nFlags, point);
}


void CScreen::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnRButtonUp(nFlags, point);
}
