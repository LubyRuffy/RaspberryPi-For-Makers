/**************************************
If this code works, it was written by Seunghyun Lee.
If not, I don't know who wrote it
**************************************/
#include <stdio.h>    // Used for printf() statements 
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h> // Include WiringPi library! 
#include <softPwm.h> // Include Servo library! 
 
const int gpwmPin = 18; 
const int gpioPin = 13; 

//Ctrl + C �� ���� ��� ȣ��Ǵ� �ñ׳� ó�� �Լ� 
void my_ctrl_c_handler(int sig){ // can be called asynchronously
	pwmWrite (gpwmPin, 0) ; 
	pwmWrite (gpioPin, 0) ;
	exit(0);
}

int main(void) 
{ 
	int angle = 0;
	char buf[32];

	signal(SIGINT, my_ctrl_c_handler);	//Ctrl + C Handler

	//�� �ѹ����� BCM ����� ����Ѵ�.
	if (wiringPiSetupGpio () < 0) 
	{ 
		printf ("Unable to setup wiringPi\n");
		return 1 ; 
	}
	//18���� PWM ��� ������ �����Ѵ�.
	pinMode(gpwmPin, PWM_OUTPUT); 
	//PWM ��带 PWM_MODE_MS�� �����Ѵ�.
	pwmSetMode(PWM_MODE_MS);
	//PWM Ŭ�� �ӵ��� 384�� �����Ѵ�.
	pwmSetClock(384);
	//PWM ���� ������ 1000���� �Ѵ�.
	pwmSetRange(1000);

	//13���� PWM ��� ������ �����Ѵ�.
	pinMode(gpioPin, PWM_OUTPUT); 
	pwmSetMode(PWM_MODE_MS);

	//PWM ������ 0���� �Ѵ�.(���� �߻� �ȵ�)
	pwmWrite (gpwmPin, 0) ; 
	pwmWrite (gpioPin, 0) ; 

	//��Ƽ�� ���ݾ� ������Ű�� PWM ������ �߻���Ų��. ��Ƽ�� 100%�� �Ǹ� �����Ѵ�.
	while(1){
			angle++;
			if(angle > 1000) {
				angle = 0;
				break;
			}
			printf("%d\n", angle);
			//PWM ������ �߻���Ų��. ��Ƽ��(%)�� angle * 100 /1000 �� �ȴ�.
			pwmWrite (gpwmPin, angle) ; 
			pwmWrite (gpioPin, angle) ;
			//10ms ����.
			delay(100);
	}
	pwmWrite (gpwmPin, 0) ; 
	pwmWrite (gpioPin, 0) ;

	return 0; 
}
