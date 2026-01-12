#ifndef _TASK_ADC_H_
#define _TASK_ADC_H_

#define ADC_BUFFER_SIZE         2       //中断产生时ADC总转换次数，这里4个通道，所以是4  /1个通道的ADC值占2个字节长度
#define ADC_DMA_DONE_CYCLE      10      //1次DMA周期读取40个数据

#define ADC_SIZE_PER_CHANEL     4000 //1个通道ADC 采样数值大小


typedef struct{
    uint8_t  Flag_Lc_Post;      //励磁方向
    uint32_t AdcValueOffset;     //ADC采样值，偏移量
    uint32_t AdcCnt;             //ADC采样数量
    uint32_t AdcCallBackTimes[ADC_BUFFER_SIZE];//ADC回调次数
    uint16_t *pAdcValueBase[ADC_BUFFER_SIZE]; //2个通道，每个通道4000个值
    uint16_t *pAdcValue;
    uint16_t jump_tick; //开始的N个ADC不采集
}stAdcCfg;


#endif