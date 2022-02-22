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
VP_ACS
Text GLabel 1450 4350 0    50   Input ~ 0
VN_ZMPT
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
D27
Text GLabel 1450 5150 0    50   Input ~ 0
D14
Text GLabel 1450 5250 0    50   Input ~ 0
D12
Text GLabel 1450 5350 0    50   Input ~ 0
D13
Text GLabel 3750 4150 2    50   Input ~ 0
D23_LED
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
P 1850 2600
F 0 "PS1" H 1850 2925 50  0000 C CNN
F 1 "HLK-PM01" H 1850 2834 50  0000 C CNN
F 2 "Converter_ACDC:Converter_ACDC_HiLink_HLK-PMxx" H 1850 2300 50  0001 C CNN
F 3 "http://www.hlktech.net/product_detail.php?ProId=54" H 2250 2250 50  0001 C CNN
	1    1850 2600
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 622CF9F0
P 1250 1200
F 0 "#FLG0101" H 1250 1275 50  0001 C CNN
F 1 "PWR_FLAG" H 1250 1373 50  0000 C CNN
F 2 "" H 1250 1200 50  0001 C CNN
F 3 "~" H 1250 1200 50  0001 C CNN
	1    1250 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0103
U 1 1 622DB783
P 2950 2400
F 0 "#PWR0103" H 2950 2250 50  0001 C CNN
F 1 "+5V" H 2965 2573 50  0000 C CNN
F 2 "" H 2950 2400 50  0001 C CNN
F 3 "" H 2950 2400 50  0001 C CNN
	1    2950 2400
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 622DC1F4
P 2400 2750
F 0 "#PWR0104" H 2400 2500 50  0001 C CNN
F 1 "GND" H 2405 2577 50  0000 C CNN
F 2 "" H 2400 2750 50  0001 C CNN
F 3 "" H 2400 2750 50  0001 C CNN
	1    2400 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 2700 2400 2700
Wire Wire Line
	2400 2700 2400 2750
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
Text Notes 1000 800  0    50   ~ 10
Alimentación y Carga
$Comp
L Connector:Screw_Terminal_01x03 J3
U 1 1 6243119F
P 850 1300
F 0 "J3" H 768 1617 50  0000 C CNN
F 1 "Power_In" H 768 1526 50  0000 C CNN
F 2 "6PCV-03-006:TE_5-1437652-4" H 850 1300 50  0001 C CNN
F 3 "~" H 850 1300 50  0001 C CNN
	1    850  1300
	-1   0    0    -1  
$EndComp
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
Text GLabel 1950 6800 0    50   Input ~ 0
D14
Text GLabel 1950 6900 0    50   Input ~ 0
D12
Text GLabel 1950 7000 0    50   Input ~ 0
D13
Text GLabel 3050 6100 2    50   Input ~ 0
D22
Text GLabel 3050 6200 2    50   Input ~ 0
D21
Text GLabel 3050 6300 2    50   Input ~ 0
D19
Text GLabel 3050 6400 2    50   Input ~ 0
D18
Text GLabel 3050 6500 2    50   Input ~ 0
D5
Text GLabel 3050 6600 2    50   Input ~ 0
TX2
Text GLabel 3050 6700 2    50   Input ~ 0
RX2
Text GLabel 3050 6800 2    50   Input ~ 0
D4
Text GLabel 3050 6900 2    50   Input ~ 0
D2
Text GLabel 3050 7000 2    50   Input ~ 0
D15
Text GLabel 3050 7100 2    50   Input ~ 0
3V3
NoConn ~ 1900 6100
NoConn ~ 1950 6200
NoConn ~ 1950 6300
NoConn ~ 1950 6400
NoConn ~ 1950 6500
NoConn ~ 1950 6600
NoConn ~ 1950 6800
NoConn ~ 1950 6900
NoConn ~ 1950 7000
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
Wire Wire Line
	3800 2550 4050 2550
Wire Wire Line
	3850 2450 4050 2450
Wire Wire Line
	3800 2350 4050 2350
Wire Wire Line
	5900 2450 5900 2750
Wire Wire Line
	5550 2450 5900 2450
