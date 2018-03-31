#ifndef _WINDOWREGISTRATION_H_
#define _WINDOWREGISTRATION_H_
#include "CommonInclude.h"


class wiWindowRegistration
{
public:
#ifndef WINSTORE_SUPPORT
	typedef HWND window_type;
#else
	typedef Windows::UI::Core::CoreWindow^ window_type;
#endif

private:
	window_type window;
	RECT originalRect;

public:
	wiWindowRegistration() :window(nullptr) {}

	window_type GetRegisteredWindow() {
		return window;
	}
	RECT const& GetOriginalRect() const
	{
		return originalRect;
	}

	void RegisterWindow(window_type wnd) {
		window = wnd;
		GetWindowRect(wnd, &originalRect);
	}
	bool IsWindowActive() {
#ifndef WINSTORE_SUPPORT
		HWND fgw = GetForegroundWindow();
		return fgw == window;
#else
		return true;
#endif
	}

	static wiWindowRegistration* GetInstance() {
		static wiWindowRegistration* reg = new wiWindowRegistration;
		return reg;
	}
};

#endif // _WINDOWREGISTRATION_H_
