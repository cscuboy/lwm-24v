#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Task_Adc.h"
#include "kalman.h"
#include "Modbus_index.h"
#include "Modbus_Param.h"
#include "calibration.h"

typedef struct 
{
    float ka; //K系数
    int32_t zero_point; //0点值
    int32_t ka_max; //K系数小流量
    int32_t ka_min; //K系数大流量
    int32_t cut_off;//小流量切断值
    
}st_measure_param;

typedef struct{
    uint32_t ljll_p_ml;  //正累计流量  ML
    uint32_t ljll_p_l;   //正累计流量  L
    uint32_t ljll_p_m;   //正累计流量  M3
    
    uint32_t ljll_n_ml;  //负累计流量  ML
    uint32_t ljll_n_l;   //负累计流量  L
    uint32_t ljll_n_m;   //负累计流量  M3    
    
    float ssll;     //瞬时流量  M3/H
}st_yc_type;

// 累计流量结构体（优化版）
typedef struct {
    // 正累计流量
    uint32_t positive_cubic_meters;  // 整数部分（立方米）
    uint32_t positive_milliliters;   // 小数部分（毫升）
    
    // 负累计流量  
    uint32_t negative_cubic_meters;  // 整数部分（立方米）
    uint32_t negative_milliliters;   // 小数部分（毫升）
    
    // 内部累加器（用于高频累加，避免频繁进位计算）
    float accumulator_ml;          // 累计增量（毫升）
    
    // 转换系数 (瞬时流量m3/h -> 毫升/周期)
    uint32_t conversion_factor_ml_per_cycle;
    
} HighPrecisionFlowTotalizer;


static float get_net_flow_total(const HighPrecisionFlowTotalizer* totalizer);
static void process_flow_carry(HighPrecisionFlowTotalizer* totalizer);
static void update_flow_total_integer(HighPrecisionFlowTotalizer* totalizer, float instant_flow_m3h, uint32_t cycle_ms);
static void flow_totalizer_init(HighPrecisionFlowTotalizer* totalizer);

/*计算任务*/
void CAL_Task_Proc(void const * argument);
static void TreadCalcPrco(HighPrecisionFlowTotalizer* totalizer);
static int32_t GetChaFen(int adcstart, int LC1_AdcCnt, uint16_t *pAdcvalue);
//计算此刻与上一次调用此函数经过的毫秒数
static void GetSystickPast(uint32_t *ulTimeDiffMs);

extern int Get_Lc_Dir(void);

static  int32_t GetCycleSum(int32_t Value);


extern QueueHandle_t xADC_Finish_QueueHandle;
extern QueueHandle_t xCAL_Finish_QueueHandle ;

//ADC 采样数据结构
extern stAdcCfg  AdcRunCfg;

st_measure_param measure_param;//测量的参数
st_yc_type gst_yc_value;


void CAL_Task_Proc(void const * argument)
{
  BaseType_t xResult;
  int tick = 0;
  HighPrecisionFlowTotalizer totalizer;

   measure_param.zero_point = 10525522;//DN50
 // measure_param.zero_point = -572483;
  measure_param.ka = 0.000000314;
  measure_param.ka_max = 0.0011;
  measure_param.ka_min = 0.0011;
  measure_param.cut_off = 2000;
  
  flow_totalizer_init(&totalizer); 
  
  for(;;)
  {
        xResult = xSemaphoreTake(xADC_Finish_QueueHandle, portMAX_DELAY);
        //采集完成了
        if (xResult == pdTRUE)
        {
            TreadCalcPrco(&totalizer);//计算
        }
        //通知计算完成
        xResult = xSemaphoreGive(xCAL_Finish_QueueHandle);
        if (xResult != pdTRUE) 
        {
            // 释放失败处理
            dprintf("启动CAL任务 失败!!\r\n");
        }
        
        // 2. 低频进位处理（如每秒调用1次，避免每次计算）
       // if(tick > 5)
        {
          process_flow_carry(&totalizer);
          tick = 0;
        }
        
        tick++;
       // osDelay(1000);
  }
}


