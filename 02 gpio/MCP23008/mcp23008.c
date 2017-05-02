#include <stdio.h>
#include <wiringPi.h>
#include <mcp23017.h>

int main (void)
{
	int i, bit, ret ;

	// wiringPi �ʱ�ȭ 
	// �� �������� �ѹ��� ����� �߿����� �ʴ�.
	wiringPiSetupGpio() ;

	// Ȯ�� GPIO �� ��ȣ�� 100�� ���� ����ϸ� I2C �ּҴ� 0X20���� �˷���
	// �� �Լ��� ȣ���ϰ� ���� 100 ~ 107�������� GPIO ���� ������ GPIO �ɰ� �����ϰ� ����� �� ����
	mcp23008Setup (100, 0x20) ;

	printf ("MCP23008 Test\n") ;

	// 100 ~ 103�������� GPIO ���� ��� ���� ����
	for (i = 0 ; i < 4 ; ++i)
	pinMode (100 + i, OUTPUT) ;

	// 104 ~ 107�������� GPIO ���� �Է� ���� ����
	for (i = 4 ; i < 8 ; ++i){
		pinMode (100 + i, INPUT) ;
	}

	i = 0;
	while(1)
	{
		i++;
		// 100 ~ 103�������� GPIO ���� ON, OFF ��Ŵ. LED�� ������ ����
		for (bit = 0 ; bit < 4 ; ++bit){
			digitalWrite (100 + bit, (i % 2));
			printf("GPIO:%d Write:%d\n", 100 + bit, (i % 2));
		}
		delay (5);
		// 104 ~ 107�������� GPIO ���� �̿��� 100 ~ 103�� ���� ���¸� �о����
		for (bit = 4 ; bit < 8 ; ++bit){
			ret = digitalRead (100 + bit);
			printf("GPIO:%d READ:%d\n", 100 + bit, ret);
		}
		delay (3000) ;
	}
	return 0 ;
}