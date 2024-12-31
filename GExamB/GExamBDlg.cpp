
// GExamBDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GExamB.h"
#include "GExamBDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  MIN_START_XY  100
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGExamBDlg 대화 상자



CGExamBDlg::CGExamBDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GEXAMB_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGExamBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGExamBDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnDraw, &CGExamBDlg::OnBnClickedBtndraw)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BtnLoad, &CGExamBDlg::OnBnClickedBtnload)
	ON_BN_CLICKED(IDC_BtnAction, &CGExamBDlg::OnBnClickedBtnAction)
	ON_WM_SIZE()
	
	

	
	
END_MESSAGE_MAP()


// CGExamBDlg 메시지 처리기

BOOL CGExamBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	int nX1 = MIN_START_XY;
	int nY1 = MIN_START_XY;
	int nCircleount = 10;

	CRect rect(0, 0, 720, 480);
	CWnd* pWnd = GetDlgItem(IDC_SCREEN_AREA);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	m_scrRect = rect;

	// 입력값 초기값
	SetDlgItemInt(IDC_EDIT_X1, nX1);
	SetDlgItemInt(IDC_EDIT_Y1, nY1);
	SetDlgItemInt(IDC_EDIT_X2, rect.Width() - MIN_START_XY);
	SetDlgItemInt(IDC_EDIT_Y2, rect.Height() - MIN_START_XY);
	SetDlgItemInt(IDC_EDIT_COUNT, nCircleount);

	pWnd = GetDlgItem(IDC_LBL_SCRINFO);
	CString strText;
	strText.Format(_T("screen size %d X %d"), rect.Width(), rect.Height());
	pWnd->SetWindowText(strText);

	// 스크린 생성
	m_pScreen = new CScreen();
	m_pScreen->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, rect, this, 0);



	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGExamBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGExamBDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGExamBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CGExamBDlg::CheckInputValue()
{
	bool bRet = true;
	int nX1 = GetDlgItemInt(IDC_EDIT_X1);
	int nY1 = GetDlgItemInt(IDC_EDIT_Y1);
	int nX2 = GetDlgItemInt(IDC_EDIT_X2);
	int nY2 = GetDlgItemInt(IDC_EDIT_Y2);

	if (nX1 < MIN_START_XY ||
		nY1 < MIN_START_XY) {
		TRACE(_T("입력값의 법위를 확인하세요 ( x1, y1 > 100 )"));
		return false;
	}
	if (nY2 > (m_scrRect.Height() - MIN_START_XY) ||
		nX2 > (m_scrRect.Width() - MIN_START_XY) ||
		nX2 < nX1 + MIN_START_XY ||
		nY2 < nY1 + MIN_START_XY)
	{
		TRACE(_T("입력값의 법위를 확인하세요 ( x1+100, y1+100 < x2,y2 < 스크린 크기 - 100)"));
		return false;
	}
	return bRet;
}
void CGExamBDlg::OnBnClickedBtndraw()
{
	CRect rect;

	int nX1 = GetDlgItemInt(IDC_EDIT_X1);
	int nY1 = GetDlgItemInt(IDC_EDIT_Y1);
	int nX2 = GetDlgItemInt(IDC_EDIT_X2);
	int nY2 = GetDlgItemInt(IDC_EDIT_Y2);
	
	if (CheckInputValue())
	{
		if (m_pScreen) {
			m_pScreen->GetWindowRect(&rect);
			m_pScreen->MakeCircle(rect.Width(), rect.Height(), CPoint(nX1, nY1), CPoint(nX2, nY2));
		}
	}
}

void CGExamBDlg::OnBnClickedBtnAction()
{
	int nCircleount = GetDlgItemInt(IDC_EDIT_COUNT);

	if (nCircleount > 100 ||  nCircleount < 5) {
		TRACE(_T("5 에서 100 사이의 값을 입력하세요"));
		return;
	}
		
	if (m_pScreen) {
		m_pScreen->MoveCircleAndSaveThread(nCircleount);

	}

}

void CGExamBDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (m_pScreen) {
		delete m_pScreen;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CGExamBDlg::OnBnClickedBtnload()
{
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP,*.PNG *.JPG) | *.BMP;*.PNG;*.JPG |모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.bmp"), _T("image"), OFN_HIDEREADONLY, szFilter);
		
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		TRACE(pathName);
		SetDlgItemText(IDC_LBL_INFO, dlg.GetFileName());
		m_pScreen->LoadCircleData(pathName);
	}
	
}

void CGExamBDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
}






