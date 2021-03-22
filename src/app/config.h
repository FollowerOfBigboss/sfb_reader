#ifdef USE_GUI
	#include "gui.h"
	wxIMPLEMENT_APP_NO_MAIN(MyApp);
	#ifdef _WIN32
		#define ENTRY int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
	#else
		#define ENTRY int main(int argc, char* argv[])
	#endif
	#ifdef _WIN32
		#define GetCmdArg(X, num) &X[num]
	#else
		#define GetCmdArg(X, num) X[num]
	#endif
#else
	#ifdef _WIN32
		#define ENTRY int wmain(int argc, wchar_t *argv[])
	#else 
		#define ENTRY int main(int argc, char* argv[])
	#endif
#define GetCmdArg(X, num) X[num]
#endif

#ifdef _WIN32
	#define strCmp wcsncmp
	#define CPT(X) L##X
#else
	#define strCmp strncmp
	#define CPT(X) X
#endif
