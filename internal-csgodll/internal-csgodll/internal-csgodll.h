// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the INTERNALCSGODLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// INTERNALCSGODLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef INTERNALCSGODLL_EXPORTS
#define INTERNALCSGODLL_API __declspec(dllexport)
#else
#define INTERNALCSGODLL_API __declspec(dllimport)
#endif

// This class is exported from the dll
class INTERNALCSGODLL_API Cinternalcsgodll {
public:
	Cinternalcsgodll(void);
	// TODO: add your methods here.
};

extern INTERNALCSGODLL_API int ninternalcsgodll;

INTERNALCSGODLL_API int fninternalcsgodll(void);
