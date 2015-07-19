// wincolor.c RHS 1/25/91

#include<windows.h>
#include<stdio.h>
#include<mem.h>
#include<string.h>
#include<stdlib.h>
#include<io.h>

#define SC_USER         (0xF000-1)
#define SC_ABOUT        SC_USER
#define SC_AUTOSAVE     SC_USER-1
#define SC_AUTOLOAD     SC_USER-2
#define SC_AUTOPILOT    SC_USER-3
#define SC_AUTOUPDATE   SC_USER-4

#define WM_MYINIT      (WM_USER + 1)
#define LastChar(s)     (s[strlen(s)-1])

#define MAXWINDOWSPARM  256
#define MAXFILENAME     80

typedef struct _colorrect
    {
    RECT    r;
    DWORD   color;
    BOOL    solidonly;
    } CRECT;

#define MAX_WININICOLORS    19

    // offset of first system color
#define CRECTS_SYSCOLORS    2
    // # of system colors
#define MAX_SYSCOLORS       COLOR_ENDCOLORS+1
    // offset of custom colors
#define CRECTS_CUSTCOLORS   CRECTS_SYSCOLORS+19
    // # of custom colors
#define MAX_CUSTCOLORS      36
    // # of rectangles in the dialog (on-screen)
#define MAX_DLGRECTS        CRECTS_CUSTCOLORS+MAX_CUSTCOLORS
    // offset of first Windows color
#define CRECTS_WINCOLORS    MAX_DLGRECTS

    // size of rectangle array
#define MAXRECTS    MAX_DLGRECTS+19

CRECT CRects[MAXRECTS] =
    {
    { 56, 3, 65, 12, 0x00ffffff, FALSE },          // selected color
    { 4, 24, 46, 62, 0x00ffffff, FALSE },          // work area

    { 183,  110, 203, 117, 0x00ffffff, FALSE },    // scroll-bars
    { 183,  126, 203, 133, 0x00ffffff, FALSE },    // desktop b-ground
    { 70,   124, 90,  131, 0x00ffffff, FALSE },    // active titlebar
    { 70,   134, 90,  141, 0x00ffffff, FALSE },    // inactive titlebar
    { 161,  78,  181, 85,  0x00ffffff, TRUE  },    // menu 
    { 70,   96,  90,  103, 0x00ffffff, TRUE  },    // window background
    { 70,   78,  90,  85,  0x00ffffff, TRUE  },    // frame
    { 161,  69,  181, 76,  0x00ffffff, TRUE  },    // menu text
    { 70,   69,  90,  76,  0x00ffffff, TRUE  },    // window text
    { 70,   87,  90,  94,  0x00ffffff, TRUE  },    // caption text
    { 70,   105, 90,  112, 0x00ffffff, FALSE },    // active border
    { 70,   114, 90,  121, 0x00ffffff, FALSE },    // inactive border
    { 183,  134, 203, 141, 0x00ffffff, FALSE },    // application workspace
    { 161,  87,  181, 94,  0x00ffffff, TRUE  },    // menu highlight
    { 161,  96,  181, 103, 0x00ffffff, TRUE  },    // text in menu highlight
    { 241,  126, 261, 133, 0x00ffffff, FALSE },    // button face
    { 241,  136, 261, 143, 0x00ffffff, FALSE },    // button shadow
    { 183,  118, 203, 125, 0x00ffffff, TRUE  },    // grayed text
    { 241,  116, 261, 123, 0x00ffffff, TRUE  },    // button text

	{ 188, 30, 197, 39, 0x00ffffff, FALSE},        // custom colors
	{ 201, 30, 210, 39, 0x00ffffff, FALSE},
	{ 214, 30, 223, 39, 0x00ffffff, FALSE},
	{ 227, 30, 236, 39, 0x00ffffff, FALSE},
	{ 240, 30, 249, 39, 0x00ffffff, FALSE},
	{ 253, 30, 262, 39, 0x00ffffff, FALSE},
	 
	{ 188, 42, 197, 51, 0x00ffffff, FALSE},
	{ 201, 42, 210, 51, 0x00ffffff, FALSE},
	{ 214, 42, 223, 51, 0x00ffffff, FALSE},
	{ 227, 42, 236, 51, 0x00ffffff, FALSE},
	{ 240, 42, 249, 51, 0x00ffffff, FALSE},
	{ 253, 42, 262, 51, 0x00ffffff, FALSE},
	 
	{ 188, 54, 197, 63, 0x00ffffff, FALSE},
	{ 201, 54, 210, 63, 0x00ffffff, FALSE},
	{ 214, 54, 223, 63, 0x00ffffff, FALSE},
	{ 227, 54, 236, 63, 0x00ffffff, FALSE},
	{ 240, 54, 249, 63, 0x00ffffff, FALSE},
	{ 253, 54, 262, 63, 0x00ffffff, FALSE},
	 
	{ 188, 66, 197, 75, 0x00ffffff, FALSE},
	{ 201, 66, 210, 75, 0x00ffffff, FALSE},
	{ 214, 66, 223, 75, 0x00ffffff, FALSE},
	{ 227, 66, 236, 75, 0x00ffffff, FALSE},
	{ 240, 66, 249, 75, 0x00ffffff, FALSE},
	{ 253, 66, 262, 75, 0x00ffffff, FALSE},
	 
	{ 188, 78, 197, 87, 0x00ffffff, FALSE},
	{ 201, 78, 210, 87, 0x00ffffff, FALSE},
	{ 214, 78, 223, 87, 0x00ffffff, FALSE},
	{ 227, 78, 236, 87, 0x00ffffff, FALSE},
	{ 240, 78, 249, 87, 0x00ffffff, FALSE},
	{ 253, 78, 262, 87, 0x00ffffff, FALSE},
	 
	{ 188, 90, 197, 99, 0x00ffffff, FALSE},
	{ 201, 90, 210, 99, 0x00ffffff, FALSE},
	{ 214, 90, 223, 99, 0x00ffffff, FALSE},
	{ 227, 90, 236, 99, 0x00ffffff, FALSE},
	{ 240, 90, 249, 99, 0x00ffffff, FALSE},
	{ 253, 90, 262, 99, 0x00ffffff, FALSE},

    { 0,0,0,0, 0x00ffffff, FALSE},                         // Windows colors
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE},
    { 0,0,0,0, 0x00ffffff, FALSE}
	     };