$Comp
L power:+5V #PWR0114
U 1 1 623FD4D5
P 5700 2600
F 0 "#PWR0114" H 5700 2450 50  0001 C CNN
F 1 "+5V" H 5715 2773 50  0000 C CNN
F 2 "" H 5700 2600 50  0001 C CNN
F 3 "" H 5700 2600 50  0001 C CNN
	1    5700 2600
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 623FCDC3
P 5900 2750
F 0 "#PWR0115" H 5900 2500 50  0001 C CNN
F 1 "GND" H 5905 2577 50  0000 C CNN
F 2 "" H 5900 2750 50  0001 C CNN
F 3 "" H 5900 2750 50  0001 C CNN
	1    5900 2750
	1    0    0    -1  
$EndComp
Text GLabel 5950 2350 2    50   Input ~ 0
D32_RELAY
Text GLabel 3850 2450 0    50   Input ~ 0
COM
Text GLabel 3800 2350 0    50   Input ~ 0
NC
Text GLabel 3800 2550 0    50   Input ~ 0
NO
$Comp
L Connector:Conn_01x03_Female J7
U 1 1 623E700B
P 5350 2450
F 0 "J7" H 5242 2735 50  0000 C CNN
F 1 "Control_Relay" H 5242 2644 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 5350 2450 50  0001 C CNN
F 3 "~" H 5350 2450 50  0001 C CNN
	1    5350 2450
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female J6
U 1 1 623E5D69
P 4250 2450
F 0 "J6" H 4278 2476 50  0000 L CNN
F 1 "Line_Relay" H 4278 2385 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 4250 2450 50  0001 C CNN
F 3 "~" H 4250 2450 50  0001 C CNN
	1    4250 2450
	1    0    0    -1  
$EndComp
Text Notes 3670 1990 0    50   ~ 10
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
D23_LED
Wire Wire Line
	4800 5500 4900 5500
Text Notes 4600 5200 0    50   ~ 10
LED
Wire Notes Line
	4350 5050 4350 5950
Text GLabel 3800 2900 0    50   Input ~ 0
NC
NoConn ~ 3800 2900
Wire Wire Line
	1050 1200 1250 1200
Wire Wire Line
	1450 2700 1350 2700
Wire Wire Line
	1450 2500 1350 2500
Text Notes 850  2200 0    50   ~ 10
Fuente de Alimentación HLK-PM01
Wire Notes Line
	700  2000 700  3000
Wire Notes Line
	3050 3000 3050 2000
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
RX0
Text GLabel 4700 4000 0    50   Input ~ 0
TX0
Text Notes 4720 3730 0    50   ~ 10
FT232R
Text Notes 2150 6500 0    50   ~ 10
Pines Sin Utilizar
Wire Notes Line
	1550 5900 1550 7300
Wire Notes Line
	1550 7200 3350 7200
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
Wire Notes Line
	4850 1750 4850 600 
Wire Wire Line
	6900 1050 6900 950 
Wire Wire Line
	6750 1050 6900 1050
$Comp
L power:GND #PWR0110
U 1 1 620E6BF1
P 6900 1450
F 0 "#PWR0110" H 6900 1200 50  0001 C CNN
F 1 "GND" H 6905 1277 50  0000 C CNN
F 2 "" H 6900 1450 50  0001 C CNN
F 3 "" H 6900 1450 50  0001 C CNN
	1    6900 1450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0109
U 1 1 620E6405
P 6900 950
F 0 "#PWR0109" H 6900 800 50  0001 C CNN
F 1 "+5V" H 6915 1123 50  0000 C CNN
F 2 "" H 6900 950 50  0001 C CNN
F 3 "" H 6900 950 50  0001 C CNN
	1    6900 950 
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female J10
U 1 1 620C97A5
P 6550 1150
F 0 "J10" H 6442 1435 50  0000 C CNN
F 1 "Control_ACS712" H 6442 1344 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6550 1150 50  0001 C CNN
F 3 "~" H 6550 1150 50  0001 C CNN
	1    6550 1150
	-1   0    0    1   
$EndComp
Wire Wire Line
	4000 1250 4300 1250
Text GLabel 4300 1250 2    50   Input ~ 0
VN_ZMPT
Wire Wire Line
	4200 1450 4200 1350
Connection ~ 4200 1450
Wire Wire Line
	4000 1450 4200 1450
Wire Wire Line
	4200 1350 4000 1350
Wire Wire Line
	4200 1550 4200 1450
Wire Wire Line
	4200 1150 4000 1150
Wire Wire Line
	4200 1000 4200 1150
