#!/usr/bin/env python
# -*- coding: cp949 -*-
#If this code works, it was written by Seunghyun Lee.
#If not, I don't know who wrote it
 
import spidev
import time
import os

Vcc = 5.0
R1 = 1000

# SPI ������ �ʱ�ȭ�Ѵ�.
spi = spidev.SpiDev()
spi.open(0,0)
 
# fsr402 ��°��� ����Ѵ�.
def fsr420_Registor(voltage):
	R = (R1 * Vcc)/voltage - R1
	return R

# MCP3008���� fsr402 ���� ���� �д´�. 
def ReadChannel(channel):
	adc = spi.xfer([1,(8 + channel)<<4,0])
	data = ((adc[1]&3) << 8) + adc[2]
	return data
 
# �츮�� MCP3008 ������ ��� 0�� ä���� �̿��� ���̴�.
mcp3008_channel = 0
 
# ���� �ֱ⸦ �����Ѵ�.
delay = 0.01

# gnuplot���� �м��� �� �ִ� ������ �����Ѵ�.
f = open('fsr402.dat', 'w') 
index = 0

try: 
	while True:
		# mcp3008���� FSR402�� ���� ���� �д´�.
		analog_level = ReadChannel(mcp3008_channel)
		Vout = analog_level * Vcc / 1024.0
		if(Vout == 0.0):
			Vout = 0.001
		# FSR402�� ���� ���� ����Ѵ�.
		Rfsr = fsr420_Registor(Vout)
		print "Digital:", analog_level, " Voltage:", Vout, " R(K Ohm):", Rfsr / 1000.0
		data = "{} {} {} {}\n".format(index, analog_level, Vout, Rfsr / 1000.0)
		f.write(data)
		time.sleep(delay)
		index += 1

except KeyboardInterrupt:   
	print "Now Exit"
	f.close()
