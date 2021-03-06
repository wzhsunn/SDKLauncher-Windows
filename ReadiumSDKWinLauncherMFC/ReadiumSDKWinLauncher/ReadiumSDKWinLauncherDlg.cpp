// ReadiumSDKWinLauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadiumSDKWinLauncher.h"
#include "ReadiumSDKWinLauncherDlg.h"
#include "afxdialogex.h"

#include "JS2CPP.h"
#include "CPP2JS.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Global Readium-shared-JS -> C++ object
MyDocHostUIHandler	g_readiumJS2Cpp;

// Global C++ object -> Readium-shared-JS
ReadiumJSApi		g_cpp2ReadiumJS;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CReadiumSDKWinLauncherDlg dialog
CReadiumSDKWinLauncherDlg::CReadiumSDKWinLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadiumSDKWinLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_cpp2ReadiumJS.WebBrowser = &m_explorer;
}

void CReadiumSDKWinLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_explorer);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}

BEGIN_MESSAGE_MAP(CReadiumSDKWinLauncherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CReadiumSDKWinLauncherDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CReadiumSDKWinLauncherDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, &CReadiumSDKWinLauncherDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CReadiumSDKWinLauncherDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CReadiumSDKWinLauncherDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CReadiumSDKWinLauncherDlg::OnBnClickedButton5)
	ON_COMMAND(ID_FILE_OPENEPUB3FILE, &CReadiumSDKWinLauncherDlg::OnFileOpenepub3file)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CReadiumSDKWinLauncherDlg::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// CReadiumSDKWinLauncherDlg message handlers

BOOL CReadiumSDKWinLauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//m_tree.SetExtendedStyle(TVS_EX_AUTOHSCROLL, TVS_EX_AUTOHSCROLL);
	//m_tree.setSt
	LONG style = ::GetWindowLong(m_tree.GetSafeHwnd(), GWL_STYLE);
	::SetWindowLong(m_tree.GetSafeHwnd(), GWL_STYLE, style | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS);

#if 1
	// Resizer stuff
	BOOL bOk = FALSE;
	
	bOk = m_resizer.Hook(this);
	ASSERT(bOk);
	
	CUIntArray arrID;
	
	arrID.Add(IDC_MO);
	arrID.Add(IDC_BUTTON1);
	arrID.Add(IDC_BUTTON2);
	arrID.Add(IDC_BUTTON4);
	arrID.Add(IDC_BUTTON3);
	arrID.Add(IDC_BUTTON5);

	bOk = m_resizer.CreatePanel(_T("ButtonPanel"), &arrID, TRUE);
	ASSERT(bOk);
	
	
	arrID.RemoveAll();
	arrID.Add(IDC_TREE1);

	bOk = m_resizer.CreatePanel(_T("LeftPanel"), &arrID, TRUE);
	ASSERT(bOk);
	bOk = m_resizer.SetAnchor(IDC_TREE1, ANCHOR_ALL);

	arrID.RemoveAll();
	arrID.Add(IDC_EXPLORER1);
	bOk = m_resizer.CreatePanel(_T("RightPanel"), &arrID, TRUE);
	ASSERT(bOk);
	bOk = m_resizer.SetAnchor(IDC_EXPLORER1, ANCHOR_ALL);
	ASSERT(bOk);

	bOk = m_resizer.CreateSplitContainer(_T("Splitter"), _T("LeftPanel"), _T("RightPanel"));
	ASSERT(bOk);
	bOk = m_resizer.SetShowSplitterGrip(_T("Splitter"), TRUE);
	ASSERT(bOk);

	bOk = m_resizer.SetAnchor(_T("Splitter"), ANCHOR_ALL);
	ASSERT(bOk);

	//bOk = m_resizer.SetParent(_T("ButtonPanel"), _T("LeftPanel"));
	//ASSERT(bOk);

	bOk = m_resizer.SetAnchor(_T("ButtonPanel"), ANCHOR_BOTTOM | ANCHOR_RIGHT);
	ASSERT(bOk);
	
	m_resizer.SetShowResizeGrip(TRUE);

	bOk = m_resizer.InvokeOnResized();
	ASSERT(bOk);
