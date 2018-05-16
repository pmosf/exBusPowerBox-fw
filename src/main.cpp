#include "ch.hpp"
#include "hal.h"
#include "CExPowerBox.hpp"

int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */

  halInit();
  chibios_rt::System::init();

  SCB_DisableDCache();

  ExPowerBox::CExPowerBox exPowerBox;
  exPowerBox.start(NORMALPRIO);

  while (true) {
    chibios_rt::BaseThread::sleep(TIME_INFINITE);
  }

  return 0;
}
