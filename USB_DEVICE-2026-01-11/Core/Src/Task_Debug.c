

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart_bsp.h"

/*ต๗สิศฮฮ๑*/
void Debug_Task_Proc(void const * argument);


void Debug_Task_Proc(void const * argument)
{

  for(;;)
  {
        osDelay(1000);
  }
}
