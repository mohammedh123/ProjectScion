#include <iostream>
 
/// Script repris de : http://www.siteduzero.com/tutoriel-3-32923-la-classe-singleton.html
/// Ecrit par : Davidbrcz
 
 
template<typename T> class CSingleton
{
public:
 
    /*
      Cette fonction retourne l'unique instance de l'objet
    */
    static T* GetInstance(void)
    {
        if(singleton==0)
	    {
		singleton = new T;
	    }
        return singleton;
    }
 
    /*
      Cette fonction tue l'unique instance de l'objet
    */
    static void Kill()
    {
        if(singleton!=0)
	    {
		delete singleton ;
		singleton=0;
	    }
    }
 
 
    //un couple d'accesseur / mutateur
    int GetValue()
    {
        return  value;
    }
 
    void SetValue(int val)
    {
	value=val;
    }
 
protected:
 
    int value;
 
    CSingleton() : value(0)
    {
        std::cout<<"Singleton crée"<<std::endl;
    }
 
    virtual ~CSingleton()
    {
        std::cout<<"Singleton détruit"<<std::endl;
    }
 
    static T *singleton;
};
 
template <typename T> T* CSingleton<T>::singleton = 0;