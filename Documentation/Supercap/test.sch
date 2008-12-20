EESchema Schematic File Version 2
LIBS:power,curr_sens,device,transistors,conn,linear,regul,74xx,cmos4000,adc-dac,memory,xilinx,special,microcontrollers,dsp,microchip,analog_switches,motorola,texas,intel,audio,interface,digital-audio,philips,display,cypress,siliconi,opto,contrib,valves
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 1 1
Title ""
Date "20 dec 2008"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Kmarq B 4000 6500 "Warning Pin power_in not driven (Net 9)" F=1
Text Label 4000 6500 0    60   ~
GND
Connection ~ 5150 3950
Connection ~ 4000 5900
Wire Wire Line
	4500 5900 4600 5900
Connection ~ 4000 6400
Wire Wire Line
	3750 6400 6350 6400
Wire Wire Line
	3750 6400 3750 6100
Wire Wire Line
	3750 4150 3750 4550
Wire Wire Line
	5150 4450 5150 4550
Connection ~ 5450 6400
Connection ~ 5750 6400
Connection ~ 5750 3600
Wire Wire Line
	6350 3600 5650 3600
Wire Wire Line
	5750 4650 5750 4800
Connection ~ 4900 3950
Wire Wire Line
	4900 4050 4900 3950
Wire Wire Line
	5150 3600 5150 3950
Wire Wire Line
	5750 3600 5750 3700
Wire Wire Line
	5150 3950 3750 3950
Wire Wire Line
	5750 4100 5750 4250
Wire Wire Line
	5750 5200 5750 5350
Wire Wire Line
	6350 6400 6350 3800
Wire Wire Line
	5450 5350 3750 5350
Wire Wire Line
	4000 6000 4000 5900
Wire Wire Line
	4000 5900 3750 5900
Wire Wire Line
	4900 4450 4900 4550
Wire Wire Line
	3750 5550 3750 5650
Wire Wire Line
	4000 6400 4000 6500
Text Label 3750 4550 0    60   ~
GND
Text Label 5150 4550 0    60   ~
GND
Text Label 4900 4550 0    60   ~
GND
Text Label 3750 5550 0    60   ~
GND
$Comp
L GND #PWR01
U 1 1 494D0EBF
P 4000 6500
F 0 "#PWR01" H 4000 6500 30  0001 C C
F 1 "GND" H 4000 6430 30  0001 C C
	1    4000 6500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 494D0EB8
P 3750 5650
F 0 "#PWR02" H 3750 5650 30  0001 C C
F 1 "GND" H 3750 5580 30  0001 C C
	1    3750 5650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 494D0EAF
P 5150 4550
F 0 "#PWR03" H 5150 4550 30  0001 C C
F 1 "GND" H 5150 4480 30  0001 C C
	1    5150 4550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 494D0EAD
P 4900 4550
F 0 "#PWR04" H 4900 4550 30  0001 C C
F 1 "GND" H 4900 4480 30  0001 C C
	1    4900 4550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 494D0EA4
P 3750 4550
F 0 "#PWR05" H 3750 4550 30  0001 C C
F 1 "GND" H 3750 4480 30  0001 C C
	1    3750 4550
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 494D0DD6
P 3400 6000
F 0 "P3" V 3350 6000 40  0000 C C
F 1 "Iout" V 3450 6000 40  0000 C C
	1    3400 6000
	-1   0    0    1   
$EndComp
$Comp
L CURR_SENS U1
U 1 1 494D0D77
P 5250 5900
F 0 "U1" H 4900 6100 60  0000 C C
F 1 "ACS756" H 5000 5650 60  0000 C C
	1    5250 5900
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 494D0AF0
P 4250 5900
F 0 "R1" V 4330 5900 50  0000 C C
F 1 "R" V 4250 5900 50  0000 C C
	1    4250 5900
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 494D0AE8
P 5150 4200
F 0 "R2" V 5230 4200 50  0000 C C
F 1 "R" V 5150 4200 50  0000 C C
	1    5150 4200
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 494D0AE1
P 5400 3600
F 0 "R3" V 5480 3600 50  0000 C C
F 1 "R" V 5400 3600 50  0000 C C
	1    5400 3600
	0    1    1    0   
$EndComp
$Comp
L ZENER D1
U 1 1 494D0ACE
P 4000 6200
F 0 "D1" H 4000 6300 50  0000 C C
F 1 "ZENER" H 4000 6100 40  0000 C C
	1    4000 6200
	0    -1   -1   0   
$EndComp
$Comp
L ZENER D2
U 1 1 494D0AC7
P 4900 4250
F 0 "D2" H 4900 4350 50  0000 C C
F 1 "ZENER" H 4900 4150 40  0000 C C
	1    4900 4250
	0    -1   -1   0   
$EndComp
$Comp
L CONN_2 P2
U 1 1 494D0ABE
P 3400 5450
F 0 "P2" V 3350 5450 40  0000 C C
F 1 "S_PWR" V 3450 5450 40  0000 C C
	1    3400 5450
	-1   0    0    1   
$EndComp
$Comp
L CONN_2 P1
U 1 1 494D0ABD
P 3400 4050
F 0 "P1" V 3350 4050 40  0000 C C
F 1 "Vout" V 3450 4050 40  0000 C C
	1    3400 4050
	-1   0    0    1   
$EndComp
$Comp
L CONN_2 P4
U 1 1 494D0AB4
P 6700 3700
F 0 "P4" V 6650 3700 40  0000 C C
F 1 "Power" V 6750 3700 40  0000 C C
	1    6700 3700
	1    0    0    -1  
$EndComp
$Comp
L CP C3
U 1 1 494D0A8D
P 5750 5000
F 0 "C3" H 5800 5100 50  0000 L C
F 1 "Supercap." H 5800 4900 50  0000 L C
	1    5750 5000
	1    0    0    -1  
$EndComp
$Comp
L CP C1
U 1 1 494D0A8A
P 5750 3900
F 0 "C1" H 5800 4000 50  0000 L C
F 1 "Supercap." H 5800 3800 50  0000 L C
	1    5750 3900
	1    0    0    -1  
$EndComp
$Comp
L CP C2
U 1 1 494D0A83
P 5750 4450
F 0 "C2" H 5800 4550 50  0000 L C
F 1 "Supercap." H 5800 4350 50  0000 L C
	1    5750 4450
	1    0    0    -1  
$EndComp
$EndSCHEMATC
