#ifndef FF_SINGLETON_H
#define FF_SINGLETON_H
#include <thread>
#include <mutex>
#include <memory>

namespace ff{

	template<typename T>
	class singleton
	{
	public:
		static T& instance()
		{
			//std::call_once(s_oOnce, singleton<T>::init);
			if(!s_pInstance)
				init();
			return *s_pInstance;
		}
		static std::shared_ptr<T> instance_ptr()
		{
			//std::call_once(s_oOnce, singleton<T>::init);
			if(!s_pInstance)
				init();
			return s_pInstance;
		}
	protected:
		singleton(){}
		singleton(const singleton<T> & ) = delete;
	private:
		static void init()
		{
			s_pInstance = std::shared_ptr<T>(new T());
		}
		
	protected:
		static std::shared_ptr<T>		s_pInstance;
		//static std::once_flag			s_oOnce;
	};
	template <typename T>
	std::shared_ptr<T> singleton<T>::s_pInstance(nullptr);
	//template <typename T>
	//std::once_flag		singleton<T>::s_oOnce;
}//end namespace ff

#endif
