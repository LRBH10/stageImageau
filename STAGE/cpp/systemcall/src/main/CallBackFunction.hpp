/*
 * callBackFunction.cpp
 *
 *  Created on: 24 juil. 2012
 *      Author: rli
 */

using namespace systemcall;

template<typename PROTOTYPE>
CallBackFunction<PROTOTYPE>::CallBackFunction (PROTOTYPE t) :
   funct ()
{
   this->funct = boost::bind (t);
}
template<typename PROTOTYPE>
template<typename T1>
CallBackFunction<PROTOTYPE>::CallBackFunction (PROTOTYPE t, T1 arg1) :
   funct ()
{
   this->funct = boost::bind (t, arg1);
}
template<typename PROTOTYPE>
template<typename T1, typename T2>
CallBackFunction<PROTOTYPE>::CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2) :
   funct ()
{
   this->funct = boost::bind (t, arg1, arg2);
}

template<typename PROTOTYPE>
template<typename T1, typename T2, typename T3>
CallBackFunction<PROTOTYPE>::CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2, T3 arg3) :
   funct ()
{
   this->funct = boost::bind (t, arg1, arg2, arg3);
}

template<typename PROTOTYPE>
template<typename T1, typename T2, typename T3, typename T4>
CallBackFunction<PROTOTYPE>::CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2, T3 arg3, T4 arg4) :
   funct ()
{
   this->funct = boost::bind (t, arg1, arg2, arg3, arg4);
}

template<typename PROTOTYPE>
template<typename T1, typename T2, typename T3, typename T4, typename T5>
CallBackFunction<PROTOTYPE>::CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) :
   funct ()
{
   this->funct = boost::bind (t, arg1, arg2, arg3, arg4, arg5);
}

template<typename PROTOTYPE>
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
CallBackFunction<PROTOTYPE>::CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) :
   funct ()
{
   this->funct = boost::bind (t, arg1, arg2, arg3, arg4, arg5, arg6);
}

template<typename PROTOTYPE>
CallBackFunction<PROTOTYPE>::~CallBackFunction ()
{

}

template<typename PROTOTYPE>
void CallBackFunction<PROTOTYPE>::callBack ()
{
   funct ();
}
template<typename PROTOTYPE>
boost::function<void (void)> CallBackFunction<PROTOTYPE>::getfunctionBind ()
{
   return funct;
}

