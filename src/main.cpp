
#include "ch.hpp"
#include "hal.h"
#include "ch_test.h"

using namespace chibios_rt;

class SequencerThread: public BaseStaticThread<128> {
private:

protected:
  virtual void main(void) {

    setName("sequencer");
  }

public:
  SequencerThread(const void *p) :
      BaseStaticThread<128>() {

  }
};

/* Static threads instances.*/

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  System::init();

  while (true) {

    BaseThread::sleep(MS2ST(500));
  }

  return 0;
}
