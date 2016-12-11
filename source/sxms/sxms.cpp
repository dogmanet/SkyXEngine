

#if defined(_WINDOWS)
#include <Windows.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	int argc;
	WCHAR ** argv = CommandLineToArgvW(lpCmdLine, &argc);
#else
int main(int argc, char** argv)
{
#endif
	int ret = 0;
	/*
	DSys * sys = new DSys();
	sys->LoadCommandLine(argc, argv);
	sys->Init();
	//sys->CreateContext();
	UINT ret = sys->MainLoop();
	DSAFE_DELETE(sys);*/
	return(ret);
}
