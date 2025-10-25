#include <stdio.h>
#include <signal.h>
#include <pigpiod_if2.h>

static const int LED = 17;
static const int BTN = 27;

static volatile int running = 1;
static volatile int duty = 0;
static const int step = 64;

void on_sigint(int sig){
	(void)sig;
	running = 0;
}

void btn_cb(int pi, unsigned user_gpio, unsigned level, uint32_t tick){
	if(user_gpio!=BTN) return;
	if(level == 0) {
		duty+=step;
		if(duty>255)duty = 0;
		set_PWM_dutycycle(pi, LED, duty);
		printf("[tick %u us] Button is pressed. -> luminosity : %d/255\n", tick, duty);
	}
}

int main(void){
	int pi = pigpio_start(NULL, NULL);
	if(pi<0){
		printf("pigpio connecting failure.");
		return 1;
	}
	
	set_mode(pi, LED, PI_OUTPUT);
	set_mode(pi, BTN, PI_INPUT);
	set_pull_up_down(pi, BTN, PI_PUD_UP);
	
	set_glitch_filter(pi, BTN, 30000);
	set_PWM_frequency(pi, LED, 1000);
	
	int btn_id = callback(pi, BTN, FALLING_EDGE, btn_cb);

	printf("PWM manipulating start! LED luminosity manipulate to press buttn.\n");
	printf("(Use Ctrl + C for finish.)\n");

	while(running){
		time_sleep(0.1);
	}
	
	callback_cancel(btn_id);
	set_PWM_dutycycle(pi, LED, 0);
	pigpio_stop(pi);
	printf("Program finished.\n");
	
	return 0;
}