int colorIndex[19] =
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };

DWORD OrgColorVals[19];

#define SelectedColor           CRects[0]
#define WorkArea                CRects[1]

static char szCopyRight[] = " WinColor, Copyright (C) Richard Hale Shaw, 1991 ";
static char szWinColor[] = "WinColor";

static char szAbout[] = "About...";
static char szAutoSave[] = "AutoSave";
static char szAutoLoad[] = "AutoLoad";
static char szAutoPilot[] = "AutoPilot";
static char szAutoUpdate[] = "Update WIN.INI";

static char szWinColorIni[] = "WINCOLOR.INI";

BOOL Set = FALSE;
BOOL AutoSave = FALSE;
BOOL AutoLoad = FALSE;
BOOL AutoPilot = FALSE;
BOOL AutoUpdate = FALSE;

char szAppName[MAXFILENAME];
HCURSOR hNormalCursor, hHourGlassCursor;
HANDLE hPrev;
LPSTR CmdLine;
HANDLE hInst;
HICON WinColorIcon;
HWND WinColorDlg;

long FAR PASCAL WndProc(HWND, WORD, WORD, LONG);
HANDLE FAR PASCAL MainWindowDlg(HWND hDlg, unsigned message, WORD wParam,
   LONG lParam);
void UpdateSystemMenu(void);
void SelectColor(LONG lParam);
void PaintWorkArea(LONG lParam);
void _PaintWorkArea(DWORD color, BOOL paintrects);
void _PaintCombo(DWORD color);
void _PaintRect(CRECT *crect, DWORD color);
void DlgPaint(HWND hWnd);
void PaintRect(LONG lParam);
void doubleRECT(RECT *r);
void HScroll(WORD wParam, LONG lParam);
void SetScrollValues(void);
void InitArray(void);
void SetColors(void);
void SaveOrgColors(void);
void RestoreColors(BOOL reset);
void SaveColorsToIni(BOOL changecursor, BOOL savecomponentcolors);
void GetColorsFromIni(void);
BOOL IniFile(char *filename,WORD operation);
void Autoload(void);
void SetColorsFromWinColors(void);
void UpdateWinIni(void);
char *MakeWinDirFileName(char *filename);
void _PaintSolidRect(DWORD color);
BOOL FAR PASCAL DialogProc(HWND hDlg, unsigned message, WORD wParam, LONG lParam);

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
    {
    MSG msg;
    WNDCLASS wndclass;

    hPrev = hPrevInstance;
    hInst = hInstance;
    InitArray();        // initialize arrays and read WINCOLOR.INI
    if(AutoPilot)       // if AutoPilot is on, see if any command-line parameters
        {
        char buf[50];
        
        lstrcpy(buf,lpCmdLine); // copy command-line into buf
        if(!strlen(buf))        // if no command-line parameters...
            {
            SetColorsFromWinColors();  // ...set System Colors and get out
            return 0;
            }
        }

    SaveOrgColors();    // initialize OrgColorVals from System Colors

    if(!hPrevInstance)	
        {
        wndclass.style = CS_DBLCLKS;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;       
        wndclass.cbWndExtra = DLGWINDOWEXTRA;       
        wndclass.hInstance = hInstance;
        wndclass.hIcon = WinColorIcon = LoadIcon(hInstance,szWinColor);
        wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
        wndclass.hbrBackground = COLOR_WINDOW+1; // 0; // GetStockObject(WHITE_BRUSH); 
        wndclass.lpszMenuName =  NULL;
        wndclass.lpszClassName = szWinColor;

        RegisterClass(&wndclass);
        }

    hNormalCursor = LoadCursor(NULL, IDC_ARROW);
    hHourGlassCursor = LoadCursor(NULL, IDC_WAIT);

    WinColorDlg = CreateDialog(hInstance,szWinColor, 0, NULL);
    GetModuleFileName(GetClassWord(WinColorDlg,GCW_HMODULE),
                szAppName, MAXFILENAME-1);
    ShowWindow(WinColorDlg, nCmdShow);

    while(GetMessage(&msg, NULL, 0, 0))
    	{
        if(!WinColorDlg || !IsDialogMessage(WinColorDlg,&msg))
            {
    	    TranslateMessage(&msg);
        	DispatchMessage(&msg);
            }
        }
    return (msg.wParam);
    }


