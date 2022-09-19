#include "WinMouse.h"

WinMouse::WinMouse()
{
}

WinMouse::~WinMouse()
{
}

void WinMouse::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
	{
    // Sent to a window if the mouse causes the cursor to move within a window
    // and mouse input is not captured.
    case WM_SETCURSOR:
    {
        // Sapphire::Logger::GetInstance().Log("WM_SETCURSOR\n");
        OnSetCursor(lParam);
        break;
    }
    // Sent to a window after it has gained the keyboard focus
    case WM_SETFOCUS:
        // Sapphire::Logger::GetInstance().Log("WM_SETFOCUS\n");
        break;
    // Sent to a window immediately before it loses the keyboard focus.
    case WM_KILLFOCUS:
        // Sapphire::Logger::GetInstance().Log("WM_KILLFOCUS\n");
        break;
    case WM_MOUSEMOVE:
    {
        // Sapphire::Logger::GetInstance().Log("WM_MOUSEMOVE\n");
        OnMouseMove(hwnd, wParam, lParam);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        // Sapphire::Logger::GetInstance().Log("WM_LBUTTONDOWN(%d, %d)\n", xPos, yPos);
        break;
    }
    case WM_MOUSELEAVE:
    {
        // Sapphire::Logger::GetInstance().Log("WM_MOUSELEAVE\n");
        isTrackMouseEventRequired = true;
        break;
    }
    case WM_MOUSEHOVER:
    {
        // Sapphire::Logger::GetInstance().Log("WM_MOUSEHOVER\n");
        isTrackMouseEventRequired = true;
        break;
    }
    case WM_NCMOUSELEAVE:
    {
        // Sapphire::Logger::GetInstance().Log("WM_NCMOUSELEAVE\n");
        isTrackMouseEventRequired = true;
        break;
    }
    case WM_NCMOUSEHOVER:
    {
        // Sapphire::Logger::GetInstance().Log("WM_NCMOUSEHOVER\n");
        isTrackMouseEventRequired = true;
        break;
    }
	default:
		break;
	}
}

void WinMouse::OnMouseMove(const HWND& hwnd, const WPARAM& wParam, LPARAM& lParam)
{
    // Track mouse event
    if (isTrackMouseEventRequired)
    {
        TRACKMOUSEEVENT e = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, HOVER_DEFAULT };
        TrackMouseEvent(&e);
        //Sapphire::Logger::GetInstance().Log("TrackMouseEvent\n");
        isTrackMouseEventRequired = false;
    }

    // Capture the mouse in case the user wants to drag it outside
    if ((wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0)
    {
        // Only release the capture if we really have it
        if (GetCapture() == hwnd)
            ReleaseCapture();
    }
    else if (GetCapture() != hwnd)
    {
        // Set the capture to continue receiving mouse events
        SetCapture(hwnd);
    }

    // Get the client area of the window
    RECT area;
    GetClientRect(hwnd, &area);

    POINTS point = MAKEPOINTS(lParam);
    POINT p = { point.x, point.y };
    ClientToScreen(hwnd, &p);
    // Sapphire::Logger::GetInstance().Log("WM_MOUSEMOVE(%d, %d)\n", point.x - lastPoint.x, point.y - lastPoint.y);
    lastPoint = point;

    // If the cursor is outside the client area...
    if ((point.x < area.left) || (point.x > area.right) || (point.y < area.top) || (point.y > area.bottom))
    {
        // Sapphire::Logger::GetInstance().Log("Outside client area\n");
    }
    else
    {
        // Sapphire::Logger::GetInstance().Log("Inside client area\n");
    }
}

void WinMouse::OnSetCursor(LPARAM& lParam)
{
    // The mouse has moved, if the cursor is in our window we must refresh the cursor
    if (LOWORD(lParam) == HTCLIENT)
    {
        // Sapphire::Logger::GetInstance().Log("The cursor is in our window\n");

        // Sets the cursor shape
        HCURSOR cursor = LoadCursor(nullptr, IDC_CROSS);
        SetCursor(cursor);
    }
}

void WinMouse::Initialize()
{
}

void WinMouse::PostFrame()
{
}

int WinMouse::getMousePositionX()
{
    return mousePosX;
}

int WinMouse::getMousePositionY()
{
    return mousePosY;
}
