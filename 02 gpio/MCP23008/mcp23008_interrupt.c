#include <stdio.h> 
#include <string.h> 
#include <errno.h> 
#include <stdlib.h> 
#include <wiringPi.h> 

// MCP23008�� I2C �ּ�.
int dID = 0x20;	
// MCP23008�� ���� �ڵ鰪
int fd = 0;	
int GPIO_INTERRUPT = 17;

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

//����Ʈ ������ �Է¹޾� ������ ������ ���ڿ��� �����Ѵ�. 
char *byte_dec2bin(char n)
{
	static char bStr[64];
	unsigned char a;
	unsigned char shift = n;
	int length, x;

	memset(bStr, 0x00, sizeof(bStr));
	if (n < 0){
		return NULL;
	}

	if( n == 0 ){
		strcpy(bStr, "00000000");
		return bStr;
	}

	for (x = 0; x < 8; x++)
	{
		a = (shift % 2) + '0';
		bStr[7 - x] =  a;
		shift = shift >> 1;
	}
	return bStr;
}

// ���� MCP23008Ĩ�� ���� ���¸� ����Ʈ�Ѵ�.
void print_mcp23008()
{
	char val;
	//GPIO IN/OUT ������ ���
	printf( "high bit <---> low bit\n");
	printf( "MCP23008 I/O Mode\n");
	val = wiringPiI2CReadReg8(fd, MCP23017_IODIRA) ;
	printf("%s\n", byte_dec2bin(val));

	printf( "MCP23008 Input Polarity Mode\n");
	val = wiringPiI2CReadReg8(fd, MCP23017_IODIRB) ;
	printf("%s\n", byte_dec2bin(val));

	printf ("MCP23008 Interrupt-on-change Control Register\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_GPINTEN) ;
	printf("%s\n", byte_dec2bin(val));

	printf( "MCP23008 DEFVAL\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_DEFVAL) ;
	printf("%s\n", byte_dec2bin(val));


	printf ("MCP23008 INTCON\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_INTCON) ;
	printf("%s\n", byte_dec2bin(val));

	printf( "MCP23008 IOCON\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_IOCON) ;
	printf("%s\n", byte_dec2bin(val));

	printf( "MCP23008 Pull up Register\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_GPPUA) ;
	printf("%s\n", byte_dec2bin(val));

	printf( "MCP23008 INTF\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_INTF) ;
	printf("%s\n", byte_dec2bin(val));
	
	printf ("MCP23008 INTCAP\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_INTCAP) ;
	printf("%s\n", byte_dec2bin(val));

	printf( "MCP23008 GPIO Status\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_GPIOA) ;
	printf("%s\n", byte_dec2bin(val));
	
	printf( "MCP23008 OLAT\n");
	val = wiringPiI2CReadReg8(fd, MCP23008_OLATA) ;
	printf("%s\n", byte_dec2bin(val));
	printf( "-------------\n");
}

//���ͷ�Ʈ �Լ��� ȣ��Ǹ� �۷ι� ���� globalCounter ���� 1 ������Ų��.
void myInterrupt (void) 
{ 
	char val;
	val = wiringPiI2CReadReg8(fd, MCP23008_INTF) ;
	printf ("Interrrupt Occured ->By   GPIO:%s\n", byte_dec2bin(val));
	if(0 == val)
		return;
	val = wiringPiI2CReadReg8(fd, MCP23008_INTCAP) ;
	printf( "Interrrupt Occured ->Read GPIO:%s\n", byte_dec2bin(val));
} 


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
	char buf[128];
	// wiringPi I2C �ʱ�ȭ
	if((fd=wiringPiI2CSetup(dID))<0){
		printf("error opening i2c channel [%X]\n\r", (unsigned char)dID);
		return 0;
	}
	
	// �� �ѹ����� BCM ����� ����Ѵ�.
    wiringPiSetupGpio(); 
	// 17�� ���� �Է¸��� ���� ->���ͷ�Ʈ�� ���� ��. MCP23008�� 8���ɰ� ����
	// �ݵ�� MCP23008�� ������ 3.3V�� ����Ѵ�.
    pinMode(GPIO_INTERRUPT, INPUT);

	// GPIO Ȯ������ IO ��带 �ʱ� �� �Է¸��� �����Ѵ�.
	wiringPiI2CWriteReg8 (fd, MCP23017_IODIRA, 0xFF) ;
	// GPIO Ȯ������ IO ��带 �����Ѵ�.
	// G0 ~ G3 : ��¸��(0), G4 ~ G7:�Է¸��(1)�� ������. ���� ������ 0XF0�� �ȴ�.
	wiringPiI2CWriteReg8 (fd, MCP23017_IODIRA, 0xF0) ;

	//GPIO �ɿ� �ݴ� �ؼ��� �ο����� �ʴ´�.
	wiringPiI2CWriteReg8 (fd, MCP23017_IODIRB, 0x00) ;
	//�Է¸�� 4�ɿ� ���ͷ�Ʈ �Ӽ��� �ο��Ѵ�.
	wiringPiI2CWriteReg8 (fd, MCP23008_GPINTEN, 0xF0) ;
	//GPIO ���� ���� ���� ���ؼ� ���ͷ�Ʈ�� �߻���Ų��.
	wiringPiI2CWriteReg8 (fd, MCP23008_INTCON, 0x00) ;
	//GPIO ���� Ǯ�� ���׿� ����.
	wiringPiI2CWriteReg8 (fd, MCP23008_GPPUA, 0xF0) ;
	//��Ʈ�� �������� ����
	wiringPiI2CWriteReg8 (fd, MCP23008_IOCON, 0x28) ;

	// ON->OFF, OFF->ON ��� �̺�Ʈ �߻�
	if (wiringPiISR (GPIO_INTERRUPT, INT_EDGE_BOTH, &myInterrupt) < 0)   
	{ 
		fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ; 
		return 1 ; 
	} 

	i = 0;
	while(1)
	{
		i++;
		// G0 ~ G3�������� GPIO ���� ON, OFF ��Ŵ. LED�� ������ ����
		for (bit = 0 ; bit < 4 ; ++bit){
			printf("Next");
			gets(buf);
			update_gpio(bit, (i % 2));
		}
	}
	return 0 ;
}