#define LBUTTONDCLICK   1
#define MY_LBUTTON      0xffff
#define SET     253
#define RESTORE 254
#define SAVE    255
#define HELP    256


static HWND hRedScroll, hGreenScroll, hBlueScroll;
int SolidTextControlIds[10] = { 215, 217, 219, 221, 231, 233, 235, 236, 247, 241 };

long FAR PASCAL WndProc(HWND hDlg, unsigned message, WORD wParam,
   LONG lParam)
    {
    static BOOL LB_Dclick = FALSE;
    static LONG templParam;
    static BOOL init = FALSE;
    static HBRUSH SolidTextBrush;

    switch(message)
        {
        case WM_CREATE:
            WinColorDlg = hDlg;
            UpdateSystemMenu();
            SolidTextBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
            SetFocus(GetDlgItem(hDlg, SET));
            break;

        case WM_CTLCOLOR:
            if(HIWORD(lParam) == CTLCOLOR_STATIC)
                {
                int i;
                for(i = 0; i < 10; i++)
                    if(GetDlgCtrlID(LOWORD(lParam)) == SolidTextControlIds[i])
                        {
                        SetBkColor(wParam,GetSysColor(COLOR_WINDOW));
                        SetTextColor(wParam,0x000000ff);
                        UnrealizeObject(SolidTextBrush);
                        return (DWORD)SolidTextBrush;
                        }
                }

            if(!init && HIWORD(lParam) == CTLCOLOR_SCROLLBAR)
                {
                HWND hw = LOWORD(lParam);

                switch(GetWindowWord(hw,GWW_ID))
                    {
                    case 211:
                        hRedScroll = hw;
                        break;
                    case 212:
                        hGreenScroll = hw;
                        break;
                    case 213:
                        hBlueScroll = hw;
                        break;
                    }
                if(hRedScroll && hGreenScroll && hBlueScroll)
                    PostMessage(hDlg,WM_MYINIT,0,0L);
                }
            goto defwindowproc;

        case WM_MYINIT:
            {
            if(!init)
                {
                SetScrollRange(hRedScroll,SB_CTL,0,255,FALSE);
                SetScrollRange(hGreenScroll,SB_CTL,0,255,FALSE);
                SetScrollRange(hBlueScroll,SB_CTL,0,255,FALSE);
                _PaintWorkArea(WorkArea.color, FALSE);
                init = TRUE;
            SetFocus(GetDlgItem(hDlg, SET));
                }
            }
            break;

        case WM_PAINT:
            DlgPaint(hDlg);
            break;

        case WM_QUERYENDSESSION:
        case WM_CLOSE:
            {
            BOOL savecomponentcolors = FALSE;

            if(Set)
                if(AutoSave || (MessageBox(hDlg,"Save Color Changes?",szWinColor,
                    MB_ICONQUESTION | MB_YESNO) == IDYES))
                    {
                    SaveOrgColors();
                    //RestoreColors(FALSE);
                    savecomponentcolors = TRUE;
                    }
            SetCursor(hHourGlassCursor);
            SaveColorsToIni(FALSE,savecomponentcolors);
            if(AutoUpdate)
                UpdateWinIni();
            if(message == WM_QUERYENDSESSION)
                return 1L;
            else                                // WM_CLOSE
                DestroyWindow(hDlg);
            }
            break;

        case WM_DESTROY:
            WinColorDlg = 0;
            SetCursor(hNormalCursor);
            PostQuitMessage(0);
            break;

        case WM_TIMER:                  // timer message
            if(wParam == LBUTTONDCLICK)
                {
                KillTimer(hDlg,LBUTTONDCLICK);
                if(!LB_Dclick)
                    SendMessage(hDlg,WM_LBUTTONDOWN,MY_LBUTTON,templParam);
                else
                    LB_Dclick = FALSE;
                break;
                }
            break;

        case WM_RBUTTONDOWN:
            PaintRect(lParam);
            break;
            
        case WM_LBUTTONDOWN: 
            if(wParam == MY_LBUTTON)
                SelectColor(lParam);
            else
                {
                LB_Dclick = FALSE;
                templParam = lParam;
                SetTimer(hDlg,LBUTTONDCLICK,GetDoubleClickTime(),NULL);
                }
            break;

        case WM_LBUTTONDBLCLK:          // left button Double click
            LB_Dclick = TRUE;
            PaintWorkArea(lParam);
            break;

        case WM_HSCROLL:
            {
            BYTE red,green,blue;

            HScroll(wParam,lParam);
            red = GetScrollPos(hRedScroll,SB_CTL);
            green = GetScrollPos(hGreenScroll,SB_CTL);
            blue = GetScrollPos(hBlueScroll,SB_CTL);
            _PaintCombo(RGB(red,green,blue));
            SetScrollValues();
            }
            break;

        case WM_COMMAND:
//            SetFocus(hDlg);
            switch(wParam)
                {
                case SET:
                    Set = TRUE;
                    SetColors();
                    return 0L;
                case RESTORE:
                    RestoreColors(TRUE);
                    Set = FALSE;
                    return 0L;
                case SAVE:
                    SaveColorsToIni(TRUE,TRUE);
                    return 0L;
                case HELP:
                    DialogBox(hInst, "WinColorHelp", hDlg, (FARPROC)DialogProc);
                    return 0L;
                default:
                    if(HIWORD(lParam) == EN_SETFOCUS)
                        {
                        RECT rect;
                        HDC hDC = GetDC(LOWORD(lParam));
                        GetClientRect(LOWORD(lParam),&rect);
                        rect.top -= 2;
                        rect.left -= 2;
                        rect.right += 2;
                        rect.bottom += 2;

                        Rectangle(hDC,rect.left,rect.top,rect.right,rect.bottom);
                        ReleaseDC(LOWORD(lParam),hDC);
                        }
                    return 0L;
                }
            break;

        case WM_INITMENU:           // modify the system menu
            CheckMenuItem(GetSystemMenu(WinColorDlg,0),
                SC_AUTOSAVE,
                (MF_BYCOMMAND | (AutoSave ? MF_CHECKED : MF_UNCHECKED)));
            CheckMenuItem(GetSystemMenu(WinColorDlg,0),
                SC_AUTOLOAD,
                (MF_BYCOMMAND | (AutoLoad ? MF_CHECKED : MF_UNCHECKED)));
            CheckMenuItem(GetSystemMenu(WinColorDlg,0),
                SC_AUTOPILOT,
                (MF_BYCOMMAND | (AutoPilot ? MF_CHECKED : MF_UNCHECKED)));
            CheckMenuItem(GetSystemMenu(WinColorDlg,0),
                SC_AUTOUPDATE,
                (MF_BYCOMMAND | (AutoUpdate ? MF_CHECKED : MF_UNCHECKED)));
            break;

        case WM_SYSCOMMAND:
            switch(wParam)
                {
                case SC_ABOUT:
                    DialogBox(hInst, "ABOUTBOX", hDlg, (FARPROC)DialogProc);
                    break;

                case SC_AUTOLOAD:
                    Autoload();
                    break;

                case SC_AUTOSAVE:
                    AutoSave = !AutoSave;
                    CheckMenuItem(GetSystemMenu(WinColorDlg,0),
                        SC_AUTOSAVE,(MF_BYCOMMAND |
                            (AutoSave ? MF_CHECKED : MF_UNCHECKED)));
                    break;;

                case SC_AUTOPILOT:
                    AutoPilot = !AutoPilot;
                    CheckMenuItem(GetSystemMenu(WinColorDlg,0),
                        SC_AUTOPILOT,(MF_BYCOMMAND |
                            (AutoPilot ? MF_CHECKED : MF_UNCHECKED)));
                    break;
                case SC_AUTOUPDATE:
                    AutoUpdate = !AutoUpdate;
                    CheckMenuItem(GetSystemMenu(WinColorDlg,0),
                        SC_AUTOUPDATE,(MF_BYCOMMAND |
                            (AutoUpdate ? MF_CHECKED : MF_UNCHECKED)));
                    break;

                default:
                    goto defwindowproc;
                }
            break;

        default:
defwindowproc:
            return DefWindowProc(hDlg,message,wParam,lParam);
        }
    return 0L;
    }


