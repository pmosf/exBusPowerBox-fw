#include "ch.hpp"
#include "hal.h"
#include "CExPowerBox.hpp"

int main(void)
{
  /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chibios_rt::System::init();

	ExPowerBox::CExPowerBox exPowerBox;
	exPowerBox.start(NORMALPRIO);
	exPowerBox.wait();

	/*while (true)
	{
		chibios_rt::BaseThread::sleep(MS2ST(500));
	}*/

	return 0;
}
