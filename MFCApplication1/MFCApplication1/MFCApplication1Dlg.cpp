
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "HalconCpp.h"
#include "HDevThread.h"

#include <easyx.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace HalconCpp;

HObject  ho_Image;
HTuple  hv_AcqHandle, hv_PointerRed, hv_PointerGreen, hv_PointerBlue, hv_Type, hv_Width, hv_Height;
bool m_kong = TRUE;
BYTE *pr, *pg, *pb, *pimage;
int i, j;
BITMAPINFO *bitmapinfo;
CDC* pDC;
CRect Video;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	bitmapinfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO));

	OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
		"default", "[0] YW500", 0, -1, &hv_AcqHandle);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
//	"default", "[0] YW500", 0, -1, &hv_AcqHandle);
//GrabImageStart(hv_AcqHandle, -1);

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedButton1()
{
	GrabImageStart(hv_AcqHandle, -1);
	SetTimer(1, 20, NULL);
}


void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	GrabImageAsync(&ho_Image, hv_AcqHandle, -1);
	GetImagePointer3(ho_Image, &hv_PointerRed, &hv_PointerGreen, &hv_PointerBlue, &hv_Type, &hv_Width, &hv_Height);

	int height = (Hlong)hv_Height;
	int width = (Hlong)hv_Width;

	if (m_kong)
	{
		pr = new BYTE[width * height];
		pg = new BYTE[width * height];
		pb = new BYTE[width * height];
		pimage = new BYTE[width * height * 3];
		m_kong = FALSE;
	}

	pr = (BYTE*)hv_PointerRed[0].L();
	pg = (BYTE*)hv_PointerGreen[0].L();
	pb = (BYTE*)hv_PointerBlue[0].L();

	int bytewidth = (width * 3 + 3) / 4 * 4;
	IMAGE img;
	for (j = height - 1; j >= 0; j--)
	{
		for (i = 0; i < width; i++)
		{
			*(pimage + j * bytewidth + i * 3 + 0) = *pb;
			*(pimage + j * bytewidth + i * 3 + 1) = *pg;
			*(pimage + j * bytewidth + i * 3 + 2) = *pr;
			pb++;
			pg++;
			pr++;
		}
	}


	BITMAPINFOHEADER *header = &(bitmapinfo->bmiHeader);
	header->biSize = sizeof(BITMAPINFOHEADER);
	header->biWidth = width;
	header->biHeight = height; 
	header->biBitCount = 24;
	header->biCompression = BI_RGB;
	header->biSizeImage = width * height * 3;
	header->biClrImportant = 0;
	header->biClrUsed = 0;
	header->biXPelsPerMeter = 0;
	header->biYPelsPerMeter = 0;
	header->biPlanes = 1;

	pDC = GetDlgItem(IDC_PICT)->GetDC();
	GetDlgItem(IDC_PICT)->GetClientRect(Video);
	SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(pDC->GetSafeHdc(),
		Video.left, Video.top, Video.Width(), Video.Height(),
		0, 0, width, height, pimage, bitmapinfo,
		DIB_RGB_COLORS, SRCCOPY);

	CDialogEx::OnTimer(nIDEvent); 
}


void CAboutDlg::OnClose()
{
	CloseFramegrabber(hv_AcqHandle);

	CDialogEx::OnClose();
}
