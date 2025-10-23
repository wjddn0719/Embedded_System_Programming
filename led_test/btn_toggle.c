
#include <stdio.h>
#include <pigpiod_if2.h>
#include <stdint.h>

int main(void){
    int pi = pigpio_start(NULL, NULL);
    if(pi<0){
        printf("연결실패");
        return 1;
    }

    const int LED = 17;
    const int BTN = 27;

    set_mode(pi, LED, PI_OUTPUT);
    set_mode(pi, BTN, PI_INPUT);

    set_pull_up_down(pi, BTN, PI_PUD_UP);

    int led_state = 0;
    int last = 0;
    uint32_t last_tick = get_current_tick(pi);
    while(1){
        int now = gpio_read(pi, BTN);

        uint32_t tick_now = get_current_tick(pi);

        if(now != last && (tick_now - last_tick)>50){
            last = now;
            last_tick = tick_now;
            if(now==0){
                led_state ^=1;
                gpio_write(pi, LED, led_state);
                printf("버튼 눌림 -> LED %s\n", led_state ? "ON" : "OFF");
            }
            time_sleep(0.01);
        }
    }   
    pigpio_stop(pi);
    return 0;
}
