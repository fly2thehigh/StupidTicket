#pragma once
#include <vector>
#include "tp.h"

class CStupidTicketDlg : public CDialogEx
{
public:
	CStupidTicketDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUPIDTICKET_DIALOG };
#endif

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_cbxFrom;
	CComboBox m_cbxTo;
	CEdit m_edtDate;
	CListCtrl m_lstData;
	CButton m_btnQry;
	std::vector<StationInfo> m_stations;

	void init();
	void update_stations(bool force);
	void resize();
};