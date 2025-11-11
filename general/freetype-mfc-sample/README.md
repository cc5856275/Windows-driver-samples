# FreeType MFC Sample

本示例展示了如何在一个简单的 MFC 窗口程序中，使用 [FreeType](https://freetype.org/) 渲染一行文字 `123456`。

## 准备工作

1. 安装 Visual Studio（建议 2022），并在安装时勾选 **使用 C++ 的桌面开发** 工作负载，其中包含 MFC 支持。
2. 下载并编译或获取预编译版本的 FreeType 库，记录其解压路径，例如 `C:\libs\freetype`。
3. 新建系统环境变量 `FREETYPE_DIR`，指向 FreeType 的根目录，上例中即 `C:\libs\freetype`。目录下应包含 `include` 和 `lib` 子目录。
4. 确保系统字体目录中存在 `arial.ttf`（Windows 默认提供），程序将使用该字体文件进行渲染。

## 构建步骤

1. 在 Visual Studio 中打开 `FreeTypeMfcSample.sln`。
2. 选择 `x64` 平台，`Debug` 或 `Release` 配置之一。
3. 生成解决方案（`生成` -> `生成解决方案`）。
4. 运行程序后，将会看到窗口左上角渲染出的 FreeType 样式的 `123456` 文本。

## 代码要点

- `MainFrm.cpp` 中通过 `FT_Init_FreeType` 和 `FT_New_Face` 初始化 FreeType，并加载 `arial.ttf` 字体。
- `RenderText` 函数遍历字符串中的每个字符，使用 `FT_Load_Char` 加载并光栅化字形，将像素拷贝到 32 位位图缓冲区。
- 渲染完成后，利用 `SetDIBitsToDevice` 将像素数据绘制到 MFC 窗口的设备上下文中。
- 通过预编译头 `pch.h` 引入 MFC 及标准库头文件，确保编译效率。

如需替换字体，只需在 `MainFrm.cpp` 的 `fontPath` 变量中指定新的字体文件路径，并确保 FreeType 能够访问该文件。
