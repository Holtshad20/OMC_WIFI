EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "OMC_WIFI"
Date "2022-02-11"
Rev "V1"
Comp "SCADA"
Comment1 "Br. Carlos García y Br. David Hernández"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x15_Female J1
U 1 1 620C54F2
P 2050 4850
F 0 "J1" H 2078 4876 50  0000 L CNN
F 1 "ESP32_Left" H 2078 4785 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x15_P2.54mm_Vertical" H 2050 4850 50  0001 C CNN
F 3 "~" H 2050 4850 50  0001 C CNN
	1    2050 4850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x15_Female J2
U 1 1 620C864B
P 3150 4850
F 0 "J2" H 3042 5735 50  0000 C CNN
F 1 "ESP32_Right" H 3042 5644 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x15_P2.54mm_Vertical" H 3150 4850 50  0001 C CNN
F 3 "~" H 3150 4850 50  0001 C CNN
	1    3150 4850
	-1   0    0    -1  
$EndComp
Text GLabel 1450 4150 0    50   Input ~ 0
EN
Text GLabel 1450 4250 0    50   Input ~ 0
VP_ZMPT
Text GLabel 1450 4350 0    50   Input ~ 0
VN_ACS
Text GLabel 1450 4450 0    50   Input ~ 0
D34
Text GLabel 1450 4550 0    50   Input ~ 0
D35
Text GLabel 1450 4650 0    50   Input ~ 0
D32_RELAY
Text GLabel 1450 4750 0    50   Input ~ 0
D33
Text GLabel 1450 4850 0    50   Input ~ 0
D25
Text GLabel 1450 4950 0    50   Input ~ 0
D26
Text GLabel 1450 5050 0    50   Input ~ 0
D27_LED
Text GLabel 1450 5150 0    50   Input ~ 0
D14
Text GLabel 1450 5250 0    50   Input ~ 0
D12
Text GLabel 1450 5350 0    50   Input ~ 0
D13
Text GLabel 3750 4150 2    50   Input ~ 0
D23
Text GLabel 3750 4250 2    50   Input ~ 0
D22
Text GLabel 3750 4350 2    50   Input ~ 0
TX0
Text GLabel 3750 4450 2    50   Input ~ 0
RX0
Text GLabel 3750 4550 2    50   Input ~ 0
D21
Text GLabel 3750 4650 2    50   Input ~ 0
D19
Text GLabel 3750 4750 2    50   Input ~ 0
D18
Text GLabel 3750 4850 2    50   Input ~ 0
D5
Text GLabel 3750 4950 2    50   Input ~ 0
TX2
Text GLabel 3750 5050 2    50   Input ~ 0
RX2
Text GLabel 3750 5150 2    50   Input ~ 0
D4
Text GLabel 3750 5250 2    50   Input ~ 0
D2
Text GLabel 3750 5350 2    50   Input ~ 0
D15
Text GLabel 3750 5550 2    50   Input ~ 0
3V3
Wire Wire Line
	3750 4150 3350 4150
Wire Wire Line
	3750 4250 3350 4250
Wire Wire Line
	3750 4350 3350 4350
Wire Wire Line
	3750 4450 3350 4450
Wire Wire Line
	3750 4550 3350 4550
Wire Wire Line
	3750 4650 3350 4650
Wire Wire Line
	3750 4750 3350 4750
Wire Wire Line
	3750 4850 3350 4850
Wire Wire Line
	3750 4950 3350 4950
Wire Wire Line
	3750 5050 3350 5050
Wire Wire Line
	3750 5150 3350 5150
Wire Wire Line
	3750 5250 3350 5250
Wire Wire Line
	3750 5350 3350 5350
Wire Wire Line
	3750 5550 3350 5550
Wire Wire Line
	1450 4150 1850 4150
Wire Wire Line
	1450 4250 1850 4250
Wire Wire Line
	1450 4350 1850 4350
Wire Wire Line
	1450 4450 1850 4450
Wire Wire Line
	1450 4550 1850 4550
Wire Wire Line
	1450 4650 1850 4650