$Comp
L power:GND #PWR0106
U 1 1 622EDE98
P 4200 1550
F 0 "#PWR0106" H 4200 1300 50  0001 C CNN
F 1 "GND" H 4205 1377 50  0000 C CNN
F 2 "" H 4200 1550 50  0001 C CNN
F 3 "" H 4200 1550 50  0001 C CNN
	1    4200 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 622ED7BD
P 4200 1000
F 0 "#PWR0105" H 4200 850 50  0001 C CNN
F 1 "+5V" H 4215 1173 50  0000 C CNN
F 2 "" H 4200 1000 50  0001 C CNN
F 3 "" H 4200 1000 50  0001 C CNN
	1    4200 1000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Female J11
U 1 1 62243A42
P 3800 1250
F 0 "J11" H 3692 1535 50  0000 C CNN
F 1 "Control_ZMPT101B" H 3692 1444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3800 1250 50  0001 C CNN
F 3 "~" H 3800 1250 50  0001 C CNN
	1    3800 1250
	-1   0    0    -1  
$EndComp
Wire Notes Line
	4720 1700 2350 1700
Wire Notes Line
	2350 1700 2350 690 
Text Notes 2430 860  0    50   ~ 0
Módulo ZMPT101B (Sensor Voltaje)\n
Text Notes 5210 780  0    50   ~ 10
Módulo ACS712 (Sensor Corriente)\n
Wire Notes Line
	4720 690  4720 1700
Wire Notes Line
	6050 7350 6050 3250
Wire Notes Line
	600  7350 6050 7350
Wire Notes Line
	2350 690  4720 690 
Wire Notes Line
	700  600  700  1950
Wire Wire Line
	5250 1100 5450 1100
Wire Wire Line
	3100 1350 2800 1350
Wire Wire Line
	3100 1250 2800 1250
$Comp
L Connector:Conn_01x02_Female J9
U 1 1 62231822
P 3300 1250
F 0 "J9" H 3328 1226 50  0000 L CNN
F 1 "Line_ZMPT101B" H 3328 1135 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 3300 1250 50  0001 C CNN
F 3 "~" H 3300 1250 50  0001 C CNN
	1    3300 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2350 5950 2350
Wire Notes Line
	3350 1800 6600 1800
Wire Notes Line
	6600 1800 6600 3200
Wire Notes Line
	3350 1800 3350 3200
Wire Notes Line
	3350 3200 6600 3200
Text GLabel 5250 1100 0    50   Input ~ 0
NO
Text GLabel 1950 6700 0    50   Input ~ 0
D27
NoConn ~ 1950 6700
Wire Wire Line
	1400 1200 1250 1200
Connection ~ 1250 1200
Wire Notes Line
	700  1950 2200 1950
Wire Notes Line
	2200 600  700  600 
$Comp
L Connector:Conn_01x03_Male J4
U 1 1 6216C21C
P 2500 2200
F 0 "J4" V 2562 2344 50  0000 L CNN
F 1 "Switch" V 2653 2344 50  0000 L CNN
F 2 "Connector_Wire:SolderWire-0.75sqmm_1x03_P4.8mm_D1.25mm_OD2.3mm" H 2500 2200 50  0001 C CNN
F 3 "~" H 2500 2200 50  0001 C CNN
	1    2500 2200
	0    1    1    0   
$EndComp
Wire Notes Line
	700  2000 3050 2000
Wire Notes Line
	700  3000 3050 3000
Wire Wire Line
	2600 2400 2600 2500
Wire Wire Line
	2600 2500 2250 2500
Wire Wire Line
	2500 2400 2500 2450
Wire Wire Line
	2500 2450 2950 2450
Wire Wire Line
	2950 2450 2950 2400
Wire Wire Line
	5550 2550 5700 2550
Wire Wire Line
	5700 2550 5700 2600
$Comp
L Connector:Conn_01x02_Female J8
U 1 1 621188AB
P 5650 1100
F 0 "J8" H 5678 1076 50  0000 L CNN
F 1 "Line_ACS712" H 5678 985 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 5650 1100 50  0001 C CNN
F 3 "~" H 5650 1100 50  0001 C CNN
	1    5650 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1200 5450 1200
Wire Notes Line
	2200 600  2200 1950