/**
 * @brief 初始化累计流量计算器
 * @param totalizer 计算器实例
 * @param cycle_ms 采集周期（毫秒）
 */
static void flow_totalizer_init(HighPrecisionFlowTotalizer* totalizer ) {
    // 清零所有累计值
    totalizer->positive_cubic_meters = 0;
    totalizer->positive_milliliters = 0;
    totalizer->negative_cubic_meters = 0;
    totalizer->negative_milliliters = 0;
    totalizer->accumulator_ml = 0;
    
    // 计算转换系数: 立方米/小时 -> 毫升/周期
    // 1 m3/h = 1,000,000 ml/h
    // 1小时 = 3,600,000 毫秒
    // 转换系数 = (1,000,000 ml/h) * (cycle_ms / 3,600,000 ms/h)
    // 为避免浮点数，先乘以1000再除以3,600,000
    totalizer->conversion_factor_ml_per_cycle = 0;//(uint32_t)((1000000ULL * cycle_ms) / 3600000);
}

/**
 * @brief 更新累计流量（高效整数版本）
 * @param totalizer 计算器实例
 * @param instant_flow_m3h 瞬时流量（立方米/小时），可正可负
 */
static void update_flow_total_integer(HighPrecisionFlowTotalizer* totalizer, float instant_flow_m3h, uint32_t cycle_ms) 
{
    float flow_increment_ml;
        
    //先乘再除，减少精度损失。
    flow_increment_ml = instant_flow_m3h * 10 * cycle_ms;
    flow_increment_ml /= 36;
    
    // 1. 流量方向判断
    if(instant_flow_m3h >= 0) 
    {
        // 正流量：浮点转整数计算
        // 更新正流量累加器
        totalizer->accumulator_ml += flow_increment_ml;//flow_increment_ml;
    } else 
    {
        // 负流量：取绝对值后计算

        // 更新负流量累加器（累加负值）
        totalizer->accumulator_ml -= flow_increment_ml;//flow_increment_ml;
    }
}

/**
 * @brief 处理进位（可定期调用，如每秒1次）
 * @param totalizer 计算器实例
 */
static void process_flow_carry(HighPrecisionFlowTotalizer* totalizer) 
{
      // 计算整数部分（立方米）和小数部分（毫升）
    // 1立方米 = 1,000,000毫升
    const uint32_t ml_per_cubic_m = 1000000;
     int32_t acc_ml ;
    
    // 根据正负选择不同的累计分量
    uint32_t* cubic_meters;
    uint32_t* milliliters;
    
    if(totalizer->accumulator_ml > 0.0) 
    {
        cubic_meters = &totalizer->positive_cubic_meters;
        milliliters = &totalizer->positive_milliliters;
        acc_ml = (uint32_t)totalizer->accumulator_ml;

    } else 
    {
        cubic_meters = &totalizer->negative_cubic_meters;
        milliliters = &totalizer->negative_milliliters;
        acc_ml = -(int32_t)totalizer->accumulator_ml;// 转换为正数处理
    }
    
    // 更新累计值
    *milliliters = acc_ml;
    // 处理毫升部分的进位,// 更新立方米部分
    if(acc_ml >= ml_per_cubic_m) 
    {
        uint32_t acc_meters = acc_ml / ml_per_cubic_m;
        (*cubic_meters) += acc_meters;
        *milliliters %= ml_per_cubic_m;
        totalizer->accumulator_ml -= acc_meters * ml_per_cubic_m;
    }

}

/**
 * @brief 获取净累计流量（立方米，浮点数，用于显示）
 */
