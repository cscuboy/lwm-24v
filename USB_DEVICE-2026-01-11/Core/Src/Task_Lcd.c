
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "lcd_ssd1315.h"
#include "Modbus_index.h"
/*显示任务*/
void LCD_Task_Proc(void const * argument);


void LCD_Task_Proc(void const * argument)
{
  /* USER CODE BEGIN LCD_Task_Proc */
    char szLine[64];
    float ssls = 0.012;
    float ljll = 0.03;
      
      // OLED?????
    //开启LED电源
    OLED_VDD_EN(IO_ON);
      
    SSD1315_Init();
      
    //设置显示方向  
    SSD1315_SetDisplayDirection(0,1);  
        

  
  /* Infinite loop */
  for(;;)
  {
    // 读取瞬时流量
    FLOAT flow = emdcb_get_param_by_index(REG_INDEX_INSTANT_FLOW);
  //  dprintf("   - 读取瞬时流量: %.2f\n", flow);
    //SSD1315_Clear();    
    sprintf(szLine,"SSLS:%0.2fm3/h   ",flow);
    SSD1315_PrintString(0, 0, szLine);   
    
    
    // 读取累计流量
    LONG meter,milli;
    //emdcb_get_long_by_index(REG_INDEX_POSITIVE_TOTAL_INT,&pint);
    //emdcb_get_float_by_index(REG_INDEX_POSITIVE_TOTAL_FRAC,&flow); 
    emdcb_get_long_by_index(REG_INDEX_POSITIVE_TOTAL_INT,&meter);//gst_yc_value.ljll_p_m);
     // 设置正向流量小数
    emdcb_get_long_by_index(REG_INDEX_POSITIVE_TOTAL_FRAC,&milli); 
    
   // dprintf("   - 读取瞬时流量: %ld,%.2f\n", pint,&flow);
    
    //sprintf(szLine,"LJLL:%0.2fm3",ljll);
    float summtmi = milli;
    summtmi *= 0.000001;
    summtmi +=meter;
    sprintf(szLine,"SUM:%0.3f        ",summtmi);
    SSD1315_PrintString(1, 0, szLine);
      
    //一条分割线    
    for (int i = 0; i < SSD1315_WIDTH; i += 4) 
    {
        SSD1315_DrawPixel(i, 15, 1);
       // SSD1315_DrawPixel(i, 32, 1);
       // SSD1315_DrawPixel(i, 48, 1);
    }
    // 更新屏幕显示
    SSD1315_Update();
  //  ssls += 0.022;
   // ljll += 0.732;  
    osDelay(1000);
  }

  /* USER CODE END LCD_Task_Proc */
}