Wire Wire Line
	1450 4750 1850 4750
Wire Wire Line
	1450 4850 1850 4850
Wire Wire Line
	1450 4950 1850 4950
Wire Wire Line
	1450 5050 1850 5050
Wire Wire Line
	1450 5150 1850 5150
Wire Wire Line
	1450 5250 1850 5250
Wire Wire Line
	1450 5350 1850 5350
Wire Notes Line
	700  5850 4150 5850
Text Notes 1850 3750 0    50   ~ 10
Módulo ESP-WROOM32-DevKit1\n
Wire Notes Line
	4150 3550 700  3550
Wire Notes Line
	700  3550 700  5850
Wire Notes Line
	4150 3550 4150 5850
$Comp
L power:+5V #PWR0101
U 1 1 621F2808
P 800 5450
F 0 "#PWR0101" H 800 5300 50  0001 C CNN
F 1 "+5V" H 815 5623 50  0000 C CNN
F 2 "" H 800 5450 50  0001 C CNN
F 3 "" H 800 5450 50  0001 C CNN
	1    800  5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  5450 800  5550
Wire Wire Line
	800  5550 1850 5550
$Comp
L power:GND #PWR0102
U 1 1 622052E0
P 950 5350
F 0 "#PWR0102" H 950 5100 50  0001 C CNN
F 1 "GND" H 955 5177 50  0000 C CNN
F 2 "" H 950 5350 50  0001 C CNN
F 3 "" H 950 5350 50  0001 C CNN
	1    950  5350
	1    0    0    1   
$EndComp
Wire Wire Line
	950  5450 950  5350
Wire Wire Line
	950  5450 1850 5450
$Comp
L Converter_ACDC:HLK-PM01 PS1
U 1 1 622A159D
P 5500 2550
F 0 "PS1" H 5500 2875 50  0000 C CNN
F 1 "HLK-PM01" H 5500 2784 50  0000 C CNN
F 2 "Converter_ACDC:Converter_ACDC_HiLink_HLK-PMxx" H 5500 2250 50  0001 C CNN
F 3 "http://www.hlktech.net/product_detail.php?ProId=54" H 5900 2200 50  0001 C CNN
	1    5500 2550
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 622CF9F0
P 1750 1950
F 0 "#FLG0101" H 1750 2025 50  0001 C CNN
F 1 "PWR_FLAG" H 1750 2123 50  0000 C CNN
F 2 "" H 1750 1950 50  0001 C CNN
F 3 "~" H 1750 1950 50  0001 C CNN
	1    1750 1950
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 622D0110
P 1750 2050
F 0 "#FLG0102" H 1750 2125 50  0001 C CNN
F 1 "PWR_FLAG" H 1750 2223 50  0000 C CNN
F 2 "" H 1750 2050 50  0001 C CNN
F 3 "~" H 1750 2050 50  0001 C CNN
	1    1750 2050
	1    0    0    1   
$EndComp
$Comp
L power:+5V #PWR0103
U 1 1 622DB783
P 6050 2300
F 0 "#PWR0103" H 6050 2150 50  0001 C CNN
F 1 "+5V" H 6065 2473 50  0000 C CNN
F 2 "" H 6050 2300 50  0001 C CNN
F 3 "" H 6050 2300 50  0001 C CNN
	1    6050 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 622DC1F4
P 6050 2700
F 0 "#PWR0104" H 6050 2450 50  0001 C CNN
F 1 "GND" H 6055 2527 50  0000 C CNN
F 2 "" H 6050 2700 50  0001 C CNN
F 3 "" H 6050 2700 50  0001 C CNN
	1    6050 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 2450 6050 2450
Wire Wire Line
	6050 2450 6050 2300
Wire Wire Line
	5900 2650 6050 2650
Wire Wire Line
	6050 2650 6050 2700
$Comp
L power:GND #PWR0107
U 1 1 62346668
P 4050 5550
F 0 "#PWR0107" H 4050 5300 50  0001 C CNN
F 1 "GND" H 4055 5377 50  0000 C CNN
F 2 "" H 4050 5550 50  0001 C CNN
F 3 "" H 4050 5550 50  0001 C CNN
	1    4050 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 5450 4050 5550
