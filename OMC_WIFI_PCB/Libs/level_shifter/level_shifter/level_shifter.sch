EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:R R1
U 1 1 626B1110
P 4850 1750
F 0 "R1" H 4920 1796 50  0000 L CNN
F 1 "10k" H 4920 1705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4780 1750 50  0001 C CNN
F 3 "~" H 4850 1750 50  0001 C CNN
	1    4850 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 626B48FD
P 5650 1750
F 0 "R5" H 5720 1796 50  0000 L CNN
F 1 "10k" H 5720 1705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5580 1750 50  0001 C CNN
F 3 "~" H 5650 1750 50  0001 C CNN
	1    5650 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 1950 4850 1950
Wire Wire Line
	4850 1950 4850 1900
Wire Wire Line
	5450 1950 5650 1950
Wire Wire Line
	5650 1950 5650 1900
Wire Wire Line
	4850 1600 4850 1550
Wire Wire Line
	4850 1550 5250 1550
Wire Wire Line
	5250 1550 5250 1650
$Comp
L Transistor_FET:BSS138 Q1
U 1 1 626AAF8D
P 5250 1850
F 0 "Q1" V 5454 1896 50  0000 L CNN
F 1 "BSS138" V 5454 1805 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5450 1775 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BSS138-D.PDF" H 5250 1850 50  0001 L CNN
	1    5250 1850
	0    1    1    0   
$EndComp
Text GLabel 4850 1500 1    50   Input ~ 0
3V
Text GLabel 5650 1500 1    50   Input ~ 0
5V
Wire Wire Line
	4850 1500 4850 1550
Connection ~ 4850 1550
Wire Wire Line
	5650 1600 5650 1500
Wire Wire Line
	4800 1950 4850 1950
Connection ~ 4850 1950
Text GLabel 5700 1950 2    50   Input ~ 0
HV1
Wire Wire Line
	5650 1950 5700 1950
Connection ~ 5650 1950
$Comp
L Device:R R2
U 1 1 626C783F
P 4850 2550
F 0 "R2" H 4920 2596 50  0000 L CNN
F 1 "10k" H 4920 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4780 2550 50  0001 C CNN
F 3 "~" H 4850 2550 50  0001 C CNN
	1    4850 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 626C7845
P 5650 2550
F 0 "R6" H 5720 2596 50  0000 L CNN
F 1 "10k" H 5720 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5580 2550 50  0001 C CNN
F 3 "~" H 5650 2550 50  0001 C CNN
	1    5650 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 2750 4850 2750
Wire Wire Line
	4850 2750 4850 2700
Wire Wire Line
	5450 2750 5650 2750
Wire Wire Line
	5650 2750 5650 2700
Wire Wire Line
	4850 2400 4850 2350
Wire Wire Line
	4850 2350 5250 2350
Wire Wire Line
	5250 2350 5250 2450
$Comp
L Transistor_FET:BSS138 Q2
U 1 1 626C7852
P 5250 2650
F 0 "Q2" V 5454 2696 50  0000 L CNN
F 1 "BSS138" V 5454 2605 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5450 2575 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BSS138-D.PDF" H 5250 2650 50  0001 L CNN
	1    5250 2650
	0    1    1    0   
$EndComp
Text GLabel 4850 2300 1    50   Input ~ 0
3V
Text GLabel 5650 2300 1    50   Input ~ 0
5V
Wire Wire Line
	4850 2300 4850 2350
Connection ~ 4850 2350
Wire Wire Line
	5650 2400 5650 2300
Text GLabel 4800 2750 0    50   Input ~ 0
LV2
Wire Wire Line
	4800 2750 4850 2750
Connection ~ 4850 2750
Text GLabel 5700 2750 2    50   Input ~ 0
HV2
Wire Wire Line
	5650 2750 5700 2750
Connection ~ 5650 2750
$Comp
L Device:R R3
U 1 1 626CA572
P 4850 3350
F 0 "R3" H 4920 3396 50  0000 L CNN
F 1 "10k" H 4920 3305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4780 3350 50  0001 C CNN
F 3 "~" H 4850 3350 50  0001 C CNN
	1    4850 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 626CA578
P 5650 3350
F 0 "R7" H 5720 3396 50  0000 L CNN
F 1 "10k" H 5720 3305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5580 3350 50  0001 C CNN
F 3 "~" H 5650 3350 50  0001 C CNN
	1    5650 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3550 4850 3550
Wire Wire Line
	4850 3550 4850 3500
Wire Wire Line
	5450 3550 5650 3550
Wire Wire Line
	5650 3550 5650 3500
Wire Wire Line
	4850 3200 4850 3150
Wire Wire Line
	4850 3150 5250 3150
