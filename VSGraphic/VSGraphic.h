#ifndef VSGRAPHIC_H
#define VSGRAPHIC_H
#ifdef VSGRAPHIC_EXPORTS
#define  VSGRAPHIC_API __declspec(dllexport) 
#else
#define  VSGRAPHIC_API __declspec(dllimport)
#endif
#define PROFILER
namespace VSEngine2
{
#define TEXLEVEL 16 
	class VSGRAPHIC_API VSEngineFlag
	{
	public:
		static bool IsRunEditor;
		static bool EnableCLODMesh;
	};
}
#endif