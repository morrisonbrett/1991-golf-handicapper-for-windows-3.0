/*
**************************************************************************
print.c	 Brett A. Morrison

This file contains the routines for printing the data.
**************************************************************************
*/

#include "golf.h"

extern char szAppName[];     /* located in golf.c */

/*
--------------------------------------------------------------------------
PrintDlgProc

This function prints a dialog box informing the user that the print job
has started.

Inputs:
  hDlg	  - Handle to the parent window
  message - Message from MainWndProc
  wParam  - Additional message information
  lParam  - Additional message information

Outputs:
  TRUE if successful, else FALSE
--------------------------------------------------------------------------
*/
BOOL FAR PASCAL PrintDlgProc(HWND hDlg, WORD message, WORD wParam, LONG lParam)
{ /* PrintDlgProc */
  return FALSE;
} /* PrintDlgProc */

/*
--------------------------------------------------------------------------
AbortProc

This procedure sends an abort message to the printing queue.

Inputs:
  hPrinterDC - Handle to the printer
  nCode      - Code determining if job can be aborted

Outputs:
  TRUE if job can be aborted, else FALSE
--------------------------------------------------------------------------
*/
BOOL FAR PASCAL AbortProc(HDC hPrinterDC, short nCode)
{ /* AbortProc */
  return FALSE;
} /* AbortProc */

/*
--------------------------------------------------------------------------
PrintFile

This procedure prints the current file.

Inputs:
  hInstance   - Instance of the parent window
  hwnd	      - Handle to the parent window
  *szFileName - The filename to be printed
--------------------------------------------------------------------------
*/
BOOL PrintFile(HANDLE hInstance, HWND hwnd, char *szFileName)
{ /* PrintFile */
  MessageBox(hwnd, "Printing to come soon!", szAppName, MB_OK | MB_ICONEXCLAMATION);
  return FALSE;
} /* PrintFile */