// saves the current system colors into OrgColorVals
void SaveOrgColors(void)
    {
    int i;

    for(i = 0; i < 19; i++)
        OrgColorVals[i] = GetSysColor(i);
    }

void SetColors(void)
    {
    int i;
    DWORD ColorVals[19];
    CRECT *start = &CRects[CRECTS_SYSCOLORS];

    for(i = 0; i < 19; i++, start++)
        ColorVals[i] = start->color;

    SetSysColors(19,&colorIndex,&ColorVals);
    }

void SetColorsFromWinColors(void)
    {
    int i;
    DWORD ColorVals[19];
    CRECT *start = &CRects[57];

    for(i = 0; i < 19; i++, start++)
        ColorVals[i] = start->color;

    SetSysColors(19,&colorIndex,&ColorVals);
    }

void RestoreColors(BOOL reset)
    {    
    int i;
    CRECT *start = &CRects[CRECTS_SYSCOLORS];

    for(i = 0; i < 19; i++, start++)
        start->color = OrgColorVals[i];

    if(reset)
        SetSysColors(19,&colorIndex,&OrgColorVals);
    }

void SaveColorsToIni(BOOL changecursor, BOOL savecomponentcolors)
    {
    int i,hdl;
    char buf[40], *section = "[WinColor]\r\n";
    char name[144];
    OFSTRUCT OfStruct;

    if(changecursor)
        SetCursor(hHourGlassCursor);

    strcpy(name,szWinColorIni);
    MakeWinDirFileName(name);
    if((hdl = OpenFile(name,&OfStruct,
        OF_CREATE | OF_WRITE | OF_SHARE_DENY_WRITE)) == -1)
        {
        if(changecursor)
            SetCursor(hNormalCursor);
        sprintf(buf,"Unable to update %s",szWinColorIni);
        MessageBox(WinColorDlg,buf,szWinColor,MB_ICONEXCLAMATION);
        return;
        }
    write(hdl,section,strlen(section));
    sprintf(buf,"%s=%d\r\n",szAutoSave,(AutoSave ? 1 : 0));
    write(hdl,buf,strlen(buf));
    sprintf(buf,"%s=%d\r\n",szAutoLoad,(AutoLoad ? 1 : 0));
    write(hdl,buf,strlen(buf));
    sprintf(buf,"%s=%d\r\n",szAutoPilot,(AutoPilot ? 1 : 0));
    write(hdl,buf,strlen(buf));
    sprintf(buf,"%s=%d\r\n",szAutoUpdate,(AutoUpdate ? 1 : 0));
    write(hdl,buf,strlen(buf));

    for(i = 0; i < MAX_DLGRECTS; i++)
        {
        sprintf(buf,"color%02d=%010lu\r\n",i,CRects[i].color);
        write(hdl,buf,strlen(buf));
        }

    for( ; i < MAXRECTS; i++)
        {
        if(savecomponentcolors)
            CRects[i].color = GetSysColor(i-57);
        sprintf(buf,"color%02d=%010lu\r\n",i,CRects[i].color);
        write(hdl,buf,strlen(buf));
        }
    _lclose(hdl);
    SaveOrgColors();
    Set = FALSE;
    if(changecursor)
        SetCursor(hNormalCursor);
    }


