#ifndef _BOB3_CLOCK_H_

#define _BOB3_CLOCK_H_
#ifdef __cplusplus
extern "C" {
#endif

void _clock_handle_interrupt_isr();
void clock_sleep_until_ms(uint32_t ms);
void clock_sleep_ms(uint32_t ms);
uint16_t clock_get_us();
uint32_t clock_get_sys_ms();




#ifdef __cplusplus
} // extern "C"
#endif

#endif