Wire Wire Line
	3350 5450 4050 5450
Text Notes 1200 1400 0    50   ~ 10
Alimentación y Carga
Wire Notes Line
	900  2550 3800 2550
Wire Notes Line
	3800 1200 900  1200
$Comp
L Connector:Screw_Terminal_01x03 J3
U 1 1 6243119F
P 1350 2050
F 0 "J3" H 1268 2367 50  0000 C CNN
F 1 "Power_In" H 1268 2276 50  0000 C CNN
F 2 "6PCV-03-006:TE_5-1437652-4" H 1350 2050 50  0001 C CNN
F 3 "~" H 1350 2050 50  0001 C CNN
	1    1350 2050
	-1   0    0    -1  
$EndComp
Text GLabel 2200 2050 2    50   Input ~ 0
I_ACS
Text GLabel 2650 2050 0    50   Input ~ 0
NO
Text GLabel 1900 6100 0    50   Input ~ 0
EN
Text GLabel 1950 6200 0    50   Input ~ 0
D34
Text GLabel 1950 6300 0    50   Input ~ 0
D35
Text GLabel 1950 6400 0    50   Input ~ 0
D33
Text GLabel 1950 6500 0    50   Input ~ 0
D25
Text GLabel 1950 6600 0    50   Input ~ 0
D26
Text GLabel 1950 6700 0    50   Input ~ 0
D14
Text GLabel 1950 6800 0    50   Input ~ 0
D12
Text GLabel 1950 6900 0    50   Input ~ 0
D13
Text GLabel 3050 6100 2    50   Input ~ 0
D23
Text GLabel 3050 6200 2    50   Input ~ 0
D22
Text GLabel 3050 6300 2    50   Input ~ 0
D21
Text GLabel 3050 6400 2    50   Input ~ 0
D19
Text GLabel 3050 6500 2    50   Input ~ 0
D18
Text GLabel 3050 6600 2    50   Input ~ 0
D5
Text GLabel 3050 6700 2    50   Input ~ 0
TX2
Text GLabel 3050 6800 2    50   Input ~ 0
RX2
Text GLabel 3050 6900 2    50   Input ~ 0
D4
Text GLabel 3050 7000 2    50   Input ~ 0
D2
Text GLabel 3050 7100 2    50   Input ~ 0
D15
Text GLabel 3050 7200 2    50   Input ~ 0
3V3
NoConn ~ 1900 6100
NoConn ~ 1950 6200
NoConn ~ 1950 6300
NoConn ~ 1950 6400
NoConn ~ 1950 6500
NoConn ~ 1950 6600
NoConn ~ 1950 6700
NoConn ~ 1950 6800
NoConn ~ 1950 6900
NoConn ~ 3050 6100
NoConn ~ 3050 6200
NoConn ~ 3050 6300
NoConn ~ 3050 6400
NoConn ~ 3050 6500
NoConn ~ 3050 6600
NoConn ~ 3050 6700
NoConn ~ 3050 6800
NoConn ~ 3050 6900
NoConn ~ 3050 7000
NoConn ~ 3050 7100
NoConn ~ 3050 7200
Wire Wire Line
	7850 2800 8100 2800
Wire Wire Line
	7900 2700 8100 2700
Wire Wire Line
	7850 2600 8100 2600
Wire Wire Line
	9750 2800 9750 2850
Wire Wire Line
	9600 2800 9750 2800
Wire Wire Line
	9950 2700 9950 3000
Wire Wire Line
	9600 2700 9950 2700
$Comp
L power:+5V #PWR0114
U 1 1 623FD4D5
P 9750 2850
F 0 "#PWR0114" H 9750 2700 50  0001 C CNN
F 1 "+5V" H 9765 3023 50  0000 C CNN
F 2 "" H 9750 2850 50  0001 C CNN
F 3 "" H 9750 2850 50  0001 C CNN
	1    9750 2850
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 623FCDC3
P 9950 3000
F 0 "#PWR0115" H 9950 2750 50  0001 C CNN
F 1 "GND" H 9955 2827 50  0000 C CNN
F 2 "" H 9950 3000 50  0001 C CNN
F 3 "" H 9950 3000 50  0001 C CNN
	1    9950 3000
	1    0    0    -1  
