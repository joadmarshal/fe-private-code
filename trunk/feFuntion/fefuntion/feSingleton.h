template
<
typename T
>
class FeSingleton
{
public:
	static T& Instance();
private:
	static void DeleteInstance();
	static T* si_instance;
//	static bool si_destroyed;
};

template
<
typename T
> 
T& FeSingleton<T>::Instance()
{
	if (!si_instance)
	{
		//Guard();
		atexit(DeleteInstance);
		return (new T);
	}
	return *si_instance;
}

static void FeSingleton::DeleteInstance()
{
	if(!si_instance)
	{
		delete si_instance;
		si_instance=NULL;
	}
	
}