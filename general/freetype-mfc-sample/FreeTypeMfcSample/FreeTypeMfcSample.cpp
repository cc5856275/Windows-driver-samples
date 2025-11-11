#include "pch.h"
#include "FreeTypeMfcSample.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFreeTypeMfcSampleApp, CWinApp)
END_MESSAGE_MAP()

CFreeTypeMfcSampleApp::CFreeTypeMfcSampleApp() noexcept
{
    SetAppID(_T("FreeTypeMfcSample.AppID.NoVersion"));
}

CFreeTypeMfcSampleApp theApp;

BOOL CFreeTypeMfcSampleApp::InitInstance()
{
    CWinApp::InitInstance();

    if (!AfxOleInit())
    {
        AfxMessageBox(_T("OLE 初始化失败"));
        return FALSE;
    }

    AfxEnableControlContainer();

    CMainFrame* pFrame = new(std::nothrow) CMainFrame();
    if (pFrame == nullptr)
    {
        AfxMessageBox(_T("无法创建主窗口"));
        return FALSE;
    }

    if (!pFrame->Create(nullptr, _T("FreeType 渲染示例")))
    {
        delete pFrame;
        AfxMessageBox(_T("窗口创建失败"));
        return FALSE;
    }

    m_pMainWnd = pFrame;

    pFrame->ShowWindow(SW_SHOW);
    pFrame->UpdateWindow();
    return TRUE;
}

int CFreeTypeMfcSampleApp::ExitInstance()
{
    AfxOleTerm(FALSE);
    return CWinApp::ExitInstance();
}
