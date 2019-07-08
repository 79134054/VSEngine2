#ifndef VSFONTTOOL_H
#define VSFONTTOOL_H
#include "VSApplication.h"
namespace VSEngine2
{

//DestPath Face Size Bold(bool) Italic(bool)
	class VSFontTool : public VSConsoleApplication
	{
	public:
		VSFontTool();
		virtual ~VSFontTool();
		DLCARE_APPLICATION(VSFontTool);
	protected:
		void CreateFontData(HDC hDC);
		void ProcessFont(HWND hWnd, const TCHAR * FontName, int weight,
			int size, DWORD italic);
		VSFontPtr m_pFont;
		VSString m_sPathName;
	public:
		virtual bool PreInitial();
		virtual bool OnInitial();	
		virtual bool OnTerminal();
	};
	IMPLEMENT_APPLICATION(VSFontTool);
	
	VSFontTool::VSFontTool()
	{

	}
	
	VSFontTool::~VSFontTool()
	{
		
		
	}
	bool VSFontTool::OnTerminal()
	{
		if (!VSConsoleApplication::OnTerminal())
		{
			return false;
		}
		m_pFont = NULL;
		return true;
	}
	bool VSFontTool::PreInitial()
	{
		if (!VSConsoleApplication::PreInitial())
		{
			return false;
		}
		m_pFont = NULL;


		return true;
	}
	bool VSFontTool::OnInitial()
	{
		if (!VSConsoleApplication::OnInitial())
		{
			return false;
		}
		m_bIsRunning = false;
		if (m_pCommand)
		{
			
			if (m_pCommand->GetString(_T("DestPath"), m_sPathName))
			{
				VSOutPutDebugString("OutPath : %s\n", m_sPathName.GetBuffer());
				printf("OutPath : %s\n", m_sPathName.GetBuffer());
			}
			else
			{
				return false;
			}

			VSString sFaceType;
			if (m_pCommand->GetString(_T("Face"), sFaceType))
			{
				VSOutPutDebugString("FaceType : %s\n", sFaceType.GetBuffer());
				printf("FaceType : %s\n", sFaceType.GetBuffer());
			}
			else
			{
				return false;
			}
			
			
			
			

			VSString sBold;
			if (m_pCommand->GetString(_T("Bold"), sBold))
			{
				VSOutPutDebugString("Bold : %s\n", sBold.GetBuffer());
				printf("Bold : %s\n", sBold.GetBuffer());
			}
			else
			{
				return false;
			}
			bool bBold = StringToBool(sBold);
			

			VSString sSize;
			if (m_pCommand->GetString(_T("Size"), sSize))
			{
				VSOutPutDebugString("Size : %s\n", sSize.GetBuffer());
				printf("Size : %s\n", sSize.GetBuffer());
			}
			else
			{
				return false;
			}
			unsigned int uiSize = StringToInt(sSize);

			VSString sItalic;
			if (m_pCommand->GetString(_T("Italic"), sItalic))
			{
				VSOutPutDebugString("Italic : %s\n", sItalic.GetBuffer());
				printf("Italic : %s\n", sItalic.GetBuffer());
			}
			else
			{
				return false;
			}
			bool bItalic = StringToBool(sItalic);
			

			unsigned int uiWindowWidth = 256;
			unsigned int uiWindowHeight = 256;
			TCHAR* className = _T("BitmapFontCreator");
			WNDCLASS wc;
			wc.style = CS_OWNDC;
			wc.lpfnWndProc = DefWindowProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = nullptr;
			wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszClassName = className;
			wc.lpszMenuName = NULL;
			RegisterClass(&wc);
			HWND hWnd = CreateWindow(className, className, WS_OVERLAPPEDWINDOW,
				0, 0, uiWindowWidth, uiWindowHeight, NULL, NULL, NULL,
				NULL);

			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);
			DWORD dwWeight = (bBold ? FW_BOLD : FW_REGULAR);
			m_pFont = VS_NEW VSFont(sFaceType.GetBuffer(), uiSize, bBold, bItalic);
			ProcessFont(hWnd, sFaceType.GetBuffer(), dwWeight, uiSize, bItalic);
			VSResourceManager::NewSaveFont(m_pFont, m_sPathName.GetBuffer(),true);
			DestroyWindow(hWnd);
			UnregisterClass(className, NULL);
			return true;
		}
		
		return false;
	}

	void VSFontTool::CreateFontData(HDC hDC)
	{

		VSArray<unsigned char> texels;
		float characterData[VSFont::ASCII_NUM + 1];
		// Get the height of the font.
		TEXTMETRIC metric;
		GetTextMetrics(hDC, &metric);
		int height = (unsigned int)metric.tmHeight;

		// Compute the width needed for all 256 characters, taking into account
		// the extra pixel we place at the left side of each character's image.
		int width = 0;
		for (int i = VSFont::ASCII_START; i <= VSFont::ASCII_END; i++)
		{
			int charWidth;
			GetCharWidth32(hDC, i, i, &charWidth);
			width += (charWidth + 1);
		}
		texels.SetBufferNum(width * height * 4);

		// The character data stores textures coordinates in the x-direction.
		float const dx = 1.0f / width;

		int start = 0;
		characterData[0] = 0.5f*dx;
		RECT r;
		r.left = 0;
		r.top = 0;
		r.right = 4 * metric.tmHeight;
		r.bottom = 2 * metric.tmHeight;
		for (unsigned int i = VSFont::ASCII_START; i <= VSFont::ASCII_END; ++i)
		{
			// Determine how many bytes wide the character will be.
			int charWidth;
			GetCharWidth32(hDC, i, i, &charWidth);

			// Place a blank pixel at the start of each character.
			++charWidth;
			int end = start + charWidth;
			characterData[i + 1 - VSFont::ASCII_START] = (end + 0.5f)*dx;

			// Clear the background.
			FillRect(hDC, &r, nullptr);
			// Draw the character.
			TCHAR cChar = i;
			TextOut(hDC, 1, 0, &cChar, 1);

			// Grab the character's pixel data from the screen and store it in the 
			// appropriate texels.
			for (int row = 0; row < height; ++row)
			{
				for (int col = 0; col < charWidth; ++col)
				{
					COLORREF kColor = GetPixel(hDC, col, row);
					unsigned char Color = GetRValue(kColor);
					texels[(width*row + start + col) * 4 + 0] = Color;
					texels[(width*row + start + col) * 4 + 1] = Color;
					texels[(width*row + start + col) * 4 + 2] = Color;
					texels[(width*row + start + col) * 4 + 3] = Color;
				}
			}
			start += charWidth;
		}

		VS2DTexture *p2DTexture = VS_NEW  VS2DTexture(width, height, VSRenderer::SFT_A8R8G8B8, 1, true);
		p2DTexture->CreateRAMDate();
		VSMemcpy(p2DTexture->GetBuffer(0), texels.GetBuffer(), p2DTexture->GetByteSize(0));
		m_pFont->SetFontData(p2DTexture, characterData);
	}

	void VSFontTool::ProcessFont(HWND hWnd, const TCHAR * FontName, int weight,
		int size, DWORD italic)
	{
		HDC hDC = GetDC(hWnd);
		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkColor(hDC, RGB(255, 255, 255));

		// Create the font.
		HFONT hFont = CreateFont(size, 0, 0, 0, weight, italic, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, VARIABLE_PITCH, FontName);

		HGDIOBJ oldFont = SelectObject(hDC, hFont);

		
		CreateFontData(hDC);
		
		SelectObject(hDC, oldFont);
		DeleteObject(hFont);
		ReleaseDC(hWnd, hDC);
	}

}
#endif