Wire Wire Line
	5250 3150 5250 3250
$Comp
L Transistor_FET:BSS138 Q3
U 1 1 626CA585
P 5250 3450
F 0 "Q3" V 5454 3496 50  0000 L CNN
F 1 "BSS138" V 5454 3405 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5450 3375 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BSS138-D.PDF" H 5250 3450 50  0001 L CNN
	1    5250 3450
	0    1    1    0   
$EndComp
Text GLabel 4850 3100 1    50   Input ~ 0
3V
Text GLabel 5650 3100 1    50   Input ~ 0
5V
Wire Wire Line
	4850 3100 4850 3150
Connection ~ 4850 3150
Wire Wire Line
	5650 3200 5650 3100
Text GLabel 4800 3550 0    50   Input ~ 0
LV3
Wire Wire Line
	4800 3550 4850 3550
Connection ~ 4850 3550
Text GLabel 5700 3550 2    50   Input ~ 0
HV3
Wire Wire Line
	5650 3550 5700 3550
Connection ~ 5650 3550
$Comp
L Device:R R4
U 1 1 626CF4B1
P 4850 4250
F 0 "R4" H 4920 4296 50  0000 L CNN
F 1 "10k" H 4920 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4780 4250 50  0001 C CNN
F 3 "~" H 4850 4250 50  0001 C CNN
	1    4850 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 626CF4B7
P 5650 4250
F 0 "R8" H 5720 4296 50  0000 L CNN
F 1 "10k" H 5720 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5580 4250 50  0001 C CNN
F 3 "~" H 5650 4250 50  0001 C CNN
	1    5650 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 4450 4850 4450
Wire Wire Line
	4850 4450 4850 4400
Wire Wire Line
	5450 4450 5650 4450
Wire Wire Line
	5650 4450 5650 4400
Wire Wire Line
	4850 4100 4850 4050
Wire Wire Line
	4850 4050 5250 4050
Wire Wire Line
	5250 4050 5250 4150
$Comp
L Transistor_FET:BSS138 Q4
U 1 1 626CF4C4
P 5250 4350
F 0 "Q4" V 5454 4396 50  0000 L CNN
F 1 "BSS138" V 5454 4305 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5450 4275 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BSS138-D.PDF" H 5250 4350 50  0001 L CNN
	1    5250 4350
	0    1    1    0   
$EndComp
Text GLabel 4850 4000 1    50   Input ~ 0
3V
Text GLabel 5650 4000 1    50   Input ~ 0
5V
Wire Wire Line
	4850 4000 4850 4050
Connection ~ 4850 4050
Wire Wire Line
	5650 4100 5650 4000
Text GLabel 4800 4450 0    50   Input ~ 0
LV4
Wire Wire Line
	4800 4450 4850 4450
Connection ~ 4850 4450
Text GLabel 5700 4450 2    50   Input ~ 0
HV4
Wire Wire Line
	5650 4450 5700 4450
Connection ~ 5650 4450
Text GLabel 4800 1950 0    50   Input ~ 0
LV1
Text GLabel 6850 2150 0    50   Input ~ 0
LV2
Text GLabel 6850 2450 0    50   Input ~ 0
LV3
Text GLabel 6850 2550 0    50   Input ~ 0
LV4
Text GLabel 6850 2050 0    50   Input ~ 0
LV1
$Comp
L Connector:Conn_01x06_Male J2
U 1 1 626E28C7
P 7550 2250
F 0 "J2" H 7658 2631 50  0000 C CNN
F 1 "Conn_01x06_Male" H 7658 2540 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7550 2250 50  0001 C CNN
F 3 "~" H 7550 2250 50  0001 C CNN
	1    7550 2250
	1    0    0    -1  
$EndComp
Text GLabel 6850 2250 0    50   Input ~ 0
3V
Text GLabel 7750 2250 2    50   Input ~ 0
5V
Text GLabel 6850 2350 0    50   Input ~ 0
GND
Text GLabel 7750 2350 2    50   Input ~ 0
GND
Text GLabel 7750 2050 2    50   Input ~ 0
HV1
Text GLabel 7750 2150 2    50   Input ~ 0
HV2
Text GLabel 7750 2450 2    50   Input ~ 0
HV3
Text GLabel 7750 2550 2    50   Input ~ 0
HV4
$Comp
L Connector:Conn_01x06_Male J1
U 1 1 626E0154
P 7050 2250
F 0 "J1" H 7022 2224 50  0000 R CNN
F 1 "Conn_01x06_Male" H 7022 2133 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7050 2250 50  0001 C CNN
F 3 "~" H 7050 2250 50  0001 C CNN
	1    7050 2250
	-1   0    0    -1  
$EndComp
$EndSCHEMATC