BOOL IniFile(char *filename,WORD operation)
    {
    char name[144];
    OFSTRUCT OfStruct;
    int hdl;

    strcpy(name,filename);
    MakeWinDirFileName(name);
    if((hdl = OpenFile(name,&OfStruct,operation)) == -1)
        return FALSE;
    _lclose(hdl);
    return TRUE;
    }

char *MakeWinDirFileName(char *filename)
    {
    char name[20];

    strcpy(name,filename);
    GetWindowsDirectory(filename,144);
    if(LastChar(filename) != '\\')
        strcat(filename,"\\");
    strcat(filename,name);
    return filename;
    }


void GetColorsFromIni(void)
    {    
    int i;
    char key[10], value[15];

    AutoSave = GetPrivateProfileInt(szWinColor,szAutoSave,0,szWinColorIni);
    AutoLoad = GetPrivateProfileInt(szWinColor,szAutoLoad,0,szWinColorIni);
    AutoPilot = GetPrivateProfileInt(szWinColor,szAutoPilot,0,szWinColorIni);
    AutoUpdate = GetPrivateProfileInt(szWinColor,szAutoUpdate,0,szWinColorIni);

    if(IniFile(szWinColorIni,OF_EXIST))
        {
        char def[15];
        DWORD defval;

        SetCursor(hHourGlassCursor);
        for(i = 0; i < MAX_DLGRECTS; i++)
            {
            sprintf(key,"color%02d",i);
            GetPrivateProfileString(szWinColor,key,"01677215",value,
                sizeof(value),szWinColorIni);
            CRects[i].color = atol(value);
            }
        for( ; i < MAXRECTS; i++)
            {
            sprintf(key,"color%02d",i);
//            defval = GetSysColor(i-MAX_DLGRECTS);
            defval = GetSysColor(i-57);
            sprintf(def,"%lu",defval);
            GetPrivateProfileString(szWinColor,key,def,value,
                sizeof(value),szWinColorIni);
            CRects[i].color = atol(value);
            }
        SetColorsFromWinColors();        
        SetCursor(hNormalCursor);
        }        
    SaveOrgColors();
    }

