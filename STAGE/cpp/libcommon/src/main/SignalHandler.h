/**
 * SignalHandler.h
 *
 * $Id: SignalHandler.h 661 2012-07-19 12:10:02Z bdm $
 */

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

/**
 * SignalHandler
 * @since 7 sept. 2011
 * @author bdm
 */
class SignalHandler
{
   public:
      enum SignalState
      {
         SS_QUIT = 0, SS_STOP, SS_RUN
      };

   private:
      static SignalState state;

   public:
      SignalHandler ();
      virtual ~SignalHandler ();

      static void setRunningState (SignalState state);
      static bool stillRunning ();
};

#endif /* SIGNALHANDLER_H_ */
