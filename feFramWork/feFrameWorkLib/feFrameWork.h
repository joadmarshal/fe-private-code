#include <map>
#include <string>
namespace feFrameWork
{
	class IFComponent
	{
	public:
		virtual void  ComponentCtrl(void *ctrlPtr)=0;
	private:
		virtual const char * ComponentName()=0;
	};
	class CFrameWorkApp
	{
	private:
		CFrameWorkApp &GetFrameWorkApp();
		std::map<std::string,IFComponent *> m_ComponentMap;
		static void framework_main_thread(void *param);
		CFrameWorkApp();
		static CFrameWorkApp *_pApp;
	public:
		//bool 
		bool RegeditComponent(IFComponent *);
		bool UnRegeditComponent(const char *componentName);
	};
	
}