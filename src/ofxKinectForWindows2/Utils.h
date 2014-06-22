#pragma once

#ifndef __func__
#define __func__ __FUNCTION__
#endif
#define OFXKINECTFORWINDOWS2_WARNING ofLogWarning(string(__func__))
#define OFXKINECTFORWINDOWS2_ERROR ofLogError(string(__func__))
#define OFXKINECTFORWINDOWS2_FATAL ofLogFatalError(string(__func__))

#define MAKE(T, ...) shared_ptr<T>(new T(__VA_ARGS__))

#include <string>

using namespace std;

namespace ofxKinectForWindows2 {
	class Exception : public std::exception {
	public:
		Exception(const std::string &);
		const char * what() const override;
	protected:
		string message;
	};

	// Safe release for COM interfaces
	template<class Interface>
	inline void SafeRelease(Interface *& pInterfaceToRelease)
	{
		if (pInterfaceToRelease != NULL)
		{
			pInterfaceToRelease->Release();
			pInterfaceToRelease = NULL;
		}
	}
}