$EndComp
Text GLabel 10000 2600 2    50   Input ~ 0
D32_RELAY
Text GLabel 7900 2700 0    50   Input ~ 0
COM
Text GLabel 7850 2600 0    50   Input ~ 0
NC
Text GLabel 7850 2800 0    50   Input ~ 0
NO
$Comp
L Connector:Conn_01x03_Female J7
U 1 1 623E700B
P 9400 2700
F 0 "J7" H 9292 2985 50  0000 C CNN
F 1 "Control_Relay" H 9292 2894 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 9400 2700 50  0001 C CNN
F 3 "~" H 9400 2700 50  0001 C CNN
	1    9400 2700
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female J6
U 1 1 623E5D69
P 8300 2700
F 0 "J6" H 8328 2726 50  0000 L CNN
F 1 "Line_Relay" H 8328 2635 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 8300 2700 50  0001 C CNN
F 3 "~" H 8300 2700 50  0001 C CNN
	1    8300 2700
	1    0    0    -1  
$EndComp
Text Notes 7720 2240 0    50   ~ 10
Módulo Relay\n
$Comp
L Device:R R1
U 1 1 620FCE7F
P 5050 5500
F 0 "R1" V 5257 5500 50  0000 C CNN
F 1 "330" V 5166 5500 50  0000 C CNN
F 2 "Resistor_SMD:R_2010_5025Metric_Pad1.40x2.65mm_HandSolder" V 4980 5500 50  0001 C CNN
F 3 "~" H 5050 5500 50  0001 C CNN
	1    5050 5500
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D1
U 1 1 621083D3
P 5450 5500
F 0 "D1" H 5443 5717 50  0000 C CNN
F 1 "LED" H 5443 5626 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 5450 5500 50  0001 C CNN
F 3 "~" H 5450 5500 50  0001 C CNN
	1    5450 5500
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 6210BB6A
P 5750 5650
F 0 "#PWR0116" H 5750 5400 50  0001 C CNN
F 1 "GND" H 5755 5477 50  0000 C CNN
F 2 "" H 5750 5650 50  0001 C CNN
F 3 "" H 5750 5650 50  0001 C CNN
	1    5750 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 5500 5750 5500
Wire Wire Line
	5750 5500 5750 5650
Wire Wire Line
	5300 5500 5200 5500
Text GLabel 4800 5500 0    50   Input ~ 0
D27_LED
Wire Wire Line
	4800 5500 4900 5500
Text Notes 4600 5200 0    50   ~ 10
LED
Wire Notes Line
	4350 5050 4350 5950
Text GLabel 7850 3150 0    50   Input ~ 0
NC
NoConn ~ 7850 3150
$Comp
L Connector:Screw_Terminal_01x03 J12
U 1 1 621955F9
P 3100 2050
F 0 "J12" H 3018 2367 50  0000 C CNN
F 1 "Power_Out_Load" H 3018 2276 50  0000 C CNN
F 2 "6PCV-03-006:TE_5-1437652-4" H 3100 2050 50  0001 C CNN
F 3 "~" H 3100 2050 50  0001 C CNN
	1    3100 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 621D0A5D
P 2200 2250
F 0 "#PWR0117" H 2200 2000 50  0001 C CNN
F 1 "GND" H 2205 2077 50  0000 C CNN
F 2 "" H 2200 2250 50  0001 C CNN
F 3 "" H 2200 2250 50  0001 C CNN
	1    2200 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 2150 2200 2150
Connection ~ 2200 2150
Wire Wire Line
	2200 2150 2200 2250
Wire Wire Line
	2200 2150 2900 2150
Wire Wire Line
	2150 1850 2150 1950
Wire Wire Line
	1550 1950 1750 1950