static float get_net_flow_total(const HighPrecisionFlowTotalizer* totalizer) {
    // 计算正负总量（立方米）
    double positive_total = totalizer->positive_cubic_meters + 
                           (double)totalizer->positive_milliliters / 1000000.0;
    
    double negative_total = totalizer->negative_cubic_meters + 
                          (double)totalizer->negative_milliliters / 1000000.0;
    
    return (float)(positive_total - negative_total);
}

// 使用示例
/*
int main() {
    HighPrecisionFlowTotalizer totalizer;
    flow_totalizer_init(&totalizer, 300); // 300ms周期
    
    // 模拟数据采集循环
    while(1) {
        float instant_flow = 12.5f; // 12.5 m3/h
        
        // 1. 高频更新（每个周期调用）
        update_flow_total_integer(&totalizer, instant_flow);
        
        // 2. 低频进位处理（如每秒调用1次，避免每次计算）
        process_flow_carry(&totalizer);
        
        // 模拟延迟
        // delay_ms(300);
    }
    
    return 0;
}*/

static void TreadCalcPrco(HighPrecisionFlowTotalizer* totalizer)
{
    int adcstart;
    
  //  Dprintf("TreadCalcPrco: run\r\n");
    
    uint16_t *pLC1_Adcvalue = AdcRunCfg.pAdcValueBase[0];//第一次励磁数据
    uint32_t LC1_AdcCnt     = AdcRunCfg.AdcCallBackTimes[0] * ADC_DMA_DONE_CYCLE;
      
    uint16_t *pLC2_Adcvalue = AdcRunCfg.pAdcValueBase[1];//第二此励磁数据
    uint32_t LC2_AdcCnt     = AdcRunCfg.AdcCallBackTimes[1] * ADC_DMA_DONE_CYCLE;
    
    uint32_t LC1_CHAFEN = 0;
    uint32_t LC2_CHAFEN = 0;
    
    //跳开后面5%数据
   /* if(LC1_AdcCnt>ADC_SIZE_PER_CHANEL-100)
    {
        LC1_AdcCnt = ADC_SIZE_PER_CHANEL-100;
    }
    if(LC2_AdcCnt>ADC_SIZE_PER_CHANEL-100)
    {
      LC2_AdcCnt = ADC_SIZE_PER_CHANEL-100;
    }
    */
    //计算第一次励磁的ADC差分值
    
    //跳开前面5%数据，目的是在励磁上升沿时的数据不稳定
    adcstart = 0;//1000;//LC1_AdcCnt*5/100;
    if(adcstart&1)
    {
        adcstart +=1;
    }
    LC1_CHAFEN  =GetChaFen(adcstart,LC1_AdcCnt,pLC1_Adcvalue);// (s32)v_1 - (s32)v_2;//(s32)pLC1_Adcvalue[i+1] - (s32)pLC1_Adcvalue[i];
     
    
    //计算第二次励磁的ADC差分值
    adcstart = 0;//1000;//LC2_AdcCnt*5/100;
    if(adcstart&1)
    {
        adcstart +=1;
    }
    LC2_CHAFEN  =GetChaFen(adcstart,LC2_AdcCnt,pLC2_Adcvalue);
    
            
//    int32_t chafen1;
//    int32_t chafen2;
    int32_t  sumvalue = 0;
//    static int32_t  sumvalue_last = 0;//前一次的数据
//    static uint8_t flag_over = 0;
//    static uint8_t tick_over = 0;
    
//    int32_t  liusu;
    float  filter;
    uint32_t timems =0;
    
    static int64_t sum_sum = 0;
    int64_t sum_adver = 0;
    static uint8_t  sum_index = 0;
    static uint8_t sum_int = 10;
    static uint8_t first_cal = 1;
   /* chafen1 = GetCycleChaFenSum(LC1_CHAFEN);
    chafen2 = GetCycleChaFenSum(LC2_CHAFEN);
    
    sumvalue = abs(chafen1) + abs(chafen2);*/
    
   // sumvalue = GetCycleChaFenSum(LC2_CHAFEN - LC1_CHAFEN);
   // sumvalue = GetCycleSum(LC2_CHAFEN - LC1_CHAFEN);
    
    if(0== Get_Lc_Dir())
    {
      sumvalue = GetCycleSum(LC1_CHAFEN - LC2_CHAFEN);
    }
    else
    {
      sumvalue = GetCycleSum(LC2_CHAFEN - LC1_CHAFEN);
    }
   
   
   // printf("CF1:%d CF2:%d DEC%d\r\n",LC1_CHAFEN,LC2_CHAFEN,LC2_CHAFEN - LC1_CHAFEN);
    
     //2023-12-23 测试发现有突变大值，特点是时间偏差比较大，如977. 而且还会影响后续3~4个采样点
    //以下算法为临时措施，需要找到突变的原因
    
   
    
  /*  if(  timems < LC_TIME_MODE500-10 || timems > LC_TIME_MODE500+20)
    {
        flag_over = 1;
        tick_over = 4;
        
        printf("Error!----------------------\r\n");
    }
    
    if(1 == flag_over)
    {
       if(tick_over > 0)
       {
         tick_over--; 
         printf("break%d!----------------------\r\n",tick_over);
       }
       else
       {
          tick_over = 0;
          flag_over = 0;
       }
       
       sumvalue = sumvalue_last;
    }
    else
    {
        sumvalue_last = sumvalue;
    }
    */
    
    if(sum_int>0)
    {
      sum_int--;
      return;
    }
    else
    {

        sum_index++;
        if(0 == sum_index)
        {
             sum_sum = 0;
            sum_adver = 0;
            return;
        }
        
        sum_sum+=sumvalue;
        sum_adver = sum_sum/sum_index;
    }
    
    
     GetSystickPast(&timems);//获取这次与上次的时间差，返回毫秒
    if(1)
    {   
       /*理论sumvalue最大为4096*4000=16384000 ，flaot范围在-16777216 ~ 16777216*/
      
      //计算，从采样值-》瞬时流量

      float value_ssll = Calibration_ApplyCorrection(sumvalue);
      
      //滤波算法
        if(1 == first_cal)
        {
            EMFlow_Kalman_Init(value_ssll, 0.1f, 2.5f);
            first_cal = 0;
        }
        else
        {
           filter = EMFlow_Kalman_Update(value_ssll);
        }
          set_flow_avg_ssll_simple(filter);
    float filter_ssll = get_flow_avg_ssll_simple();
    
      gst_yc_value.ssll = filter_ssll;//filter;
      
        // 设置瞬时流量
      
     //转换称0.001L/S的脉冲当量
    //将M3/H 转换成0.001L/S脉冲当量系数是 *277.777~
    //即1000L/3600S
 
      update_flow_total_integer(totalizer, filter_ssll, timems);
      
      float pl = gst_yc_value.ljll_p_l;
      float nl = gst_yc_value.ljll_n_l;
      
      pl = gst_yc_value.ljll_p_m + pl/1000;//转换成m3单位
      nl = gst_yc_value.ljll_n_m + nl/1000;//转换成m3单位
     
      dprintf("sun:%d original:%0.1f filt:%f index:%d",sumvalue,value_ssll,filter_ssll,sum_index);
     // printf("adver:%llu ssll:%f  +m3:%f -m3:%f  cnt:%d time:%d\r\n",sum_adver,gst_yc_value.ssll/1000,pl,nl,LC1_AdcCnt,timems);
        
      
      dprintf(" adver:%lld +m3:%d-%d -m3:%d-%d  cnt:%d time:%d ,cn1:%d  cn2:%d \r\n",sum_adver,totalizer->positive_cubic_meters,(totalizer->positive_milliliters),totalizer->negative_cubic_meters,(totalizer->negative_milliliters),LC1_AdcCnt,timems,AdcRunCfg.AdcCallBackTimes[0],AdcRunCfg.AdcCallBackTimes[1]);

         
    emdcb_set_permission(PERM_LEVEL_03);

    //1.计算累计
        //正向总量小数位
    float xs = totalizer->positive_milliliters / 1000000.0;
    
    //瞬时流量
    emdcb_set_float_by_index(REG_INDEX_INSTANT_FLOW, filter_ssll); 
    //正向总量整数位
    emdcb_set_long_by_index(REG_INDEX_POSITIVE_TOTAL_INT, totalizer->positive_cubic_meters); 
    //正向总量小数位
    emdcb_set_float_by_index(REG_INDEX_POSITIVE_TOTAL_FRAC, xs); 
    

    xs = totalizer->negative_milliliters / 1000000.0;
    //正向总量整数位
    emdcb_set_long_by_index(REG_INDEX_NEGATIVE_TOTAL_INT, totalizer->negative_cubic_meters); 
    //正向总量小数位
    emdcb_set_float_by_index(REG_INDEX_NEGATIVE_TOTAL_FRAC, xs);     
    
    //2.计算流速
    
    
    
/*      //背光 0最亮 255最暗
    //  LcdDmaPrintf("BL(20)\r\n");
      //瞬时流量
      char txlcd[50];
      sprintf(txlcd,"SET_NUM(1,%3.3f,3);",gst_yc_value.ssll/1000);
      LcdDmaPrintf(txlcd);
       //累计流量
      sprintf(txlcd,"SET_NUM(3,%3.3f,3);",pl);
      LcdDmaPrintf(txlcd);
      //进度条
      sprintf(txlcd,"SET_PROG(4,%d);\r\n",(int)gst_yc_value.ssll/1000);
      LcdDmaPrintf(txlcd);*/
   //   printf("sum:%d  liusu:%d filter:%d FZ:%d SSLL:%0.1f elas:%d cf1: %d cf2: %d\r\n",sumvalue,liusu,filter,value_des_zero,value_ssll,timems,LC1_CHAFEN,LC2_CHAFEN);


    }
    
}

