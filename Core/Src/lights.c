#include <icu_globals.h>
#include "protocol.h"

static void updateTSFlags() {
  if (left_turn_sig_adc_input>TS_ADC_THRESHOLD && right_turn_sig_adc_input>TS_ADC_THRESHOLD) {
    return;
  }
  if (left_turn_sig_adc_input>TS_ADC_THRESHOLD) {
    if (!left_ts_input_was_on) {
      Send_VCU_CAN_ICU_Command(LEFT_TS_ADDR, light_sig_output, 0);
    }
    left_ts_input_was_on = true;
  }
  else {
    left_ts_input_was_on = false;
  }
  
  if (right_turn_sig_adc_input>TS_ADC_THRESHOLD) {
    if (!right_ts_input_was_on) {
      Send_VCU_CAN_ICU_Command(RIGHT_TS_ADDR, light_sig_output, 0);
    }
    right_ts_input_was_on = true;
  }
  else {
    right_ts_input_was_on = false;
  }
}


static void updateVSLEDOutputs() {//VCU에서 온 제어. sincronize 필요.
  // buf_Layout_2[OLED_L_TS] = FDCAN1_RxBuffer[0];
  // buf_Layout_2[OLED_R_TS] = FDCAN1_RxBuffer[2];
  // buf_Layout_2[OLED_H_TS] = FDCAN1_RxBuffer[1];
  left_ts_light_on = FDCAN1_RxBuffer[0];
  right_ts_light_on = FDCAN1_RxBuffer[2];
  hazard_sig_light_on = FDCAN1_RxBuffer[1];
}


static void updateHazardSigFlag() {
  Send_VCU_CAN_ICU_Command(HAZARD_SIG_ADDR, light_sig_output,0);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  // This function is called when the ADC conversion is complete
  // 100Hz polling rate tied to TIM6
  if(hadc->Instance == ADC1) {
    for(int i = 0; i < 5; i++){
      ROT_ADC_input[i] = g_AdcDmaBuffer_1[i];
    }
    HALL_ADC_input[0] = g_AdcDmaBuffer_1[5];
    update_ROT();
    
  }
  else if(hadc->Instance == ADC2) {
    left_turn_sig_adc_input  = g_AdcDmaBuffer_2[0];
    right_turn_sig_adc_input = g_AdcDmaBuffer_2[1];
    updateTSFlags();
  }
  else if(hadc->Instance == ADC3) {
    HALL_ADC_input[1] = g_AdcDmaBuffer_3[0];
    //update_HALL();
  }
}

