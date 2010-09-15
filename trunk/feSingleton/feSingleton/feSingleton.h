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
T* FeSingleton<T>::si_instance=0;
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
		si_instance = new T;
	}
	return *si_instance;
}

template
<
typename T
> 
void FeSingleton<T>::DeleteInstance()
{
	if(si_instance)
	{
		delete si_instance;
		si_instance=0;
	}
	
}