#define CYC_DELYA_CNT  2//计算4个周期的数值和

//计算4个周期的数值和
static int32_t GetCycleSum(int32_t Value)
{
	static uint8_t cycleindex = 0;
	static int32_t cyclevalue[10]={0,0,0,0,0,0,0,0,0,0};
	
	int i=0;
	int32_t retvaule =0;
	
	cyclevalue[cycleindex] = Value;
	cycleindex++;
	if(cycleindex >= CYC_DELYA_CNT)
	{
	  cycleindex = 0;
	}

	
	for(i=0; i<CYC_DELYA_CNT; i++)
	{
	  	retvaule += cyclevalue[i];
	}
	
	return retvaule;
}

static int32_t GetChaFen(int adcstart, int LC1_AdcCnt, uint16_t *pAdcvalue)
{
    int i;
    int32_t CHAFEN;

    for(i=adcstart; i<LC1_AdcCnt; i+=2)
    {
        CHAFEN  += (int32_t)pAdcvalue[i+1] - (int32_t)pAdcvalue[i];
    }
    return CHAFEN;
}


    

//计算此刻与上一次调用此函数经过的毫秒数
static void GetSystickPast(uint32_t *ulTimeDiffMs)
{    
    uint32_t ulCurrentTick = 0;  // 当前Tick值
    static uint32_t ulLastTickTime =0;
    /* 1. 获取当前系统Tick值 */
    ulCurrentTick = xTaskGetTickCount();

    /* 2. 计算时间差（处理Tick溢出：无符号数减法自动处理溢出） */
    // 无符号数特性：即使ulCurrentTick < ulLastTickTime（溢出），差值仍正确
    // 时间差（ms）
    *ulTimeDiffMs = ulCurrentTick - ulLastTickTime;
    
    /* 5. 更新上一次Tick值为当前值 */
    ulLastTickTime = ulCurrentTick;
    
    
}