void UpdateSystemMenu(void)
    {
    HMENU hSysMenu = GetSystemMenu(WinColorDlg,0);

    RemoveMenu(hSysMenu, SC_MAXIMIZE, MF_BYCOMMAND);
    RemoveMenu(hSysMenu, SC_SIZE, MF_BYCOMMAND);

    AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSysMenu, MF_STRING, SC_ABOUT, szAbout);
    AppendMenu(hSysMenu, MF_STRING, SC_AUTOSAVE, szAutoSave);
    AppendMenu(hSysMenu, MF_STRING, SC_AUTOLOAD, szAutoLoad);
    AppendMenu(hSysMenu, MF_STRING, SC_AUTOPILOT, szAutoPilot);
    AppendMenu(hSysMenu, MF_STRING, SC_AUTOUPDATE, szAutoUpdate);
    }


// paints the Color Work Area with the color of the rectangle pointed-to
void PaintWorkArea(LONG lParam)
    {    
    POINT point = MAKEPOINT(lParam);
    int i;

    for(i = 1; i < MAX_DLGRECTS; i++)
        if(PtInRect(&CRects[i].r,point))
            {
            _PaintWorkArea(CRects[i].color, TRUE);
            break;
            }
    }

// paints the Color Work area the specified color and sets the scroll-bars
void _PaintWorkArea(DWORD color, BOOL paintrects)
    {
    if(paintrects)
        _PaintCombo(color);
    SetScrollPos(hRedScroll,SB_CTL,GetRValue(color),TRUE);
    SetScrollPos(hGreenScroll,SB_CTL,GetGValue(color),TRUE);
    SetScrollPos(hBlueScroll,SB_CTL,GetBValue(color),TRUE);
    SetScrollValues();
    }