Wire Wire Line
	2250 1850 2250 1950
Wire Wire Line
	2250 1950 2800 1950
Wire Wire Line
	2200 2050 2050 2050
Text GLabel 1950 1850 1    50   Input ~ 0
LINE
Text GLabel 2050 1850 1    50   Input ~ 0
PHASE
Wire Wire Line
	1950 1850 1950 1950
Connection ~ 1950 1950
Wire Wire Line
	1950 1950 2150 1950
Wire Wire Line
	2050 1850 2050 2050
Connection ~ 2050 2050
Wire Wire Line
	2050 2050 1750 2050
Text GLabel 5000 2450 0    50   Input ~ 0
LINE
Text GLabel 5000 2650 0    50   Input ~ 0
PHASE
Wire Wire Line
	5100 2650 5000 2650
Wire Wire Line
	5100 2450 5000 2450
Text Notes 4500 2150 0    50   ~ 10
Fuente de Alimentación HLK-PM01
Wire Notes Line
	4350 1950 4350 2950
Wire Notes Line
	4350 2950 6200 2950
Wire Notes Line
	6200 2950 6200 1950
Wire Notes Line
	6200 1950 4350 1950
NoConn ~ 5550 4700
NoConn ~ 5550 4600
Text GLabel 5550 4700 0    50   Input ~ 0
CTS
Text GLabel 5550 4600 0    50   Input ~ 0
VCC
NoConn ~ 5550 4500
Text GLabel 5550 4500 0    50   Input ~ 0
DTR
Wire Notes Line
	5850 5050 4350 5050
Wire Notes Line
	5850 5950 5850 5050
Wire Notes Line
	4350 5950 5850 5950
Wire Notes Line
	6000 3550 4300 3550
Wire Notes Line
	6000 4850 6000 3550
Wire Notes Line
	4300 4850 6000 4850
Wire Notes Line
	4300 3550 4300 4850
Wire Wire Line
	4900 3900 4700 3900
Wire Wire Line
	4900 4000 4700 4000
Wire Wire Line
	4900 4100 4700 4100
Wire Wire Line
	4900 4200 4700 4200
Wire Wire Line
	4900 4300 4700 4300
Wire Wire Line
	4700 4400 4700 4500
Wire Wire Line
	4900 4400 4700 4400
$Comp
L power:GND #PWR0108
U 1 1 623CD6E2
P 4700 4500
F 0 "#PWR0108" H 4700 4250 50  0001 C CNN
F 1 "GND" H 4705 4327 50  0000 C CNN
F 2 "" H 4700 4500 50  0001 C CNN
F 3 "" H 4700 4500 50  0001 C CNN
	1    4700 4500
	1    0    0    -1  
$EndComp
Text GLabel 4700 4300 0    50   Input ~ 0
CTS
Text GLabel 4700 4200 0    50   Input ~ 0
VCC
Text GLabel 4700 3900 0    50   Input ~ 0
DTR
$Comp
L Connector:Conn_01x06_Female J5
U 1 1 6239A494
P 5100 4100
F 0 "J5" H 5128 4076 50  0000 L CNN
F 1 "Pins_FT232R" H 5128 3985 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x06_P2.54mm_Vertical" H 5100 4100 50  0001 C CNN
F 3 "~" H 5100 4100 50  0001 C CNN
	1    5100 4100
	1    0    0    -1  
$EndComp
Text GLabel 4700 4100 0    50   Input ~ 0
TX0
Text GLabel 4700 4000 0    50   Input ~ 0
RX0
Text Notes 4720 3730 0    50   ~ 10
FT232R
Text Notes 2150 6500 0    50   ~ 10
Pines Sin Utilizar
Wire Notes Line
	1550 5900 1550 7300
Wire Notes Line
	1550 7300 3350 7300
Wire Notes Line
	3350 7300 3350 5900
Wire Notes Line
	3350 5900 1550 5900
Text Notes 2900 3450 0    50   ~ 10
Microcontrolador
Wire Notes Line
	600  3250 600  7350
Wire Notes Line
	6050 3250 600  3250
