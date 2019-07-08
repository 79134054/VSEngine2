//这个示例的主要目的演示一个WindowApplication的创建，包括各种消息的回调的使用。读者可以选择创建那种输入输出设备方式，默认是用Windows，还可以选择用Dx的
#include <VSApplication.h>
#include <VSEngineInput.h>
using namespace VSEngine2;
class VSDemoWindowsApplication : public VSWindowApplication
{

	DLCARE_APPLICATION(VSDemoWindowsApplication);
public: 
	VSDemoWindowsApplication();
	~VSDemoWindowsApplication();
	virtual bool PreInitial();
	virtual bool OnInitial();
	virtual bool OnTerminal();
	virtual bool PostUpdate();
	virtual bool PreUpdate();
	virtual bool OnDraw();
	virtual void OnMove(int xPos, int yPos);
	virtual void OnReSize(int iWidth, int iHeight);
	virtual void OnKeyDown(unsigned int uiKey);
	virtual void OnKeyUp(unsigned int uiKey);
	virtual void OnLButtonDown(int xPos, int yPos);
	virtual void OnLButtonUp(int xPos, int yPos);
	virtual void OnRButtonDown(int xPos, int yPos);
	virtual void OnRButtonUp(int xPos, int yPos);
	virtual void OnMButtonDown(int xPos, int yPos);
	virtual void OnMButtonUp(int xPos, int yPos);
	virtual void OnMouseMove(int xPos, int yPos);
	virtual void OnMouseWheel(int xPos, int yPos, int zDet);
};
IMPLEMENT_APPLICATION(VSDemoWindowsApplication);
VSDemoWindowsApplication::VSDemoWindowsApplication()
{

}
VSDemoWindowsApplication::~VSDemoWindowsApplication()
{

}
bool VSDemoWindowsApplication::PreInitial()
{
	VSWindowApplication::PreInitial();
	m_uiInputAPIType = VSEngineInput::IAT_WINDOWS;
	//m_uiInputAPIType = VSEngineInput::IAT_DX;
	return true;
}
bool VSDemoWindowsApplication::OnInitial()
{
	VSWindowApplication::OnInitial();
	if (VSRenderer::ms_pRenderer)
	{
		VSRenderer::ms_pRenderer->SetClearColor(VSColorRGBA(0.0f, 0.0f, 0.0f));
	}
	return true;
}
bool VSDemoWindowsApplication::OnTerminal()
{
	VSWindowApplication::OnTerminal();
	return true;
}
bool VSDemoWindowsApplication::PostUpdate()
{
	VSWindowApplication::PostUpdate();
	return true;
}
bool VSDemoWindowsApplication::PreUpdate()
{
	VSWindowApplication::PreUpdate();
	return true;
}
bool VSDemoWindowsApplication::OnDraw()
{
	VSWindowApplication::OnDraw();
	return true;
}
void VSDemoWindowsApplication::OnMove(int xPos, int yPos)
{

}
void VSDemoWindowsApplication::OnReSize(int iWidth, int iHeight)
{

}
void VSDemoWindowsApplication::OnKeyDown(unsigned int uiKey)
{
	VSApplication::OnKeyDown(uiKey);
	if (uiKey == VSEngineInput::BK_F1)
	{
		ChangeScreenSize(800, 600, true,false);
	}
	VSOutPutDebugString("On Key Down\n");
}
void VSDemoWindowsApplication::OnKeyUp(unsigned int uiKey)
{
	VSOutPutDebugString("On Key Up\n");
}
void VSDemoWindowsApplication::OnLButtonDown(int xPos, int yPos)
{
	VSOutPutDebugString("On LButton Down\n");
}
void VSDemoWindowsApplication::OnLButtonUp(int xPos, int yPos)
{
	VSOutPutDebugString("On LButton Up\n");
}
void VSDemoWindowsApplication::OnRButtonDown(int xPos, int yPos)
{
	VSOutPutDebugString("On RButton Down\n");
}
void VSDemoWindowsApplication::OnRButtonUp(int xPos, int yPos)
{
	VSOutPutDebugString("On RButton Up\n");
}
void VSDemoWindowsApplication::OnMButtonDown(int xPos, int yPos)
{
	VSOutPutDebugString("On MButton Down\n");
}
void VSDemoWindowsApplication::OnMButtonUp(int xPos, int yPos)
{
	VSOutPutDebugString("On MButton Up\n");
}
void VSDemoWindowsApplication::OnMouseMove(int xPos, int yPos)
{
	VSOutPutDebugString("On Mouse Move\n");
}
void VSDemoWindowsApplication::OnMouseWheel(int xPos, int yPos, int zDet)
{
	VSOutPutDebugString("On Mouse Wheel\n");
}