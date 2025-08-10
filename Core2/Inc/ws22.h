#ifndef WS22_H
#define WS22_H

#include <stdint.h>
#include "protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

void WS22_HandlePacket(uint32_t id);
void WS22_SendMotorCommand(void);
void WS22_ForwardToICU(uint32_t id);

static void Parse_WS22_CAN_Packet(uint32_t offset);
static uint32_t Read_throttle_ADC(void);
static float Convert_throttle_input_into_current_command(uint32_t thr_in,
                                                         uint32_t thr_dead_zone_threshold,
                                                         uint32_t thr_dynamic_range);
                                            
#ifdef __cplusplus
}
#endif
#endif /* WS22_H */