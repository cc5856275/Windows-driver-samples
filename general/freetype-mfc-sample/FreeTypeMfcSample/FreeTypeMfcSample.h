#pragma once

class CMainFrame;

class CFreeTypeMfcSampleApp : public CWinApp
{
public:
    CFreeTypeMfcSampleApp() noexcept;

    BOOL InitInstance() override;
    int ExitInstance() override;

    DECLARE_MESSAGE_MAP()
};
