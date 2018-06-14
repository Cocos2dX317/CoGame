#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "cocos2d.h"

template <class T>
class CoaSingleton
{
public:
    static T* getInstance();
    static void releaseInstance();
    
protected:
    CoaSingleton(void);
    ~CoaSingleton(void);
    
private:
    static T* s_pInstance;
};

template <class T>
CoaSingleton<T>::CoaSingleton(void)
{
    
};

template<class T>
CoaSingleton<T>::~CoaSingleton(void)
{
};

template <class T>
inline T* CoaSingleton<T>::getInstance()
{
    if(!s_pInstance)
    {
        s_pInstance = new T;
    }
    return s_pInstance;
}

template <class T>
void CoaSingleton<T>::releaseInstance(){
    if(!s_pInstance)
    {
        return;
    }
    delete s_pInstance;
    s_pInstance = NULL;
}

template<class T>
T* CoaSingleton<T>::s_pInstance = NULL;

#endif //__SINGLETON_H__
