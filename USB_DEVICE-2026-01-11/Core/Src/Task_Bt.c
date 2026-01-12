

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/*¿∂—¿»ŒŒÒ*/
void BT_Task_Proc(void const * argument);


void BT_Task_Proc(void const * argument)
{
  for(;;)
  {
        osDelay(1000);
  }
}
