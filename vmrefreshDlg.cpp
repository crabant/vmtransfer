// vmrefreshDlg.cpp : implementation file
//

#include "stdafx.h"
#include "vmrefresh.h"
#include "vmrefreshDlg.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CvmrefreshDlg dialog




CvmrefreshDlg::CvmrefreshDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CvmrefreshDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CvmrefreshDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CvmrefreshDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CvmrefreshDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CvmrefreshDlg::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()


// CvmrefreshDlg message handlers

BOOL CvmrefreshDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CvmrefreshDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CvmrefreshDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CvmrefreshDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CvmrefreshDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
CString CvmrefreshDlg::newGUID()  
{  
	CString res;
	GUID guid;  
	if (S_OK == ::CoCreateGuid(&guid))  
	{  
		res.Format("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"  
			, guid.Data1  
			, guid.Data2  
			, guid.Data3  
			, guid.Data4[0], guid.Data4[1]  
		, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]  
		, guid.Data4[6], guid.Data4[7]  
		);  
	}  
	return res;
}  

CString CvmrefreshDlg::newMac()  
{  
	CString res;
	static char buf[64] = {0};  
	GUID guid;  
	if (S_OK == ::CoCreateGuid(&guid))  
	{  
		res.Format("00:%02X:%02X:%02X:%02X:%02X"  
			, guid.Data4[3], guid.Data4[4], guid.Data4[5]  
		, guid.Data4[6], guid.Data4[7]  
		);  
	}  
	return res; 
}  

void CvmrefreshDlg::updateVM()
{
	const char* FILE_NAME="Debian 6.vmx";
	const char* BAK_FILE_NAME="Debian 6.vmx.bak";
	const char* ETH0_TAG="ethernet0.address =";
	const int ETH0_MAC_LEN=17;
	const int ETH0_MAC_SKIP=8;

	std::vector<char> data;
	FILE* fp=fopen(FILE_NAME,"rb");
	if(NULL==fp)
	{
		CString error;
		error.Format("找不到VM配置文件'%s'",FILE_NAME);
		MessageBox(error,error,MB_OK);
		return;
	}
	fseek(fp,0,SEEK_END);
	int len=ftell(fp);
	if(0>=len)
	{
		fclose(fp);
		CString error;
		error.Format("读取VM配置文件'%s'长度异常",FILE_NAME);
		MessageBox(error,error,MB_OK);

		return;
	}
	fseek(fp,0,SEEK_SET);
	data.resize(len+1);
	if(1!=fread(&data[0],len,1,fp))
	{
		fclose(fp);
		CString error;
		error.Format("读取VM配置文件'%s'失败",FILE_NAME);
		MessageBox(error,error,MB_OK);
		return;
	}
	fclose(fp);

	char* pos=strstr(&data[0],ETH0_TAG);
	if(NULL==pos)
	{
		CString error;
		error.Format("找不到'%s'",ETH0_TAG);
		MessageBox(error,error,MB_OK);
		return;
	}
	pos=strchr(pos,'"');
	if(NULL==pos)
	{
		CString error;
		error.Format("找不到网卡地址开始的'\"'");
			MessageBox(error,error,MB_OK);
		return;
	}
	char* begin=++pos;
	char* end=strchr(begin,'"');
	if(NULL==pos)
	{
		CString error;
		error.Format("找不到网卡地址结束的'\"'");
		MessageBox(error,error,MB_OK);
		return;
	}
	if(ETH0_MAC_LEN!=end-begin)
	{
		CString error;
		error.Format("定位出的网卡地址长度不正确,应该是%d,实际为%d",ETH0_MAC_LEN,(int)(end-begin));
		MessageBox(error,error,MB_OK);
		return;
	}

	CString mac=newMac();
	if(ETH0_MAC_LEN!=mac.GetLength())
	{
		CString error;
		error.Format("生成的新网卡地址长度不正确,应该是%d,实际为%d",ETH0_MAC_LEN,mac.GetLength());
		MessageBox(error,error,MB_OK);
		return;
	}

	memcpy(begin+ETH0_MAC_SKIP,(const char*)mac+ETH0_MAC_SKIP,ETH0_MAC_LEN-ETH0_MAC_SKIP);

	if(!CopyFile(FILE_NAME,BAK_FILE_NAME,FALSE))
	{
		CString error;
		error.Format("生成备份文件'%s'失败",BAK_FILE_NAME);
		MessageBox(error,error,MB_OK);
		return;
	}
	fp=fopen(FILE_NAME,"wb");
	if(NULL==fp)
	{
		CString error;
		error.Format("写入方式打开VM配置文件'%s'失败",FILE_NAME);
		MessageBox(error,error,MB_OK);
		return;
	}
	if(1!=fwrite(&data[0],len,1,fp))
	{
		fclose(fp);
		CString error;
		error.Format("写入VM配置文件'%s'失败,请使用'%s'恢复文件'%s'",FILE_NAME,BAK_FILE_NAME,FILE_NAME);
		MessageBox(error,error,MB_OK);
		return;
	}
	fclose(fp);
}
	
	

void CvmrefreshDlg::OnBnClickedButtonRefresh()
{
	// TODO: Add your control notification handler code here
	updateVM();
}