#endif
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	g_cpp2ReadiumJS.initReadiumSDK();
	m_explorer.Navigate(L"http://localhost:5000/reader.html", NULL, NULL, NULL, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReadiumSDKWinLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReadiumSDKWinLauncherDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReadiumSDKWinLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CReadiumSDKWinLauncherDlg::OnBnClickedButton1()
{
	g_cpp2ReadiumJS.previousMediaOverlay();
}
void CReadiumSDKWinLauncherDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
void CReadiumSDKWinLauncherDlg::OnBnClickedButton3()
{
	g_cpp2ReadiumJS.turnMediaOverlayOff();
	g_cpp2ReadiumJS.openPageLeft();
}
void CReadiumSDKWinLauncherDlg::OnBnClickedButton2()
{
	g_cpp2ReadiumJS.toggleMediaOverlay();
}
void CReadiumSDKWinLauncherDlg::OnBnClickedButton4()
{
	g_cpp2ReadiumJS.nextMediaOverlay();
}
void CReadiumSDKWinLauncherDlg::OnBnClickedButton5()
{
	g_cpp2ReadiumJS.turnMediaOverlayOff();
	g_cpp2ReadiumJS.openPageRight();
}
void CReadiumSDKWinLauncherDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//CString s = m_tree.GetItemText(pNMTreeView->itemNew.hItem);
	HTREEITEM item = pNMTreeView->itemNew.hItem;
	CString s = mapHTREEITEM2CString[item];
	g_cpp2ReadiumJS.turnMediaOverlayOff();
	g_cpp2ReadiumJS.openContentUrl(std::string(CT2CA(s)), "");
	//AfxMessageBox(s);
	//g_cpp2ReadiumJS.openContentUrl("ai2d_d1e1002", "content1.xhtml");
	//g_cpp2ReadiumJS.openContentUrl("chapter_003.xhtml", "");
	//g_cpp2ReadiumJS.openContentUrl("file01.xhtml", "toch_0004");
	*pResult = 0;
}

BEGIN_EVENTSINK_MAP(CReadiumSDKWinLauncherDlg, CDialogEx)
	ON_EVENT(CReadiumSDKWinLauncherDlg, IDC_EXPLORER1, 104, CReadiumSDKWinLauncherDlg::DownloadCompleteExplorer1, VTS_NONE)
END_EVENTSINK_MAP()
void CReadiumSDKWinLauncherDlg::DownloadCompleteExplorer1()
{
	// Install JS->C++ callbacks (g_readiumJS2Cpp)
	ICustomDoc* doc = 0;
	LPDISPATCH disp_doc = m_explorer.get_Document();
	if (disp_doc)
	{
		disp_doc->QueryInterface(IID_ICustomDoc, (void**)&doc);
		if (doc)
		{
			doc->SetUIHandler(&g_readiumJS2Cpp);
			doc->Release();
		}
		else
			AfxMessageBox(L"Error getting Trident's ICustomDoc interface" );
		disp_doc->Release();
	}
	else
		AfxMessageBox(L"Error getting Trident's document interface" );
}
void CReadiumSDKWinLauncherDlg::SetWindowCaption()
{
	std::string sTitle = g_cpp2ReadiumJS.getBookTitle();
	CString cstr(sTitle.c_str());
	SetWindowText((LPCTSTR)(cstr + CString(" - ReadiumWinSDKLauncher")));
}

void CReadiumSDKWinLauncherDlg::OnFileOpenepub3file()
{
	
	CFileDialog fileDialog(1, NULL, L"*.epub");	//������ ������ ������ �����
	int result = fileDialog.DoModal();	//��������� ���������� ����
	if (result == IDOK)	//���� ���� ������
	{
		//AfxMessageBox(fileDialog.GetPathName()); // �������� ������ ����
		
		CT2CA pszConvertedAnsiString(fileDialog.GetPathName());
		std::string path(pszConvertedAnsiString);
		
		try{
		g_cpp2ReadiumJS.on_actionOpen_ePub3(path);
		}
		catch (std::exception& rEx)
		{
			std::string sErrorString(rEx.what());
			CString ss(CA2T(sErrorString.c_str()));

			AfxMessageBox(ss);
			return;
		}
		catch (...)
		{
			AfxMessageBox((LPCTSTR)"Unknown Exception!");
			return;
		}
		
		SetWindowCaption();
		std::list<std::pair<std::string, std::string> > spineList = g_cpp2ReadiumJS.getSpineList();
		// fill up the tree control
		m_tree.DeleteAllItems();
		mapHTREEITEM2CString.clear();
		/*std::list<std::pair<std::string, std::string> >::const_iterator iter = spineList.begin();
		for (; iter != spineList.end(); ++iter)
		{
			std::string s = iter->first;
			std::string s1 = iter->second;
			CString ss(s.c_str());
			CString ss1(s1.c_str());
			HTREEITEM hItem = m_tree.InsertItem(ss);
			mapHTREEITEM2CString[hItem] = ss1;

		}*/

		TOCEntry tocEntry;	g_cpp2ReadiumJS.getTOCList(tocEntry);
		m_tree.DeleteAllItems();
		digInto(tocEntry, 0);

		HTREEITEM item = m_tree.GetFirstVisibleItem();
		m_tree.SelectItem(item);
	}
	
}
void CReadiumSDKWinLauncherDlg::digInto(TOCEntry& tocEntry, HTREEITEM hParent)
{
	for (int i = 0; i < tocEntry.arrChildren.size(); ++i)
	{
		CString ss(tocEntry.arrChildren[i].sTOCName.c_str());
		CString ss1(tocEntry.arrChildren[i].sURI.c_str());

		HTREEITEM hItem = 0;
		if (hParent != 0)
		{
			hItem = m_tree.InsertItem(ss, hParent);
		}
		else
		{
			hItem = m_tree.InsertItem(ss);
		}
		mapHTREEITEM2CString[hItem] = ss1;
		if (tocEntry.arrChildren[i].arrChildren.size())
		{
			digInto(tocEntry.arrChildren[i], hItem);
		}
	}
}