void SetScrollValues(void)
    {
    char temp[5];

    sprintf(temp,"%03d",GetScrollPos(hRedScroll,SB_CTL));
    SetDlgItemText(WinColorDlg,122,temp);

    sprintf(temp,"%03d",GetScrollPos(hGreenScroll,SB_CTL));
    SetDlgItemText(WinColorDlg,123,temp);

    sprintf(temp,"%03d",GetScrollPos(hBlueScroll,SB_CTL));
    SetDlgItemText(WinColorDlg,124,temp);
    }


// paints the Color Work Area and the Selected Color Rectangle
void _PaintCombo(DWORD color)
    {
    _PaintRect(&WorkArea,color);
    _PaintRect(&SelectedColor,color);
    }

// paints the Selected Color Rectange the color of the rectangle pointed-to
void SelectColor(LONG lParam)
    {
    POINT point = MAKEPOINT(lParam);
    int i;

    for(i = 1; i < MAX_DLGRECTS; i++)
        if(PtInRect(&CRects[i].r,point))
            {
            _PaintRect(&SelectedColor,CRects[i].color);
            break;
            }
    }

// paints the pointed to rectangle the selected color
void PaintRect(LONG lParam)
    {
    POINT point = MAKEPOINT(lParam);
    int i;

    for(i = 1; i < MAX_DLGRECTS; i++)
        if(PtInRect(&CRects[i].r,point))
            {
            _PaintRect(&(CRects[i].r),SelectedColor.color);
            break;
            }
    }

// paints a rectangle a color
void _PaintRect(CRECT *crect, DWORD color)
    {
    HBRUSH Old,Temp;
    RECT *rect = &crect->r;
    HDC hDC = GetDC(WinColorDlg);

    Temp = CreateSolidBrush((crect->solidonly ? GetNearestColor(hDC,color) :
        color));
    Old = SelectObject(hDC,Temp);
    Rectangle(hDC,rect->left,rect->top,rect->right,rect->bottom);
    SelectObject(hDC,Old);
    DeleteObject(Temp);
    if(crect == &SelectedColor)
        _PaintSolidRect(color);
    ReleaseDC(WinColorDlg,hDC);
    crect->color = color;
    }

// paints the rectangles in the Dialog
void DlgPaint(HWND hWnd)
    {
    PAINTSTRUCT ps;
    int i;
    HDC hDC = BeginPaint(hWnd,&ps);
    HBRUSH Old, Temp;
    DWORD color;

    SetBkMode(hDC,TRANSPARENT);         // has to be for back ground dithering

    for(i = 0; i < MAX_DLGRECTS; i++)
        {
        color = (CRects[i].solidonly ? GetNearestColor(hDC,CRects[i].color)
                : CRects[i].color);
        Temp = CreateSolidBrush(color);
        Old = SelectObject(hDC,Temp);
        Rectangle(hDC,CRects[i].r.left,CRects[i].r.top,CRects[i].r.right,
            CRects[i].r.bottom);
        SelectObject(hDC,Old);
        DeleteObject(Temp);
        }
    _PaintSolidRect(CRects[0].color);
    EndPaint(hWnd,&ps);
    }

void doubleRECT(RECT *r)
    {
    r->left *= 2;
    r->top *= 2;
    r->right *= 2;
    r->bottom *= 2;
    }

   // processes color-setting horizontal scroll bars
void HScroll(WORD wParam, LONG lParam)
    {
    HWND hsb;
    short color;

    hsb = HIWORD(lParam);
    color = GetScrollPos(hsb,SB_CTL);

    switch(wParam)
        {
        case SB_PAGEDOWN:
            color += 16;       // fall thru
        case SB_LINEDOWN:
            color++;
            break;
        case SB_PAGEUP:
            color -= 16;       // fall thru
        case SB_LINEUP:
            color--;
            break;
        case SB_TOP:
            color = 0;
            break;
        case SB_BOTTOM:
            color = 255;
            break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            color = LOWORD(lParam);
            break;
        }

    SetScrollPos(hsb,SB_CTL,color,TRUE);
    }

