#include <mraa.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include "mraa/aio.hpp"
#include "mraa/common.hpp"
#include "mraa/pwm.hpp"

#define LED_PIN 3
#define LEDR_PIN 10
#define LED2_PIN 5
#define LEDR2_PIN 11
#define AIO_LDR1 0
#define AIO_LDR2 1
#define POT1 3
#define POT2 2
#define DP1 2
#define DP2 4
#define DP3 7
#define DP4 8
#define DP5 9
#define DP6 6
#define DP7 12

pthread_t thread_2;
pthread_t thread_3;
pthread_t thread_4;

sem_t meu_semaforo;
float v1;
float v2;
float v3;
volatile sig_atomic_t flag=1;
float valor1;
int valor2 = 0;
int cont = 2;
int flagv1 = 0;
int flagv2 = 0;
float valorpot2 = 100.00;
uint32_t off = 1;
uint32_t on = 0;

mraa::Aio aio(AIO_LDR1);
mraa::Aio aio2(AIO_LDR2);
mraa::Aio aio3(POT1);
mraa::Aio aio4(POT2);

void sig_handler(int signum){
	if(signum == SIGINT){
	flag=0;
	}
}

void* tarefa_2(void *p){
	usleep(15000);
	if(v2*100 >= valorpot2 || v1*100 >= valorpot2){
		if(v1*100 >= valorpot2){
			if(flagv2 == 0){
				if(cont == 1){
				cont = 0;
				}
			else if(cont == 2){
				cont = 1;
				}
			}
			flagv2 = 1;
		}if(v2*100 >= valorpot2){
			if (flagv1==0){
				if(cont == 1){
					cont = 0;
				}
				else if(cont == 2){
					cont = 1;
				}
			}
			flagv1 = 1;
		}
	}else{
		if(v2*100 < valorpot2){
			if(cont == 1){
				cont = 2;
			}
			else if(cont == 0){
				cont = 1;
			}
			flagv2 = 0;
		}
		if(v1*100 < valorpot2){
			if(cont == 1){
				cont = 2;
			}
			else if(cont == 0){
				cont = 1;
			}
			flagv1 = 0;
		}
	}
}

void* tarefa_4(void *p){
	sem_wait(&meu_semaforo);
	valor1 = aio3.readFloat();
	valorpot2 = 100*aio4.readFloat();
	std::cout << "ADC POT1: " << valor1 << std::endl;
	sem_post(&meu_semaforo);
}

void* tarefa_3(void *p){
	sem_wait(&meu_semaforo);
	mraa::Pwm pwm(LED_PIN);
	mraa::Pwm pwm2(LEDR_PIN);
	mraa::Pwm pwm1(LED2_PIN);
	mraa::Pwm pwm22(LEDR2_PIN);
	pwm.enable(true);
	pwm2.enable(true);
	pwm1.enable(true);
	pwm22.enable(true);
	if(v1*100 < valorpot2){
		pwm.write(valor1);
		pwm1.write(valor2);
	}else if(v1*100 >= valorpot2){
		pwm1.write(valor1);
		pwm.write(valor2);
	}if(v2*100 < valorpot2){
		pwm2.write(valor1);
		pwm22.write(valor2);
	}else if(v2*100 >= valorpot2){
		pwm2.write(valor2);
		pwm22.write(valor1);
	}
	sem_post(&meu_semaforo);
}

int main(void){
	mraa_init();
	mraa_gpio_context dp1;
	mraa_gpio_context dp2;
	mraa_gpio_context dp3;
	mraa_gpio_context dp4;
	mraa_gpio_context dp5;
	mraa_gpio_context dp6;
	mraa_gpio_context dp7;
	dp1 = mraa_gpio_init(DP1);
	dp2 = mraa_gpio_init(DP2);
	dp3 = mraa_gpio_init(DP3);
	dp4 = mraa_gpio_init(DP4);
	dp5 = mraa_gpio_init(DP5);
	dp6 = mraa_gpio_init(DP6);
	dp7 = mraa_gpio_init(DP7);
	mraa_gpio_dir(dp1,MRAA_GPIO_OUT);
	mraa_gpio_dir(dp2,MRAA_GPIO_OUT);
	mraa_gpio_dir(dp3,MRAA_GPIO_OUT);
	mraa_gpio_dir(dp4,MRAA_GPIO_OUT);
	mraa_gpio_dir(dp5,MRAA_GPIO_OUT);
	mraa_gpio_dir(dp6,MRAA_GPIO_OUT);
	mraa_gpio_dir(dp7,MRAA_GPIO_OUT);
	mraa_gpio_write(dp1,off);
	mraa_gpio_write(dp2,on);
	mraa_gpio_write(dp3,on);
	mraa_gpio_write(dp4,on);
	mraa_gpio_write(dp5,on);
	mraa_gpio_write(dp6,on);
	mraa_gpio_write(dp7,off);
	signal(SIGINT, sig_handler);
	sem_init(&meu_semaforo,0,1);
	while(flag){
		std::cout << "CONT: " << cont << std::endl;
		usleep(5000);
		v2 = aio2.readFloat();
		v1 = aio.readFloat();
		//std::cout << "ADC LDR2: " << (v2*100) << std::endl;
		//std::cout << "ADC LDR1: " << (v1*100) - 2 << std::endl;
		pthread_create(&thread_2,NULL,tarefa_2,NULL);
		pthread_create(&thread_3,NULL,tarefa_3,NULL);
		pthread_create(&thread_4,NULL,tarefa_4,NULL);
		pthread_join(thread_3,NULL);
		pthread_join(thread_2,NULL);
		pthread_join(thread_4,NULL);
		if(cont == 0){
			mraa_gpio_write(dp1,on);
			mraa_gpio_write(dp2,on);
			mraa_gpio_write(dp3,on);
			mraa_gpio_write(dp4,on);
			mraa_gpio_write(dp5,on);
			mraa_gpio_write(dp6,on);
			mraa_gpio_write(dp7,off);
		}else if(cont == 1){
			mraa_gpio_write(dp1,on);
			mraa_gpio_write(dp2,off);
			mraa_gpio_write(dp3,off);
			mraa_gpio_write(dp4,on);
			mraa_gpio_write(dp5,off);
			mraa_gpio_write(dp6,off);
			mraa_gpio_write(dp7,off);
		}else if(cont==2){
			mraa_gpio_write(dp1,off);
			mraa_gpio_write(dp2,on);
			mraa_gpio_write(dp3,on);
			mraa_gpio_write(dp4,on);
			mraa_gpio_write(dp5,on);
			mraa_gpio_write(dp6,off);
			mraa_gpio_write(dp7,on);
		}
	}
	sem_destroy(&meu_semaforo);
	return EXIT_SUCCESS;
}