std::map<std::string, std::string>	_fileExtension2MimeMap;
std::string getMime(CString sFilename)
{
	if (_fileExtension2MimeMap.size() == 0)
	{
		_fileExtension2MimeMap["css"] = "text/css";
		_fileExtension2MimeMap["htm"] = "text/html";
		_fileExtension2MimeMap["html"] = "text/html";
		_fileExtension2MimeMap["xml"] = "text/xml";
		_fileExtension2MimeMap["java"] = "text/x-java-source, text/java";
		_fileExtension2MimeMap["txt"] = "text/plain";
		_fileExtension2MimeMap["asc"] = ("text/plain");
		_fileExtension2MimeMap["gif"] = ("image/gif");
		_fileExtension2MimeMap["jpg"] = ("image/jpeg");
		_fileExtension2MimeMap["jpeg"] = ("image/jpeg");
		_fileExtension2MimeMap["png"] = ("image/png");
		_fileExtension2MimeMap["mp3"] = ("audio/mpeg");
		_fileExtension2MimeMap["m3u"] = ("audio/mpeg-url");
		_fileExtension2MimeMap["mp4"] = ("video/mp4");
		_fileExtension2MimeMap["ogv"] = ("video/ogg");
		_fileExtension2MimeMap["flv"] = ("video/x-flv");
		_fileExtension2MimeMap["mov"] = ("video/quicktime");
		_fileExtension2MimeMap["swf"] = ("application/x-shockwave-flash");
		_fileExtension2MimeMap["js"] = ("application/javascript");
		_fileExtension2MimeMap["pdf"] = ("application/pdf");
		_fileExtension2MimeMap["doc"] = ("application/msword");
		_fileExtension2MimeMap["ogg"] = ("application/x-ogg");
		_fileExtension2MimeMap["zip"] = ("application/octet-stream");
		_fileExtension2MimeMap["exe"] = ("application/octet-stream");
		_fileExtension2MimeMap["class"] = ("application/octet-stream");
		_fileExtension2MimeMap["webm"] = ("video/webm");
		_fileExtension2MimeMap["otf"] = "font/opentype";
		_fileExtension2MimeMap["xhtml"] = "application/xhtml+xml";
	}
	CString folderPath;
	int pos = sFilename.ReverseFind('.');
	if (pos != -1)
	{
		folderPath = sFilename.Mid(pos+1);
	}
	
	std::string strStd=CT2CA(folderPath);

	//std::string sResult = _fileExtension2MimeMap[strStd];

	std::map<std::string, std::string>::iterator it = _fileExtension2MimeMap.find(strStd);
	if (it != _fileExtension2MimeMap.end())
		return (it->second);
	
	return "";
}

CString getWebServerPath()
{
	TCHAR szDirectory[MAX_PATH];
	::GetCurrentDirectory(sizeof(szDirectory)-1, szDirectory);
	CString sDirectoryPath(szDirectory); 
	//sDirectoryPath += L"\\readium-shared-js-develop";
	sDirectoryPath += L"\\readium-shared-js-0.10";
	//sDirectoryPath += L"\\readium-shared-js-04-08-14";
	//sDirectoryPath += L"\\readium-shared-js-0.13";
	//sDirectoryPath += L"\\readium-shared-js";
	return sDirectoryPath;
}
bool getResponseStringAndMime(PCSTR rawURL, BYTE** bytes, ULONGLONG* pSize, std::string& mimeTxt)
{
	

	CString path = getWebServerPath();
	CString filePath = path + rawURL;
	CString pureURI(rawURL);
	//AfxMessageBox(pureURI);
	
	mimeTxt = getMime(pureURI);

	CFile  fp1;
	CFileStatus status;
	if (fp1.GetStatus(filePath, status) == TRUE)
	{
		if (fp1.Open(filePath, CFile::modeRead | CFile::typeBinary))
		{
			ULONGLONG dwLength = fp1.GetLength();
			*pSize = dwLength;
			// make room for whole file, plus null
			BYTE *buffer = new BYTE[dwLength*2];
			memset(buffer, 0, dwLength);
			fp1.Read(buffer, dwLength); // read whole file
			*bytes = buffer;
			//delete [] buffer;
			fp1.Close();
			return true;
		}
	}
	return g_cpp2ReadiumJS.getByteResp(std::string(CT2CA(pureURI)), bytes, pSize);;
}