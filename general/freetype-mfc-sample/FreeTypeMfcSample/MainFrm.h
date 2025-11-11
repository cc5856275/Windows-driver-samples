#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

struct FreetypeRenderedText
{
    std::vector<UINT32> pixels;
    int width = 0;
    int height = 0;
};

class CMainFrame : public CFrameWnd
{
public:
    CMainFrame() noexcept;
    ~CMainFrame() override;

protected:
    DECLARE_DYNAMIC(CMainFrame)
    DECLARE_MESSAGE_MAP()

    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnDestroy();
    void OnPaint();

private:
    bool InitializeFreeType();
    void DestroyFreeType();
    FreetypeRenderedText RenderText(const CStringW& text, UINT pixelHeight);
    std::string WideToUtf8(const CStringW& value) const;

private:
    FT_Library m_ftLibrary = nullptr;
    FT_Face m_ftFace = nullptr;
    bool m_showError = false;
};
