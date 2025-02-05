#pragma once

#define DEFAULTWIDTH 800
#define DEFAULTHEIGHT 600

namespace Win32 {
    enum WindowType : DWORD {
        POPUP = WS_POPUP,
        RESIZABLE = WS_SIZEBOX,
        STATIC = WS_OVERLAPPED,
        WINDOWED = WS_OVERLAPPEDWINDOW,

    };

    namespace Utils {
        bool KROK3NGINE_API AddBitmap(const WCHAR* a_szFileName, HDC a_hWinDC, int x = 0, int y = 0);
    
        /// Sets and clears style flags for a particular window.
        inline void KROK3NGINE_API ModifyWindowStyle(HWND a_hWnd, DWORD a_flagsToDisable, DWORD a_flagsToEnable) {
            DWORD style = GetWindowLong(a_hWnd, GWL_STYLE);
            SetWindowLong(a_hWnd, GWL_STYLE, (style & ~a_flagsToDisable) | a_flagsToEnable);
        }

        /// Sets and clears extended style flags for a particular window.
        inline void KROK3NGINE_API ModifyWindowExStyle(HWND a_hWnd, DWORD a_flagsToDisable, DWORD a_flagsToEnable) {
            DWORD exStyle = GetWindowLong(a_hWnd, GWL_EXSTYLE);
            SetWindowLong(a_hWnd, GWL_EXSTYLE, (exStyle & ~a_flagsToDisable) | a_flagsToEnable);
        }


        inline bool KROK3NGINE_API HasStyle(HWND a_hwnd, DWORD a_style) {
            DWORD dwStyle = (DWORD)GetWindowLong(a_hwnd, GWL_STYLE);
            if ((dwStyle & a_style) != 0) return TRUE;
            return FALSE;
        }


        /// Sets and clears style flags for a particular window.
        inline void KROK3NGINE_API ModifyClassStyle(HWND a_hWnd, DWORD a_flagsToDisable, DWORD a_flagsToEnable) {
            DWORD style = GetWindowLong(a_hWnd, GCL_STYLE);
            SetClassLong(a_hWnd, GCL_STYLE, (style & ~a_flagsToDisable) | a_flagsToEnable);
        }

        inline bool KROK3NGINE_API IsWindowFullscreen(HWND a_hWnd) {
            WINDOWPLACEMENT placement;
            GetWindowPlacement(a_hWnd, &placement);

            if (placement.showCmd == SW_MAXIMIZE)
                return TRUE;
            return FALSE;
        }

        inline void KROK3NGINE_API MaximizeWindow(HWND a_hwnd) {
            WINDOWPLACEMENT wPos;
            GetWindowPlacement(a_hwnd, &wPos);
            if (wPos.showCmd == SW_MAXIMIZE) ShowWindow(a_hwnd, SW_NORMAL);
            else ShowWindow(a_hwnd, SW_MAXIMIZE);
        }
    }
}