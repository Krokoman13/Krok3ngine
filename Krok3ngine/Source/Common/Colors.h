
namespace Color {
	static COLORREF GrayScale(BYTE a_value) { return RGB(a_value, a_value, a_value); };
	static COLORREF black = GrayScale(0);
	static COLORREF gray = GrayScale(128);
	static COLORREF white = GrayScale(255);

	static COLORREF grayBackground = GrayScale(36);
}