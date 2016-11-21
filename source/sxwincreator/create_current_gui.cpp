

void CreateCursor()
{
	SXMainWndElem::CurSE = LoadCursor(0,IDC_SIZENWSE);
	SXMainWndElem::CurSW = LoadCursor(0,IDC_SIZENESW);
	SXMainWndElem::CurRePos = LoadCursor(0,IDC_SIZEALL);

	SXMainWndElem::CurNS = LoadCursor(0,IDC_SIZENS);
	SXMainWndElem::CurWE = LoadCursor(0,IDC_SIZEWE);
}