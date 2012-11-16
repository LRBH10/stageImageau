#include <stdio.h>
#include <stdarg.h>

float Print( const char* Format, ... )
{
      va_list Arguments;
      va_start(Arguments, Format);
      double FArg;
      int IArg;
      for(int i = 0; Format[i] != '\0'; ++i )
      {
            if (Format[i] == 'f')
            {
                  FArg=va_arg(Arguments, double);
                  printf("Caught a float : %.3lf\n",FArg);
            }
            else if (Format[i] == 'i')
            {
                  IArg=va_arg(Arguments, int);
                  printf("Caught an integer : %d\n",IArg);
            }
      }
      va_end(Arguments);
}

int main()
{
      Print("This is funny, isn't it ?", 1, 2, 12.1200, 3, 4);
      return 0;
}

