#include "pch.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()

CMainFrame::CMainFrame() noexcept = default;

CMainFrame::~CMainFrame()
{
    DestroyFreeType();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    if (!InitializeFreeType())
    {
        m_showError = true;
    }

    return 0;
}

void CMainFrame::OnDestroy()
{
    DestroyFreeType();
    CFrameWnd::OnDestroy();
}

void CMainFrame::OnPaint()
{
    CPaintDC dc(this);

    if (m_showError || m_ftFace == nullptr)
    {
        dc.TextOutW(10, 10, _T("FreeType 初始化失败"));
        return;
    }

    const CStringW text(L"123456");
    const UINT pixelHeight = 64;

    const auto rendered = RenderText(text, pixelHeight);
    if (rendered.pixels.empty() || rendered.width <= 0 || rendered.height <= 0)
    {
        dc.TextOutW(10, 10, _T("渲染文本失败"));
        return;
    }

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = rendered.width;
    bmi.bmiHeader.biHeight = -rendered.height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    SetDIBitsToDevice(dc.GetSafeHdc(),
                      10,
                      10,
                      rendered.width,
                      rendered.height,
                      0,
                      0,
                      0,
                      rendered.height,
                      rendered.pixels.data(),
                      &bmi,
                      DIB_RGB_COLORS);
}

bool CMainFrame::InitializeFreeType()
{
    if (FT_Init_FreeType(&m_ftLibrary) != 0)
    {
        return false;
    }

    CStringW fontPath(L"C:\\Windows\\Fonts\\arial.ttf");
    const std::string utf8Path = WideToUtf8(fontPath);
    if (utf8Path.empty())
    {
        return false;
    }

    if (FT_New_Face(m_ftLibrary, utf8Path.c_str(), 0, &m_ftFace) != 0)
    {
        return false;
    }

    if (FT_Set_Pixel_Sizes(m_ftFace, 0, 64) != 0)
    {
        return false;
    }

    return true;
}

void CMainFrame::DestroyFreeType()
{
    if (m_ftFace != nullptr)
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = nullptr;
    }

    if (m_ftLibrary != nullptr)
    {
        FT_Done_FreeType(m_ftLibrary);
        m_ftLibrary = nullptr;
    }
}

FreetypeRenderedText CMainFrame::RenderText(const CStringW& text, UINT pixelHeight)
{
    FreetypeRenderedText result;
    if (m_ftFace == nullptr)
    {
        return result;
    }

    if (FT_Set_Pixel_Sizes(m_ftFace, 0, pixelHeight) != 0)
    {
        return result;
    }

    int penX = 0;
    int maxAboveBaseline = 0;
    int maxBelowBaseline = 0;

    for (int i = 0; i < text.GetLength(); ++i)
    {
        const FT_ULong ch = static_cast<FT_ULong>(text[i]);
        if (FT_Load_Char(m_ftFace, ch, FT_LOAD_RENDER) != 0)
        {
            continue;
        }

        FT_GlyphSlot slot = m_ftFace->glyph;
        maxAboveBaseline = std::max<int>(maxAboveBaseline, slot->bitmap_top);
        const int below = slot->bitmap.rows - slot->bitmap_top;
        maxBelowBaseline = std::max<int>(maxBelowBaseline, below);
        penX += (slot->advance.x >> 6);
    }

    if (penX <= 0 || (maxAboveBaseline + maxBelowBaseline) <= 0)
    {
        return result;
    }

    result.width = penX;
    result.height = maxAboveBaseline + maxBelowBaseline;
    result.pixels.assign(static_cast<size_t>(result.width * result.height), 0xFF000000);

    const int baseline = maxAboveBaseline;
    penX = 0;

    for (int i = 0; i < text.GetLength(); ++i)
    {
        const FT_ULong ch = static_cast<FT_ULong>(text[i]);
        if (FT_Load_Char(m_ftFace, ch, FT_LOAD_RENDER) != 0)
        {
            continue;
        }

        FT_GlyphSlot slot = m_ftFace->glyph;
        const FT_Bitmap& bitmap = slot->bitmap;
        const int top = slot->bitmap_top;
        const int left = slot->bitmap_left;

        for (int row = 0; row < static_cast<int>(bitmap.rows); ++row)
        {
            for (int col = 0; col < static_cast<int>(bitmap.width); ++col)
            {
                const int x = penX + left + col;
                const int y = baseline - top + row;
                if (x < 0 || x >= result.width || y < 0 || y >= result.height)
                {
                    continue;
                }

                const unsigned char value = bitmap.buffer[row * bitmap.pitch + col];
                const UINT32 color = 0xFF000000 | (value << 16) | (value << 8) | value;
                result.pixels[static_cast<size_t>(y) * static_cast<size_t>(result.width) + static_cast<size_t>(x)] = color;
            }
        }

        penX += (slot->advance.x >> 6);
    }

    return result;
}

std::string CMainFrame::WideToUtf8(const CStringW& value) const
{
    if (value.IsEmpty())
    {
        return std::string();
    }

    const int required = WideCharToMultiByte(CP_UTF8, 0, value, -1, nullptr, 0, nullptr, nullptr);
    if (required <= 0)
    {
        return std::string();
    }

    std::string utf8;
    utf8.resize(static_cast<size_t>(required));
    WideCharToMultiByte(CP_UTF8, 0, value, -1, utf8.data(), required, nullptr, nullptr);

    if (!utf8.empty() && utf8.back() == '\0')
    {
        utf8.pop_back();
    }

    return utf8;
}
