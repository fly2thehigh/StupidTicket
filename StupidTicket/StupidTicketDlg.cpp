#include "pch.h"
#include "framework.h"
#include "StupidTicket.h"
#include "afxdialogex.h"
#include "fn.h"
#include "StupidTicketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStupidTicketDlg::CStupidTicketDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STUPIDTICKET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStupidTicketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBX_FROM, m_cbxFrom);
	DDX_Control(pDX, IDC_CBX_TO, m_cbxTo);
	DDX_Control(pDX, IDC_EDT_DATE, m_edtDate);
	DDX_Control(pDX, IDC_LST_DATA, m_lstData);
	DDX_Control(pDX, IDC_BTN_QRY, m_btnQry);
}

BEGIN_MESSAGE_MAP(CStupidTicketDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

BOOL CStupidTicketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	init();

	return TRUE;
}

void CStupidTicketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CStupidTicketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStupidTicketDlg::init()
{
	//resize
	int x = GetSystemMetrics(SM_CXFULLSCREEN);
	int y = GetSystemMetrics(SM_CYFULLSCREEN);
	MoveWindow(50, 30, x-100, y-60);

	//get stations
	update_stations(false);

	UpdateData(FALSE);
}

void CStupidTicketDlg::update_stations(bool force)
{
	m_stations.clear();
	m_cbxFrom.Clear();
	m_cbxTo.Clear();
	query_staions(m_stations, force);
	for (auto s : m_stations)
	{
		m_cbxFrom.AddString(s.name);
		m_cbxTo.AddString(s.name);
	}
	m_cbxFrom.SetCurSel(0);
	m_cbxTo.SetCurSel(0);
}

void CStupidTicketDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED)
		resize();
}

void CStupidTicketDlg::resize()
{
	//UpdateData(TRUE);
	CRect rc, rc1;
	GetClientRect(&rc);

	int x = 0, y = 0;
	//label from
	CWnd* pWnd = this->GetDlgItem(IDC_STC_FROM);
	if (!pWnd)
		return;
	pWnd->GetWindowRect(&rc1);
	x = rc.left + 30;
	y = rc.top + 30;
	pWnd->MoveWindow(x, y, rc1.Width(), rc1.Height());
	x += rc1.Width();
	// cbx from
	m_cbxFrom.GetWindowRect(&rc1);
	x += 10;
	m_cbxFrom.MoveWindow(x, y-3, rc1.Width(), rc1.Height());
	x += rc1.Width();

	//label to
	pWnd = GetDlgItem(IDC_STC_TO);
	pWnd->GetWindowRect(&rc1);
	x += 50;
	pWnd->MoveWindow(x, y, rc1.Width(), rc1.Height());
	x += rc1.Width();
	// cbx to
	m_cbxTo.GetWindowRect(&rc1);
	x += 10;
	m_cbxTo.MoveWindow(x, y-3, rc1.Width(), rc1.Height());
	x += rc1.Width();

	//label date
	pWnd = GetDlgItem(IDC_STC_DATE);
	pWnd->GetWindowRect(&rc1);
	x += 50;
	pWnd->MoveWindow(x, y, rc1.Width(), rc1.Height());
	x += rc1.Width();
	// edit date
	m_edtDate.GetWindowRect(&rc1);
	x += 10;
	m_edtDate.MoveWindow(x, y-3, rc1.Width(), rc1.Height());
	x += rc1.Width();

	//radio btn adult
	pWnd = GetDlgItem(IDC_RD_ADULT);
	pWnd->GetWindowRect(&rc1);
	x += 50;
	pWnd->MoveWindow(x, y, rc1.Width(), rc1.Height());
	x += rc1.Width();

	//radio btn student
	pWnd = GetDlgItem(IDC_RD_STU);
	pWnd->GetWindowRect(&rc1);
	x += 10;
	pWnd->MoveWindow(x, y, rc1.Width(), rc1.Height());
	x += rc1.Width();

	//btn query
	m_btnQry.GetWindowRect(&rc1);
	x = rc.right - rc1.Width() - 30;
	m_btnQry.MoveWindow(x, y, rc1.Width(), rc1.Height());

	//list
	m_lstData.GetWindowRect(&rc1);
	x = rc.left + 30;
	y = rc.top + 70;
	m_lstData.MoveWindow(x, y, rc.Width() - 60, rc.Height() - 90);

	UpdateData(FALSE);
}

void CStupidTicketDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = 960;
	lpMMI->ptMinTrackSize.y = 720;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
