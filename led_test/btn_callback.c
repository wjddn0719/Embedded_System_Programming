#include <stdio.h>
#include <signal.h>
#include <pigpiod_if2.h>

static const int LED = 17;
static const int BTN = 27;

static volatile int led_state = 0;
static volatile int running = 1;

void on_sigint(int sig){
    (void)sig;
    running = 0;
}

void btn_cb(int pi, unsigned user_gpio, unsigned level, uint32_t tick){
    if(user_gpio != BTN) return;

    if(level == 0){
        led_state ^=1;
        gpio_write(pi, LED, led_state);
        printf("[콜백 실행 시각 %u us] 버튼 눌림 -> LED %s\n", tick, led_state ? "ON" : "OFF");
    }
}

int main(void){
    int pi = pigpio_start(NULL, NULL);
    if(pi<0){
        printf("pigpio 연결 실패");
        return 1;
    }

    signal(SIGINT, on_sigint);

    set_mode(pi, LED, PI_OUTPUT);
    set_mode(pi, BTN, PI_INPUT);

    set_pull_up_down(pi, BTN, PI_PUD_UP);

    set_glitch_filter(pi, BTN, 30000);

    int cb_id = callback(pi, BTN, FALLING_EDGE, btn_cb);

    printf("버튼 콜백 등록완료! 버튼을 눌러 LED를 토글하세요. \n");
    printf("(Ctrl+C 누르면 프로그램 종료)");
    
    while(running){
        time_sleep(0.1);
    }

    callback_cancel(cb_id);
    gpio_write(pi, LED, 0);
    pigpio_stop(pi);
    printf("프로그램 종료.\n");

    return 0;
}
