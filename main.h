#include "ch.h"
#include "hal.h"

/*Macros*/
#define BUFFERSIZE 16  /*for rx,tx buffers*/
#define RESET_LIMIT 3000

/*Constants */
static const ADCConversionGroup adcch10 = {
   // this 3 fields are common for all MCUs
      // set to TRUE if need circular buffer, set FALSE otherwise
      circular : FALSE,
      // number of channels
      num_channels : 1,
      // callback function when conversion ends
      end_cb : NULL,
      //callback function when error appears
      error_cb : NULL,
      //look to datasheet for information about the registers
      // CR1 register content
      cr1 : 0,
      // CR2 register content
      cr2 : ADC_CR2_SWSTART,//?
      // SMRP1 register content
      smpr1 : 0b110,//144cycles
      // SMRP2 register content
      smpr2 : 0,
      // SQR1 register content
      sqr1 : ((ADC_CH_NUM - 1) << 20),
      // SQR2 register content
      sqr2 : 0,
      // SQR3 register content. We must select 1 channel
      // For example 2nd channel
      // if we want to select more than 1 channel then simply
      // shift and logic or the values example (ch 15 & ch 10) : (15 | (10 << 5))
      sqr3 : 10
};

static const ADCConversionGroup adcch11 = {
   // this 3 fields are common for all MCUs
      // set to TRUE if need circular buffer, set FALSE otherwise
      circular : FALSE,
      // number of channels
      num_channels : 1,
      // callback function when conversion ends
      end_cb : NULL,
      //callback function when error appears
      error_cb : NULL,
      //look to datasheet for information about the registers
      // CR1 register content
      cr1 : 0,
      // CR2 register content
      cr2 : ADC_CR2_SWSTART,//?
      // SMPR1 register content
      smpr1 : 0b110000,//144cycles
      // SMPR2 register content
      smpr2 : 0,
      // SQR1 register content
      sqr1 : ((ADC_CH_NUM - 1) << 20),
      // SQR2 register content
      sqr2 : 0,
      // SQR3 register content. We must select 1 channel
      // For example 2nd channel
      // if we want to select more than 1 channel then simply
      // shift and logic or the values example (ch 15 & ch 10) : (15 | (10 << 5))
      sqr3 : 11
};

static const ADCConversionGroup adcch12 = {
   // this 3 fields are common for all MCUs
      // set to TRUE if need circular buffer, set FALSE otherwise
      circular : FALSE,
      // number of channels
      num_channels : 1,
      // callback function when conversion ends
      end_cb : NULL,
      //callback function when error appears
      error_cb : NULL,
      //look to datasheet for information about the registers
      // CR1 register content
      cr1 : 0,
      // CR2 register content
      cr2 : ADC_CR2_SWSTART,//?
      // SMRP1 register content
      smpr1 : 0b110000000,//144cycles
      // SMRP2 register content
      smpr2 : 0,
      // SQR1 register content
      sqr1 : ((ADC_CH_NUM - 1) << 20),
      // SQR2 register content
      sqr2 : 0,
      // SQR3 register content. We must select 1 channel
      // For example 2nd channel
      // if we want to select more than 1 channel then simply
      // shift and logic or the values example (ch 15 & ch 10) : (15 | (10 << 5))
      sqr3 : 12
};

/*Function prototypes*/
void init_globals();
void init_IO();
void adc_configure();
uint16_t adc_convert(uint8_t);
void set_cca_exp_io_mode();
void set_module_addr(protocol_01*);
