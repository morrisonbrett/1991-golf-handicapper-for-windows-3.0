#include "golf.h"

/*
**************************************************************************
edit.c  Brett A. Morrison

This file contains the routines for processing edit box messages.
**************************************************************************
*/
extern char szAppName[];

BOOL CheckEditShort(HWND hDlg, int iControl_Id, char szField_Name[], short *iItem,
		    short min, short max)
{
  BOOL lpTranslated;
  int  iTemp;
  char szTemp[40];
  char szTemp2[3];

  iTemp = GetDlgItemInt(hDlg, iControl_Id, &lpTranslated, FALSE);
  if (!lpTranslated || (!range(iTemp, min, max)))
  {
    lstrcpy(szTemp, szField_Name);
    lstrcat(szTemp, " must be between ");
    lstrcat(szTemp, itoa((int)min, szTemp2, 10));
    lstrcat(szTemp, " and ");
    lstrcat(szTemp, itoa((int)max, szTemp2, 10));
    lstrcat(szTemp, ".");
    MessageBeep(0);
    MessageBox(hDlg, szTemp, szAppName, MB_OK | MB_ICONSTOP);
    SetFocus(GetDlgItem(hDlg, iControl_Id));
    return FALSE;
  } /* if */
  *iItem = (short)iTemp;
  return TRUE;
} /* CheckEdit */