Text GLabel 1400 1200 2    50   Input ~ 0
COM
Text GLabel 1350 2700 0    50   Input ~ 0
NEUTRAL
Text GLabel 1350 2500 0    50   Input ~ 0
LINE
Text GLabel 5250 1200 0    50   Input ~ 0
LINE
Text GLabel 2800 1350 0    50   Input ~ 0
NEUTRAL
Text GLabel 2800 1250 0    50   Input ~ 0
LINE
Wire Wire Line
	1400 1400 1250 1400
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 622D0110
P 1250 1400
F 0 "#FLG0102" H 1250 1475 50  0001 C CNN
F 1 "PWR_FLAG" H 1250 1573 50  0000 C CNN
F 2 "" H 1250 1400 50  0001 C CNN
F 3 "~" H 1250 1400 50  0001 C CNN
	1    1250 1400
	1    0    0    1   
$EndComp
Text GLabel 1400 1400 2    50   Input ~ 0
NEUTRAL
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 62127B9A
P 1900 1300
F 0 "#FLG0103" H 1900 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 1900 1473 50  0000 C CNN
F 2 "" H 1900 1300 50  0001 C CNN
F 3 "~" H 1900 1300 50  0001 C CNN
	1    1900 1300
	1    0    0    1   
$EndComp
Wire Wire Line
	2000 1300 1900 1300
Wire Wire Line
	1800 1300 1900 1300
Connection ~ 1900 1300
$Comp
L Device:Fuse F1
U 1 1 6211FE5D
P 1300 1300
F 0 "F1" V 1103 1300 50  0000 C CNN
F 1 "Fuse" V 1194 1300 50  0000 C CNN
F 2 "Fuse:Fuse_Littelfuse_395Series" V 1230 1300 50  0001 C CNN
F 3 "~" H 1300 1300 50  0001 C CNN
	1    1300 1300
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F2
U 1 1 6214DC3E
P 1650 1300
F 0 "F2" V 1453 1300 50  0000 C CNN
F 1 "Fuse" V 1544 1300 50  0000 C CNN
F 2 "Fuse:Fuse_Littelfuse_395Series" V 1580 1300 50  0001 C CNN
F 3 "~" H 1650 1300 50  0001 C CNN
	1    1650 1300
	0    1    1    0   
$EndComp
Text GLabel 2000 1300 2    50   Input ~ 0
LINE
Wire Wire Line
	1050 1300 1150 1300
Wire Wire Line
	1050 1400 1250 1400
Connection ~ 1250 1400
Wire Wire Line
	6750 1250 6900 1250
Wire Wire Line
	6900 1250 6900 1450
$Comp
L Device:Jumper JP1
U 1 1 621BDC2F
P 7300 1150
F 0 "JP1" H 7300 1414 50  0000 C CNN
F 1 "Jumper" H 7300 1323 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7300 1150 50  0001 C CNN
F 3 "~" H 7300 1150 50  0001 C CNN
	1    7300 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 1150 7000 1150
Wire Notes Line
	4850 600  9000 600 
Wire Notes Line
	4850 1750 9000 1750
Wire Notes Line
	9000 600  9000 1750
Text GLabel 7850 1150 2    50   Input ~ 0
VP_ACS
Wire Wire Line
	7600 1150 7850 1150
$Comp
L power:PWR_FLAG #FLG0104
U 1 1 62219A74
P 2950 2450
F 0 "#FLG0104" H 2950 2525 50  0001 C CNN
F 1 "PWR_FLAG" H 2950 2623 50  0000 C CNN
F 2 "" H 2950 2450 50  0001 C CNN
F 3 "~" H 2950 2450 50  0001 C CNN
	1    2950 2450
	1    0    0    1   
$EndComp
Connection ~ 2950 2450
$Comp
L power:GND #PWR0111
U 1 1 62227D4B
P 2400 2400
F 0 "#PWR0111" H 2400 2150 50  0001 C CNN
F 1 "GND" H 2405 2227 50  0000 C CNN
F 2 "" H 2400 2400 50  0001 C CNN
F 3 "" H 2400 2400 50  0001 C CNN
	1    2400 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1300 1500 1250
Wire Wire Line
	1500 1250 1800 1250
Wire Wire Line
	1800 1250 1800 1300
Connection ~ 1800 1300
Wire Wire Line
	1450 1300 1450 1250
Wire Wire Line
	1450 1250 1150 1250
Wire Wire Line
	1150 1250 1150 1300
Connection ~ 1150 1300
$EndSCHEMATC
