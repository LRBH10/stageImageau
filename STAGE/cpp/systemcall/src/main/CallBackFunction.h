/*
 * callBackFunction.h
 *
 *  Created on: 24 juil. 2012
 *      Author: rli
 */

#ifndef CALLBACKFUNCTION_H_
#define CALLBACKFUNCTION_H_

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>

using namespace std;

namespace systemcall
{
   /**
    * Template that generate Call Back Function\n
    * Example for utilization :\n
    * @code
    * void callBacktest(MyStruct my_struct, int y , int z);
    * CallBackFunction<void (&)(MyStruct,int,int)> callme (callBacktest, mystruct,y,z);
    * callme.callBack();
    * @endcode
    */
   template<typename PROTOTYPE>
   class CallBackFunction
   {
      public:
         /**
          * Constructor with void
          * @param T function prototype
          */
         CallBackFunction (PROTOTYPE T);

         /**
          * Constructor with 1 argument
          * @param T function prototype
          */
         template<typename T1>
         CallBackFunction (PROTOTYPE t, T1 arg1);

         /**
          * Constructor with 2 arguments
          * @param T function prototype
          */
         template<typename T1, typename T2>
         CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2);

         /**
          * Constructor with 3 arguments
          * @param T function prototype
          */
         template<typename T1, typename T2, typename T3>
         CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2, T3 arg3);

         /**
          * Constructor with 4 arguments
          * @param T function prototype
          */
         template<typename T1, typename T2, typename T3, typename T4>
         CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2, T3 arg3, T4 arg4);

         /**
          * Constructor with 5 arguments
          * @param T function prototype
          */
         template<typename T1, typename T2, typename T3, typename T4, typename T5>
         CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);

         /**
          * Constructor with 6 arguments
          * @param T function prototype
          */
         template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
         CallBackFunction (PROTOTYPE t, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6);

         /**
          * default destructor
          */
         virtual ~CallBackFunction ();

         /**
          * To Call the function
          */
         void callBack ();

         /**
          * To get bind of this function
          * @return boost::function<void (void)>
          */
         boost::function<void (void)> getfunctionBind ();

      private:
         boost::function<void (void)> funct;
   };

#include "CallBackFunction.hpp"
} /* namespace systemcall */
#endif /* CALLBACKFUNCTION_H_ */