Text GLabel 4650 1400 0    50   Input ~ 0
PHASE
Text GLabel 4650 1300 0    50   Input ~ 0
LINE
Wire Notes Line
	10300 1850 10300 700 
Wire Notes Line
	7600 1850 7600 700 
Wire Notes Line
	7600 1850 10300 1850
Wire Wire Line
	9500 1250 9650 1250
Wire Wire Line
	10050 1150 10050 1400
Wire Wire Line
	9500 1150 10050 1150
Wire Wire Line
	9650 1350 9650 1450
Wire Wire Line
	9500 1350 9650 1350
$Comp
L power:GND #PWR0110
U 1 1 620E6BF1
P 10050 1400
F 0 "#PWR0110" H 10050 1150 50  0001 C CNN
F 1 "GND" H 10055 1227 50  0000 C CNN
F 2 "" H 10050 1400 50  0001 C CNN
F 3 "" H 10050 1400 50  0001 C CNN
	1    10050 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0109
U 1 1 620E6405
P 9650 1450
F 0 "#PWR0109" H 9650 1300 50  0001 C CNN
F 1 "+5V" H 9665 1623 50  0000 C CNN
F 2 "" H 9650 1450 50  0001 C CNN
F 3 "" H 9650 1450 50  0001 C CNN
	1    9650 1450
	1    0    0    1   
$EndComp
Text GLabel 9650 1250 2    50   Input ~ 0
VN_ACS
$Comp
L Connector:Conn_01x03_Female J10
U 1 1 620C97A5
P 9300 1250
F 0 "J10" H 9192 1535 50  0000 C CNN
F 1 "Control_ACS712" H 9192 1444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 9300 1250 50  0001 C CNN
F 3 "~" H 9300 1250 50  0001 C CNN
	1    9300 1250
	-1   0    0    -1  
$EndComp
Wire Notes Line
	7600 700  10300 700 
Wire Wire Line
	5850 1300 6150 1300
Text GLabel 6150 1300 2    50   Input ~ 0
VP_ZMPT
Wire Wire Line
	6050 1500 6050 1400
Connection ~ 6050 1500
Wire Wire Line
	5850 1500 6050 1500
Wire Wire Line
	6050 1400 5850 1400
Wire Wire Line
	6050 1600 6050 1500
Wire Wire Line
	6050 1200 5850 1200
Wire Wire Line
	6050 1050 6050 1200
$Comp
L power:GND #PWR0106
U 1 1 622EDE98
P 6050 1600
F 0 "#PWR0106" H 6050 1350 50  0001 C CNN
F 1 "GND" H 6055 1427 50  0000 C CNN
F 2 "" H 6050 1600 50  0001 C CNN
F 3 "" H 6050 1600 50  0001 C CNN
	1    6050 1600
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 622ED7BD
P 6050 1050
F 0 "#PWR0105" H 6050 900 50  0001 C CNN
F 1 "+5V" H 6065 1223 50  0000 C CNN
F 2 "" H 6050 1050 50  0001 C CNN
F 3 "" H 6050 1050 50  0001 C CNN
	1    6050 1050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Female J11
U 1 1 62243A42
P 5650 1300
F 0 "J11" H 5542 1585 50  0000 C CNN
F 1 "Control_ZMPT101B" H 5542 1494 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5650 1300 50  0001 C CNN
F 3 "~" H 5650 1300 50  0001 C CNN
	1    5650 1300
	-1   0    0    -1  
$EndComp
Wire Notes Line
	6570 1750 4200 1750
Wire Notes Line
	4200 1750 4200 740 
Text Notes 4280 910  0    50   ~ 0
Módulo ZMPT101B (Sensor Voltaje)\n
Text Notes 7960 880  0    50   ~ 10
Módulo ACS712 (Sensor Corriente)\n
Wire Notes Line
	6570 740  6570 1750
Wire Notes Line
	6050 7350 6050 3250
Wire Notes Line
	600  7350 6050 7350
Wire Notes Line
	4200 740  6570 740 
Connection ~ 1750 2050
Wire Wire Line
	1750 2050 1550 2050
