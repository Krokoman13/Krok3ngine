#include "Krok3ngine.h"
#include "Win32Utils.h"

namespace Win32 {
	namespace Utils {
		bool KROK3NGINE_API AddBitmap(const WCHAR* a_szFileName, HDC a_hWinDC, int a_x, int a_y) {
			BITMAP qBitmap;
			HDC hLocalDC = CreateCompatibleDC(a_hWinDC);

			HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, a_szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (hBitmap == NULL) {
				MessageBox(NULL, L"LoadImage Failed", L"Error", MB_OK);
				return false;
			}
			GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));

			SelectObject(hLocalDC, hBitmap);

			if (!BitBlt(a_hWinDC, a_x, a_y, qBitmap.bmWidth, qBitmap.bmHeight, hLocalDC, 0, 0, SRCCOPY)) {
				MessageBox(NULL, L"Blit Failed", L"Error", MB_OK);
				return false;
			}

			::DeleteDC(hLocalDC);
			::DeleteObject(hBitmap);
			return true;
		}
	}
}