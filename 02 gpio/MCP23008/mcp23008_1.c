#include <stdio.h> 
#include <string.h> 
#include <errno.h> 
#include <stdlib.h> 
#include <wiringPi.h> 

// MCP23008�� I2C �ּ�.
int dID = 0x20;	
// MCP23008�� ���� �ڵ鰪
int fd = 0;	

#define MCP23017_IODIRA  0x00
#define MCP23017_IODIRB  0x01
#define MCP23017_GPIOA   0x12
#define MCP23017_GPIOB   0x13
#define MCP23017_GPPUA   0x0C
#define MCP23017_GPPUB   0x0D
#define MCP23017_OLATA   0x14
#define MCP23017_OLATB   0x15
#define MCP23008_GPIOA   0x09
#define MCP23008_GPPUA   0x06
#define MCP23008_OLATA   0x0A
#define MCP23008_GPINTEN  0x02
#define MCP23008_DEFVAL  0x03
#define MCP23008_INTCON  0x04
#define MCP23008_IOCON  0x05
#define MCP23008_INTF  0x07
#define MCP23008_INTCAP  0x08


char update_gpio(int pin, int val)
{
	char old, new_val;
	if(pin > 8 || pin < 0){
		printf("Invalid PIN number[%d]\n", pin);
		return 0;
	}
	//���� GPIO�� 8���� ���¸� ��� �д´�.
	old = (char)wiringPiI2CReadReg8(fd, MCP23008_GPIOA) ;
	//�ش� ��Ʈ�� ���� �����Ѵ�.
	if (val == 0)
		new_val = (char)(old & ~(1 << pin));
	else
		new_val = (char)(old | (1 << pin));

	//������ ��Ʈ ���� ����Ѵ�.
	wiringPiI2CWriteReg8 (fd, MCP23008_GPIOA, new_val) ;
	return new_val;
}

int read_gpio(int pin)
{
	char old, val;
	if(pin > 8 || pin < 0){
		printf("Invalid PIN number[%d]\n", pin);
		return 0;
	}
	//���� GPIO�� 8���� ���¸� ��� �д´�.
	old = (char)wiringPiI2CReadReg8(fd, MCP23008_GPIOA) ;
	//�ɿ� �ش��ϴ� ��Ʈ ���� 1�̸� 1�� �ƴϸ� 0�� �����Ѵ�.
	if(old & (1 << pin)) return 1;
	return 0;
}


int main (void)
{
	int i, bit, ret ;

	// wiringPi I2C �ʱ�ȭ
	if((fd=wiringPiI2CSetup(dID))<0){
		printf("error opening i2c channel [%X]\n\r", (unsigned char)dID);
		return 0;
	}
	
	//GPIO Ȯ������ IO ��带 �����Ѵ�.
	//G0 ~ G3 : ��¸��(0), G4 ~ G7:�Է¸��(1)�� ������. ���� ������ 0XF0�� �ȴ�.
	wiringPiI2CWriteReg8 (fd, MCP23017_IODIRA, 0xF0) ;


	i = 0;
	while(1)
	{
		i++;
		// G0 ~ G3�������� GPIO ���� ON, OFF ��Ŵ. LED�� ������ ����
		for (bit = 0 ; bit < 4 ; ++bit){
			update_gpio(bit, (i % 2));
			printf("GPIO:%d Write:%d\n", bit, (i % 2));
		}
		delay (5);
		// G4 ~ G7�������� GPIO ���� �̿��� G0 ~ G3�� ���� ���¸� �о����
		for (bit = 4 ; bit < 8 ; ++bit){
			ret = read_gpio (bit);
			printf("GPIO:%d READ:%d\n", bit, ret);
		}
		delay (3000) ;
	}
	return 0 ;
}