char outbuf[MAXWINDOWSPARM];

void Autoload(void)
    {
    char *p;

    GetProfileString("windows","load",NULL,outbuf,sizeof(outbuf));
    strupr(outbuf);

    AutoLoad = !AutoLoad;

        // if Auto Load is selected and app name is not in LOAD= list
    if(AutoLoad && !strstr(outbuf,szWinColor))
        {
        strcpy(outbuf,szAppName);               // put WinColor in buffer
        strcat(outbuf," ");                     // add blank and name to it
        p = &LastChar(outbuf);                  // goto last character
        p++;                                    // set to the NULL
                                                // append LOAD= list
        GetProfileString("windows","load",NULL,p,sizeof(outbuf)-(p-outbuf));
        WriteProfileString("windows","load",outbuf);
        }
        // if Auto Load is not selected and app name is in LOAD= list
    if(!AutoLoad && (p = strstr(outbuf,szAppName)))
        {
        *p = '\0';                              // NULL at start of appname
        p += strlen(szAppName);                 // move past appname
        strcat(outbuf,p);                       // copy everybody up
        WriteProfileString("windows","load",outbuf);
        }
    }


char *WinIniKeys[MAX_WININICOLORS] =
    {
    "Scrollbar",
    "Background",
    "ActiveTitle",
    "InactiveTitle",
    "Menu",
    "Window",
    "WindowFrame",
    "MenuText",
    "WindowText",
    "TitleText",
    "ActiveBorder",
    "InactiveBorder",
    "AppWorkspace",
    "Hilight",
    "HilightText",
    "ButtonFace",
    "ButtonShadow",
    "GrayText",
    "ButtonText"
    };


void UpdateWinIni(void)
    {
    int i;
    char buf[15];

    for(i = 0; i < MAX_WININICOLORS; i++)
        {
        sprintf(buf,"%d %d %d",
            GetRValue(GetSysColor(i)),
            GetGValue(GetSysColor(i)),
            GetBValue(GetSysColor(i)));
        WriteProfileString("colors",WinIniKeys[i],buf);
        }
    }
/*
[colors]
Background=255 128 0
AppWorkspace=255 128 192
Window=255 255 255
WindowText=0 0 0
Menu=0 255 255
MenuText=0 0 0
ActiveTitle=255 0 0
InactiveTitle=128 128 255
TitleText=255 255 255
ActiveBorder=128 0 255
InactiveBorder=255 255 128
WindowFrame=0 0 0
Scrollbar=255 226 114
*/


void _PaintSolidRect(DWORD color)
    {
    static RECT SolidRect = { 132, 6, 150, 24 };
    HDC hDC = GetDC(WinColorDlg);
    HBRUSH Temp, Old;

    DWORD x = GetNearestColor(hDC,color);
    
    Temp = CreateSolidBrush(x); //RGB(red,green,blue));
    Old = SelectObject(hDC,Temp);
    Rectangle(hDC,SolidRect.left,SolidRect.top,SolidRect.right,SolidRect.bottom);
    SelectObject(hDC,Old);
    DeleteObject(Temp);
    ReleaseDC(WinColorDlg,hDC);
    }


BOOL FAR PASCAL DialogProc(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
    {
    switch(message)
        {
        case WM_INITDIALOG:
            return (TRUE);

        case WM_COMMAND:
    	    if(wParam == IDOK || wParam == IDCANCEL)
                {
                EndDialog(hDlg, TRUE);
                return (TRUE);
                }
            break;
        }
    return FALSE;
    }


void InitArray(void)
    {
    int i;
    CRECT *start = &CRects[CRECTS_SYSCOLORS];

    for(i = 0; i < 19; i++, start++)
        {
        start->color = GetSysColor(i);
        doubleRECT(&start->r);
        }
    for(i = 0; i < MAX_CUSTCOLORS; i++, start++)
        doubleRECT(&start->r);

    doubleRECT(&SelectedColor.r);
    doubleRECT(&WorkArea.r);
    GetColorsFromIni();
    }



