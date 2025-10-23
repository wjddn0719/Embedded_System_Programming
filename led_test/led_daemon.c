#include <stdio.h>
#include <pigpiod_if2.h>
#include <unistd.h>

int main(void){
    int pi = pigpio_start(NULL, NULL);
    if(pi < 0){
        printf("pigpio 데몬 연결 실패! (pigpiod가 실행 중인지 확인)\n");
        return 1;
    }
    int led_pin = 17;

    set_mode(pi, led_pin, PI_OUTPUT);

    for(int i=0; i<10; i++){
        gpio_write(pi, led_pin, 1);
        time_sleep(0.5);
        gpio_write(pi, led_pin, 0);
        time_sleep(0.5);
    }
    pigpio_stop(pi);
    return 0;
}
