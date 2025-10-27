#include <stdio.h>
#include <pigpiod_if2.h>
#include <signal.h>

static const int LED = 17;
static const int BTN = 27;

static volatile int running = 1;
static volatile int duty = 0;
static const int step = 64;
static const int freqs[] = {100, 500, 1000, 5000, 20000};
static const int NF = sizeof(freqs)/sizeof(freqs[0]);
static volatile int fidx = 2;

static volatile uint32_t press_start_tick = 0;
static const uint32_t LONG_THRESHOLD_US = 600000;

void on_sigint(int sig){
	(void)sig;
	running = 0;
}

void apply_duty(int pi){
	set_PWM_dutycycle(pi, LED, duty);
	printf("Luminosity : %d/255\n", duty);
	duty+=step;
	if(duty>255){
		duty = 0;
	}
}

void apply_freq(int pi){
	set_PWM_frequency(pi, LED, freqs[fidx]);
	printf("Frequency : %d Hz\n", freqs[fidx]);
	fidx++;
	if(fidx>=NF){
		fidx = 0;
	}
}

void btn_cb(int pi, unsigned user_gpio, unsigned level, uint32_t tick){
	if(user_gpio != BTN) return;
	if(level==0){
		press_start_tick = tick;
	}
	else if(level==1){
		uint32_t dt = tick - press_start_tick;
		if(dt >= LONG_THRESHOLD_US){
			apply_freq(pi);
		}
		else{
			apply_duty(pi);
		}
	}
	else{
		return;
	}
}

int main(void){
	int pi = pigpio_start(NULL, NULL);
	if(pi < 0){
	printf("Pigpio connecting failure.\n");
	return 1;	
	}

	signal(SIGINT, on_sigint);
	
	set_mode(pi, LED, PI_OUTPUT);
	set_mode(pi, BTN, PI_INPUT);
	set_pull_up_down(pi, BTN, PI_PUD_UP);
	set_glitch_filter(pi, BTN, 30000);

	apply_freq(pi);
	apply_freq(pi);

	int cb_id = callback(pi, BTN, EITHER_EDGE, btn_cb);

	printf("Shortly : luminosity changing / Longly : frequency change (Ctrl + C finish)\n ");
	while(running)time_sleep(0.1);

	callback_cancel(cb_id);
	set_PWM_dutycycle(pi, LED, 0);
	pigpio_stop(pi);
	printf("Program finished.\n");
	return 0;
}
