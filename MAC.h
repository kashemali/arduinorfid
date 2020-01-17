#ifndef _TIMER_H
#define _TIMER_H
class Timer
{
    unsigned long startus, startms;
    int _interval;
    int _arg;
   
  private:
    void(*_elapsed)(int);
  public:

    // constructor resets time

    Timer(int interval, void(*elapsed)(void),int arg) {
      _interval = interval;
      _elapsed = elapsed;
      _arg = arg;
      reset ();
    }
    Timer (int interval) {
      _interval = interval;
      reset ();
    };

    // reset time to now
    void reset () {
      startus = micros ();
      startms = millis ();
    };
    void elapsed () {
      bool test=millis () - startms >= _interval;
      if(test){
        _elapsed(_arg);
        
      }
      
    };

};    // end of class Elapsed

#endif