Connection ~ 1750 1950
Wire Wire Line
	1750 1950 1950 1950
Wire Wire Line
	2650 2050 2800 2050
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 624F2829
P 2800 2050
F 0 "#FLG0103" H 2800 2125 50  0001 C CNN
F 1 "PWR_FLAG" H 2800 2223 50  0000 C CNN
F 2 "" H 2800 2050 50  0001 C CNN
F 3 "~" H 2800 2050 50  0001 C CNN
	1    2800 2050
	1    0    0    1   
$EndComp
Connection ~ 2800 2050
Wire Wire Line
	2800 2050 2900 2050
$Comp
L power:PWR_FLAG #FLG0104
U 1 1 624F3089
P 2800 1950
F 0 "#FLG0104" H 2800 2025 50  0001 C CNN
F 1 "PWR_FLAG" H 2800 2123 50  0000 C CNN
F 2 "" H 2800 1950 50  0001 C CNN
F 3 "~" H 2800 1950 50  0001 C CNN
	1    2800 1950
	1    0    0    -1  
$EndComp
Connection ~ 2800 1950
Wire Wire Line
	2800 1950 2900 1950
Wire Notes Line
	3800 1200 3800 2550
Wire Notes Line
	900  1200 900  2550
Connection ~ 8150 1300
$Comp
L power:PWR_FLAG #FLG0105
U 1 1 624FAE80
P 8150 1300
F 0 "#FLG0105" H 8150 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 8150 1473 50  0000 C CNN
F 2 "" H 8150 1300 50  0001 C CNN
F 3 "~" H 8150 1300 50  0001 C CNN
	1    8150 1300
	1    0    0    1   
$EndComp
Text GLabel 8000 1200 0    50   Input ~ 0
I_ACS
Text GLabel 8000 1300 0    50   Input ~ 0
COM
Wire Wire Line
	8000 1300 8150 1300
Wire Wire Line
	8150 1300 8200 1300
Wire Wire Line
	8000 1200 8200 1200
$Comp
L Connector:Conn_01x02_Female J8
U 1 1 621188AB
P 8400 1200
F 0 "J8" H 8428 1176 50  0000 L CNN
F 1 "Line_ACS712" H 8428 1085 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 8400 1200 50  0001 C CNN
F 3 "~" H 8400 1200 50  0001 C CNN
	1    8400 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1400 4650 1400
Wire Wire Line
	4950 1300 4650 1300
$Comp
L Connector:Conn_01x02_Female J9
U 1 1 62231822
P 5150 1300
F 0 "J9" H 5178 1276 50  0000 L CNN
F 1 "Line_ZMPT101B" H 5178 1185 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 5150 1300 50  0001 C CNN
F 3 "~" H 5150 1300 50  0001 C CNN
	1    5150 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 2600 10000 2600
Wire Notes Line
	7400 2050 10650 2050
Wire Notes Line
	10650 2050 10650 3450
Wire Notes Line
	7400 2050 7400 3450
Wire Notes Line
	7400 3450 10650 3450
$Comp
L Connector:Conn_01x03_Male J4
U 1 1 6216C21C
P 2250 1650
F 0 "J4" V 2312 1794 50  0000 L CNN
F 1 "Switch" V 2403 1794 50  0000 L CNN
F 2 "Connector_Wire:SolderWire-0.75sqmm_1x03_P4.8mm_D1.25mm_OD2.3mm" H 2250 1650 50  0001 C CNN
F 3 "~" H 2250 1650 50  0001 C CNN
	1    2250 1650
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0111
U 1 1 62178CC2
P 2550 1700
F 0 "#PWR0111" H 2550 1550 50  0001 C CNN
F 1 "+5V" H 2565 1873 50  0000 C CNN
F 2 "" H 2550 1700 50  0001 C CNN
F 3 "" H 2550 1700 50  0001 C CNN
	1    2550 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 1850 2350 1900
Wire Wire Line
	2350 1900 2550 1900
Wire Wire Line
	2550 1900 2550 1700
$EndSCHEMATC
