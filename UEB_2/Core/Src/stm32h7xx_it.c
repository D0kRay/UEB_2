/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//operation Modes
#define turnOff					0
#define do_nothing				99
#define start_threePhaseMode	1
#define do_threePhaseMode		10
#define start_DCMode			2
#define do_DCMode				20
#define start_calibrateADC		3
#define do_calibrateADC			30
#define start_control			8
#define do_control				80

//Errors
#define no_Error				0
#define invalidValues_Error		1
#define overcurrent_Error		2
#define watchdog_Error			3
#define opticalFiber_Error		4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern float counter;
//all "extern" variables have their origin and are defined in "main.c"
//Variables needed for managing the program structure, used in TIM4
uint8_t 					dipPositionPrevious = 0; //the value of the previous Dip Switch position (0 to 15)
uint8_t 					dipPositionCurrent = 0;  //the value of the current Dip Switch position (0 to 15)
uint8_t 					operationMode = 0;		 //the value adjusts in which mode the system works / the motor is operated
extern uint8_t 				error;
extern uint8_t				motor_status;

//Variables for the PWM signal, generating Sin and AC.
//general:
extern bool 				rotationDirectionCW;
extern bool 				enableSoftstarter;
float 						softstarter = 0;
extern float				softstarterDuration;
extern float 				v_bridge_uf;
extern float 				voltage_ref;
extern float				maxTensionRelationship;

uint64_t sineTable[1800]=
{
	 903, 906, 909, 913, 916, 919, 922, 925, 928, 931, 935, 938, 941, 944, 947, 950, 953, 957, 960, 963,
	 966, 969, 972, 975, 978, 982, 985, 988, 991, 994, 997,1000,1003,1007,1010,1013,1016,1019,1022,1025,
	1028,1031,1035,1038,1041,1044,1047,1050,1053,1056,1059,1062,1066,1069,1072,1075,1078,1081,1084,1087,
	1090,1093,1096,1099,1102,1106,1109,1112,1115,1118,1121,1124,1127,1130,1133,1136,1139,1142,1145,1148,
	1151,1154,1157,1160,1163,1166,1169,1172,1175,1178,1181,1184,1187,1190,1193,1196,1199,1202,1205,1208,
	1211,1214,1217,1220,1223,1225,1228,1231,1234,1237,1240,1243,1246,1249,1252,1255,1257,1260,1263,1266,
	1269,1272,1275,1278,1280,1283,1286,1289,1292,1295,1297,1300,1303,1306,1309,1311,1314,1317,1320,1323,
	1325,1328,1331,1334,1336,1339,1342,1345,1347,1350,1353,1355,1358,1361,1364,1366,1369,1372,1374,1377,
	1380,1382,1385,1388,1390,1393,1395,1398,1401,1403,1406,1408,1411,1414,1416,1419,1421,1424,1426,1429,
	1432,1434,1437,1439,1442,1444,1447,1449,1452,1454,1457,1459,1461,1464,1466,1469,1471,1474,1476,1479,
	1481,1483,1486,1488,1490,1493,1495,1498,1500,1502,1505,1507,1509,1511,1514,1516,1518,1521,1523,1525,
	1527,1530,1532,1534,1536,1539,1541,1543,1545,1547,1550,1552,1554,1556,1558,1560,1562,1565,1567,1569,
	1571,1573,1575,1577,1579,1581,1583,1585,1587,1589,1591,1593,1595,1597,1599,1601,1603,1605,1607,1609,
	1611,1613,1615,1617,1619,1621,1623,1624,1626,1628,1630,1632,1634,1635,1637,1639,1641,1643,1644,1646,
	1648,1650,1651,1653,1655,1657,1658,1660,1662,1663,1665,1667,1668,1670,1671,1673,1675,1676,1678,1679,
	1681,1683,1684,1686,1687,1689,1690,1692,1693,1695,1696,1698,1699,1700,1702,1703,1705,1706,1708,1709,
	1710,1712,1713,1714,1716,1717,1718,1720,1721,1722,1723,1725,1726,1727,1728,1730,1731,1732,1733,1734,
	1736,1737,1738,1739,1740,1741,1742,1744,1745,1746,1747,1748,1749,1750,1751,1752,1753,1754,1755,1756,
	1757,1758,1759,1760,1761,1762,1762,1763,1764,1765,1766,1767,1768,1769,1769,1770,1771,1772,1773,1773,
	1774,1775,1775,1776,1777,1778,1778,1779,1780,1780,1781,1782,1782,1783,1783,1784,1785,1785,1786,1786,
	1787,1787,1788,1788,1789,1789,1790,1790,1791,1791,1792,1792,1793,1793,1793,1794,1794,1794,1795,1795,
	1795,1796,1796,1796,1797,1797,1797,1797,1798,1798,1798,1798,1798,1799,1799,1799,1799,1799,1799,1799,
	1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1800,1799,
	1799,1799,1799,1799,1799,1799,1798,1798,1798,1798,1798,1797,1797,1797,1797,1796,1796,1796,1795,1795,
	1795,1794,1794,1794,1793,1793,1793,1792,1792,1791,1791,1790,1790,1789,1789,1788,1788,1787,1787,1786,
	1786,1785,1785,1784,1783,1783,1782,1782,1781,1780,1780,1779,1778,1778,1777,1776,1775,1775,1774,1773,
	1773,1772,1771,1770,1769,1769,1768,1767,1766,1765,1764,1763,1762,1762,1761,1760,1759,1758,1757,1756,
	1755,1754,1753,1752,1751,1750,1749,1748,1747,1746,1745,1744,1742,1741,1740,1739,1738,1737,1736,1734,
	1733,1732,1731,1730,1728,1727,1726,1725,1723,1722,1721,1720,1718,1717,1716,1714,1713,1712,1710,1709,
	1708,1706,1705,1703,1702,1700,1699,1698,1696,1695,1693,1692,1690,1689,1687,1686,1684,1683,1681,1679,
	1678,1676,1675,1673,1671,1670,1668,1667,1665,1663,1662,1660,1658,1657,1655,1653,1651,1650,1648,1646,
	1644,1643,1641,1639,1637,1635,1634,1632,1630,1628,1626,1624,1623,1621,1619,1617,1615,1613,1611,1609,
	1607,1605,1603,1601,1599,1597,1595,1593,1591,1589,1587,1585,1583,1581,1579,1577,1575,1573,1571,1569,
	1567,1565,1562,1560,1558,1556,1554,1552,1550,1547,1545,1543,1541,1539,1536,1534,1532,1530,1527,1525,
	1523,1521,1518,1516,1514,1511,1509,1507,1505,1502,1500,1498,1495,1493,1490,1488,1486,1483,1481,1479,
	1476,1474,1471,1469,1466,1464,1461,1459,1457,1454,1452,1449,1447,1444,1442,1439,1437,1434,1432,1429,
	1426,1424,1421,1419,1416,1414,1411,1408,1406,1403,1401,1398,1395,1393,1390,1388,1385,1382,1380,1377,
	1374,1372,1369,1366,1364,1361,1358,1355,1353,1350,1347,1345,1342,1339,1336,1334,1331,1328,1325,1323,
	1320,1317,1314,1311,1309,1306,1303,1300,1297,1295,1292,1289,1286,1283,1280,1278,1275,1272,1269,1266,
	1263,1260,1257,1255,1252,1249,1246,1243,1240,1237,1234,1231,1228,1225,1223,1220,1217,1214,1211,1208,
	1205,1202,1199,1196,1193,1190,1187,1184,1181,1178,1175,1172,1169,1166,1163,1160,1157,1154,1151,1148,
	1145,1142,1139,1136,1133,1130,1127,1124,1121,1118,1115,1112,1109,1106,1102,1099,1096,1093,1090,1087,
	1084,1081,1078,1075,1072,1069,1066,1062,1059,1056,1053,1050,1047,1044,1041,1038,1035,1031,1028,1025,
	1022,1019,1016,1013,1010,1007,1003,1000, 997, 994, 991, 988, 985, 982, 978, 975, 972, 969, 966, 963,
	 960, 957, 953, 950, 947, 944, 941, 938, 935, 931, 928, 925, 922, 919, 916, 913, 909, 906, 903, 900,
	 897, 894, 891, 887, 884, 881, 878, 875, 872, 869, 865, 862, 859, 856, 853, 850, 847, 843, 840, 837,
	 834, 831, 828, 825, 822, 818, 815, 812, 809, 806, 803, 800, 797, 793, 790, 787, 784, 781, 778, 775,
	 772, 769, 765, 762, 759, 756, 753, 750, 747, 744, 741, 738, 734, 731, 728, 725, 722, 719, 716, 713,
	 710, 707, 704, 701, 698, 694, 691, 688, 685, 682, 679, 676, 673, 670, 667, 664, 661, 658, 655, 652,
	 649, 646, 643, 640, 637, 634, 631, 628, 625, 622, 619, 616, 613, 610, 607, 604, 601, 598, 595, 592,
	 589, 586, 583, 580, 577, 575, 572, 569, 566, 563, 560, 557, 554, 551, 548, 545, 543, 540, 537, 534,
	 531, 528, 525, 522, 520, 517, 514, 511, 508, 505, 503, 500, 497, 494, 491, 489, 486, 483, 480, 477,
	 475, 472, 469, 466, 464, 461, 458, 455, 453, 450, 447, 445, 442, 439, 436, 434, 431, 428, 426, 423,
	 420, 418, 415, 412, 410, 407, 405, 402, 399, 397, 394, 392, 389, 386, 384, 381, 379, 376, 374, 371,
	 368, 366, 363, 361, 358, 356, 353, 351, 348, 346, 343, 341, 339, 336, 334, 331, 329, 326, 324, 321,
	 319, 317, 314, 312, 310, 307, 305, 302, 300, 298, 295, 293, 291, 289, 286, 284, 282, 279, 277, 275,
	 273, 270, 268, 266, 264, 261, 259, 257, 255, 253, 250, 248, 246, 244, 242, 240, 238, 235, 233, 231,
	 229, 227, 225, 223, 221, 219, 217, 215, 213, 211, 209, 207, 205, 203, 201, 199, 197, 195, 193, 191,
	 189, 187, 185, 183, 181, 179, 177, 176, 174, 172, 170, 168, 166, 165, 163, 161, 159, 157, 156, 154,
	 152, 150, 149, 147, 145, 143, 142, 140, 138, 137, 135, 133, 132, 130, 129, 127, 125, 124, 122, 121,
	 119, 117, 116, 114, 113, 111, 110, 108, 107, 105, 104, 102, 101, 100,  98,  97,  95,  94,  92,  91,
	  90,  88,  87,  86,  84,  83,  82,  80,  79,  78,  77,  75,  74,  73,  72,  70,  69,  68,  67,  66,
	  64,  63,  62,  61,  60,  59,  58,  56,  55,  54,  53,  52,  51,  50,  49,  48,  47,  46,  45,  44,
	  43,  42,  41,  40,  39,  38,  38,  37,  36,  35,  34,  33,  32,  31,  31,  30,  29,  28,  27,  27,
	  26,  25,  25,  24,  23,  22,  22,  21,  20,  20,  19,  18,  18,  17,  17,  16,  15,  15,  14,  14,
	  13,  13,  12,  12,  11,  11,  10,  10,   9,   9,   8,   8,   7,   7,   7,   6,   6,   6,   5,   5,
	   5,   4,   4,   4,   3,   3,   3,   3,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,
	   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
	   1,	1,	 1,	  1,   1,   1,   2,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   5,   5,
	   5,   6,   6,   6,   7,   7,   7,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,
	  14,  15,  15,  16,  17,  17,  18,  18,  19,  20,  20,  21,  22,  22,  23,  24,  25,  25,  26,  27,
	  27,  28,  29,  30,  31,  31,  32,  33,  34,  35,  36,  37,  38,  38,  39,  40,  41,  42,  43,  44,
	  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  58,  59,  60,  61,  62,  63,  64,  66,
	  67,  68,  69,  70,  72,  73,  74,  75,  77,  78,  79,  80,  82,  83,  84,  86,  87,  88,  90,  91,
	  92,  94,  95,  97,  98, 100, 101, 102, 104, 105, 107, 108, 110, 111, 113, 114, 116, 117, 119, 121,
	 122, 124, 125, 127, 129, 130, 132, 133, 135, 137, 138, 140, 142, 143, 145, 147, 149, 150, 152, 154,
	 156, 157, 159, 161, 163, 165, 166, 168, 170, 172, 174, 176, 177, 179, 181, 183, 185, 187, 189, 191,
	 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 223, 225, 227, 229, 231,
	 233, 235, 238, 240, 242, 244, 246, 248, 250, 253, 255, 257, 259, 261, 264, 266, 268, 270, 273, 275,
	 277, 279, 282, 284, 286, 289, 291, 293, 295, 298, 300, 302, 305, 307, 310, 312, 314, 317, 319, 321,
	 324, 326, 329, 331, 334, 336, 339, 341, 343, 346, 348, 351, 353, 356, 358, 361, 363, 366, 368, 371,
	 374, 376, 379, 381, 384, 386, 389, 392, 394, 397, 399, 402, 405, 407, 410, 412, 415, 418, 420, 423,
	 426, 428, 431, 434, 436, 439, 442, 445, 447, 450, 453, 455, 458, 461, 464, 466, 469, 472, 475, 477,
	 480, 483, 486, 489, 491, 494, 497, 500, 503, 505, 508, 511, 514, 517, 520, 522, 525, 528, 531, 534,
	 537, 540, 543, 545, 548, 551, 554, 557, 560, 563, 566, 569, 572, 575, 577, 580, 583, 586, 589, 592,
	 595, 598, 601, 604, 607, 610, 613, 616, 619, 622, 625, 628, 631, 634, 637, 640, 643, 646, 649, 652,
	 655, 658, 661, 664, 667, 670, 673, 676, 679, 682, 685, 688, 691, 694, 698, 701, 704, 707, 710, 713,
	 716, 719, 722, 725, 728, 731, 734, 738, 741, 744, 747, 750, 753, 756, 759, 762, 765, 769, 772, 775,
	 778, 781, 784, 787, 790, 793, 797, 800, 803, 806, 809, 812, 815, 818, 822, 825, 828, 831, 834, 837,
	 840, 843, 847, 850, 853, 856, 859, 862, 865, 869, 872, 875, 878, 881, 884, 887, 891, 894, 897, 900,
};

extern float 				frequency;//frequency of sine (adjustable in main)
extern float 				pwmFrequency; //frequency of the pwm signal (20kHz)
extern float 				pwmPeriodConversion;
extern uint32_t 			numberOfSineValues;
extern uint32_t 			rangeOfSineValues;

extern uint32_t				counter_on_channel_1;
extern uint32_t				counter_on_channel_2;
extern uint32_t				counter_on_channel_3;

const float 				pi = 3.141592653;
float 						omegaT = 0;
float 						angle1 = 0;
float 						angle2 = 2*pi/3;
float 						angle3 = 4*pi/3;
float 						phaseShift = 2*pi/3;

//variables for ADC (measuring current)
extern unsigned 			bufferFlag;
extern unsigned char		seqFlag;
extern uint32_t				convres;
extern uint32_t     		buffer[7];
extern uint32_t				numberOfAveragedValues;
extern uint32_t				bufferHistory[7][1000];
int 						actualBufferHistoryPosition = 0;
extern float 				overCurrentThreshold;
extern float 				amperePerDigits;
extern float				bufferAverage[7];
extern float				bufferCalibrated[7];
extern float 				bufferCalibrated6;
extern float 				current_IHB1;
extern float 				current_IHB2;
extern float 				current_IHB3;
extern float 				current_HB1H;
extern float 				current_HB2H;
extern float 				current_V_BRUECKE;
extern float 				current_I_BRUECKE;
extern uint32_t 			WDHTR;
extern bool					current_measured_inPeriod_1;
extern bool					current_measured_inPeriod_2;
extern bool					current_measured_inPeriod_3;

//variables for third harmonic
extern 						bool enableThirdHarmonic;
float 						SG_max = 0.0;
float 						SG_min = 0.0;
float 						U_harm = 0.0;
float 						pulseWidth1 = 0.0;
float 						pulseWidth2 = 0.0;
float 						pulseWidth3 = 0.0;


//variables for the incremental encoder
extern int					rotVelo;	//calculated rotation velocity
float						period = 0.0005;	//time of the interupt for the incremental encoder
int							startInc = 0;	//last value of the encoder
int							actInc = 0;	//actual value of the encoder
float						totalIncrement = 4096.0;	//total number of increments per rotation (1024 per rotation * 4 positive and negative signals of two flanks)
float						secToMin = 60.0;	//factor for calculating the velocity in rotations per minute


//variables for the controller
float						inductivity = 0.0022;	//inductivity of the dc motor (2.2 mH for Engel GNM 5440)
float						resistance = 0.7;	//resistance of the dc motor (0.7 Ohm for Engel GNM 5440)
float						timeConstant = 0.0;	//time constant of the PI controller
float						damping = 1.0;		//damping of our PI controller
float						samplingTime = 0.00005;	//sampling time of the PI controller, 50us, equal to period of tim2
float						amplification = 0.0;	//amplification of the PI controller
int							countTimeDelay = 0;		//gives a little delay after switch on the controller
int 						countJump = 0;			//count variable for simulating a jump
float						referenceValue = 0.4;	//reference value w
float						controlDeviation = 0.0;	//deviation between reference value and controlled value e=w-x
float						controlDeviationOld = 0.0;
float						controlValue = 0.0;		//output value given by the controller u
float						controlValueOld = 0.0;
float						controlValuePWM = 0.0;	//normalized output value to control the duty cycle of the PWM signal y
float						maxDutyCycle = 0.9;		//maximum of the possible PWM duty cycle


//variables for the watchdog
extern IWDG_HandleTypeDef 	hiwdg1;

//the following four functions are needed for calculating the third harmonic
float max(float a, float b)//returns the higher value
{
    if (a>b)
        return a;
    else
        return b;
}
float min(float a, float b)//returns the lower value
{
    if (a<b)
        return a;
    else
        return b;
}
float max3(float a, float b, float c)//returns the highest value
{
	return max(max(a, b),c);
}
float min3(float a, float b, float c)//returns the lowest value
{
	return min(min(a, b),c);
}

//the following two functions are for calculating our DC PI-Regulator values

/*float TA(float la, float ra)	//function for the time constant of the dc motor
{
	float ta;
	ta = la / ra;
	return ta;
}

float VR(float la, float d, float tSamp)	//function for the amplification of the PI Regulator
{
	float vr;
	vr = la / (6 * d * d * tSamp );
	return vr;
}*/

/**
  * @brief Take measure from adc by reading from buffer
  * @input channel to be red: 1 = IHB1, 2 = IHB_2, 3 = IHB_3, 4 = HB1H, 5 = HB2H, 6 = V_BRUECKE, 7 = I_BRUECKE
  */
void measure(int channel)
{
	  //With turning on the pin PE4 at this point, you can check with the oscilloscope at which time TIM2 starts.
	  //At the end of TIM2 the Pin gets turned off, so you can see how much time the calculation of TIM2 took.
	//HAL_GPIO_WritePin(Test_pulse_GPIO_Port, Test_pulse_Pin, 1);
	float conversionFactor = 0;
	switch (channel) {
	case 1:
		  //calculating the average of the current measured in the three phases
		  current_IHB1 = (bufferCalibrated[0]+bufferAverage[0])*amperePerDigits;
		  break;
	case 2:
		  //calculating the average of the current measured in the three phases
		  current_IHB2 = (bufferCalibrated[1]+bufferAverage[1])*amperePerDigits;
		  break;
	case 3:
		  //calculating the average of the current measured in the three phases
		  current_IHB3 = (bufferCalibrated[2]+bufferAverage[2])*amperePerDigits;
		  break;
	case 4:
		  //calculating the average of the measured mosfet signal
		current_HB1H = ((bufferAverage[3]+bufferCalibrated[3])/4096)*current_V_BRUECKE;
		  break;
	case 5:
		conversionFactor = 59.4/4096/1000;
		  //calculating the average of the measured mosfet signal
		current_HB2H = ((bufferAverage[4]+bufferCalibrated[4])/4096)*current_V_BRUECKE;
		  break;
	case 6:
		conversionFactor = 59.4/4096/1000;
		  //calculating the average of the source voltage
		current_V_BRUECKE = (bufferAverage[5]+bufferCalibrated[5])*conversionFactor;
		  break;
	case 7:
		 conversionFactor = 66/4096/1.65;
		  //calculating the average of the current taken from source
		current_I_BRUECKE = (bufferAverage[6]+bufferCalibrated[6])*conversionFactor - 20;
		  break;
	default:
		break;
	}
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
/* USER CODE BEGIN EV */
extern TIM_HandleTypeDef htim1;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi2_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

	current_measured_inPeriod_1 = false;
	current_measured_inPeriod_2 = false;
	current_measured_inPeriod_3 = false;
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  //With turning on the pin PE4 at this point, you can check with the oscilloscope at which time TIM2 starts.
  //At the end of TIM2 the Pin gets turned off, so you can see how much time the calculation of TIM2 took.
  //HAL_GPIO_WritePin(Test_pulse_GPIO_Port, Test_pulse_Pin, 1);



  switch (operationMode)
      {
  	  	  case turnOff://Dip 0000 -> turn off the motor
		  {
			  HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 0); //disable bridge
			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
			  operationMode = do_nothing;
		  }
  	  	  case do_nothing://after turning the motor off -> do nothing
		  {
			  break;
		  }

  		  case start_threePhaseMode://Dip 1000 -> initialisation of three phase PWM mode
		  {
			  //if the softstarter is enabled, the value of the factor "softstarter" starts at 0. It slowly rises to 1
			  //if the soststarter is disabled, the value of the factor "softstarter" automatically is at 1. It won't rise.
			  softstarter = !enableSoftstarter;
			  HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 1);//enable Bridge
			  operationMode = do_threePhaseMode;
		  }
  	  	  case do_threePhaseMode://after initialisation of three phase PWM mode is finished -> working in three phase PWM mode
		  {
			  if(softstarter < 1)//the factor "softstarter" slowly rises from 0 to 1
			  {
				  softstarter = softstarter + 1/(pwmFrequency*softstarterDuration);
			  }
			  else
				  softstarter = 1;
			  /*calculating phase1
			  The phase1 starts with the angle 0, with each cycle the alternation of the angle is added to the previous angle.
			  The alternation of the angle is dependent on the adjusted frequency and the elapsed time.
			  The elapsed time is calculated using the reciprocal of the pwmFrequency. (actual: 1/20kHz = 50u seconds)
			  The softstarter is just a factor between 0 and 1 to allow a lower frequency at the beginning
			  If you want to change the rotation direction, you can do it in two ways:
			  1. Give "frequency" a negative sign
			  2. Enable or disable the variable "roationDirectionCW"
			  if "rotationDirectionCW" == true , the value in this bracket is +1
			  if "rotationDirectionCW" == false, the value in this bracket is -1
			  so its just another factor multiplied with the frequency.*/
			  angle1 = angle1
					  + (2*pi*frequency*(1/pwmFrequency)
					  * (-1+2*rotationDirectionCW) //if rotationDirectionCW==true  ->  1
					  	  	  	  	  	  	  	   //if rotationDirectionCW==false -> -1
					  * softstarter);			   //value between 0 and 1

			  if(angle1 > 2*pi) //This ensures, that the angle stays in the range of values from 0 to 2*pi while the angle increases
			  {
				  angle1 = angle1 - 2*pi;
			  }
			  if(angle1 < 0) //This ensures, that the angle stays in the range of values from 0 to 2*pi while the angle decreases
			  {
				  angle1 = angle1 + 2*pi;
			  }
			  //calculating angle2
			  //phasShift in three phase mode is 120° = 2*pi/3
			  //Therefore in the range from 240° to <360° = 4*pi/3 to <2*pi
			  //The angle2 is always 120° = 2*pi/3 out of phase with angle1
			  //to ensure that angle2 never leaves the range from 0 to 2*pi
			  //if angle1 is between 120 and <360° = 2*pi/3 to <2*pi   the angle2 is 120° = 2*pi/3 lower  than angle1
			  //if angle1 is between   0 and <120° =      0 to <2*pi   the angle2 is 240° = 4*pi/3 higher than angle1
			  if(angle1 > phaseShift)//
			  {
				  angle2 = angle1-phaseShift;
			  }
			  else
			  {
				  angle2 = angle1+2*phaseShift;
			  }
			  //calculating angle3
			  //same ratio as angle1 to angle2
			  if(angle2 > phaseShift)
			  {
				  angle3 = angle2-phaseShift;
			  }
			  else
			  {
				  angle3 = angle2+2*phaseShift;
			  }

//		older Version of calculating angle2 and 3. Works but takes a little more time than the actual solution
//				  //calculating angle2
//				  //angle2 has the start value 2/3*pi
//				  angle2 = angle2 + (2*pi*frequency*softstarter*(1/pwmFrequency));
//				  if (angle2 > 2*pi)
//				  {
//					  angle2 = angle2 - 2*pi;
//				  }
//
//				  //calculating angle3
//				  //angle2 has the start value 4/3*pi
//				  angle3 = angle3 + (2*pi*frequency*softstarter*(1/pwmFrequency));
//				  if (angle3 > 2*pi)
//				  {
//					  angle3 = angle3 - 2*pi;
//				  }

			  //The current angles get transformed into the according place in the table of sine values
			  uint32_t tablePosition1 = angle1*(numberOfSineValues/(2*pi));//the sine table has 1800 values
			  uint32_t tablePosition2 = angle2*(numberOfSineValues/(2*pi));
			  uint32_t tablePosition3 = angle3*(numberOfSineValues/(2*pi));

			  pulseWidth1 = sineTable[tablePosition1];
			  pulseWidth2 = sineTable[tablePosition2];
			  pulseWidth3 = sineTable[tablePosition3];

			  //adding the third harmonic
			  //setting the range of table values from (0)-(+1800) to (-900)-(+900)

			  if(true == enableThirdHarmonic)
			  {
				  //setting the range of table values from (0)-(+1800) to (-900)-(+900)
				  //this is needed in the further functions
				  pulseWidth1 = pulseWidth1-(rangeOfSineValues/2);
				  pulseWidth2 = pulseWidth2-(rangeOfSineValues/2);
				  pulseWidth3 = pulseWidth3-(rangeOfSineValues/2);

				  //calculating the highest pulse with
				  SG_max = max3(pulseWidth1,pulseWidth2,pulseWidth3);
				  //calculating the lowest pulse with
				  SG_min = min3(pulseWidth1,pulseWidth2,pulseWidth3);
				  //calculating the harmonic part
				  U_harm = -(SG_min + SG_max)*0.5;

				  //setting the range of table values back to (0)-(+1800)
				  //and adding the harmonic proportion
				  pulseWidth1 = pulseWidth1+(rangeOfSineValues/2)+U_harm;
				  pulseWidth2 = pulseWidth2+(rangeOfSineValues/2)+U_harm;
				  pulseWidth3 = pulseWidth3+(rangeOfSineValues/2)+U_harm;
			  }
			  //adjusting the current duty cycles of the PWM signals

			  //adjusting angle1
			  counter_on_channel_1 = pulseWidth1*pwmPeriodConversion
					  *(voltage_ref/(v_bridge_uf*maxTensionRelationship))
					  +0.05*rangeOfSineValues*pwmPeriodConversion;

			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,counter_on_channel_1);

			  //adjusting angle2
			  counter_on_channel_2 = pulseWidth2*pwmPeriodConversion
						 *(voltage_ref/(v_bridge_uf*maxTensionRelationship))
						 +0.05*rangeOfSineValues*pwmPeriodConversion;

			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,counter_on_channel_2);

			  //adjusting angle3
			  counter_on_channel_3 = pulseWidth3*pwmPeriodConversion
						 *(voltage_ref/(v_bridge_uf*maxTensionRelationship))
						 +0.05*rangeOfSineValues*pwmPeriodConversion;

			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,counter_on_channel_3);
			  break;
		  }

  	  	  case start_DCMode://Dip 0100 -> initialisation of DC Mode
		  {
			  //if the softstarter is enabled, the value of the factor "softstarter" starts at 0. It slowly rises to 1
			  //if the softstarter is disabled, the value of the factor "softstarter" automtically is at 1. It will not rise.
			  softstarter = !enableSoftstarter;
			  HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 1);
			  operationMode = do_DCMode;
			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
		  }
  	  	  case do_DCMode://after Initialisation of DC mode is finished -> working DC mode
		  {
			  if(softstarter < 1)//the factor "softstarter" slowly rises from 0 to 1
			  {
				  softstarter = softstarter + 1/(pwmFrequency*softstarterDuration);
			  }
			  else
			  {
				  softstarter = 1;
			  }
			  if(false == rotationDirectionCW) 								//motor rotates counterclockwise
			  {
				  counter_on_channel_1 = rangeOfSineValues*pwmPeriodConversion*0.05;
				  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,counter_on_channel_1);	//Adjusting x7_U as 0V

				  counter_on_channel_2 =  rangeOfSineValues*pwmPeriodConversion*(voltage_ref*softstarter/(v_bridge_uf))
								+(rangeOfSineValues*pwmPeriodConversion*0.05);
				  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,counter_on_channel_2);	//Adjusting x7_V as the voltage_ref of DC Motor

			  }
			  else															//motor rotates clockwise
			  {
				  counter_on_channel_1 =  rangeOfSineValues*pwmPeriodConversion*(voltage_ref*softstarter/(v_bridge_uf))
								+(rangeOfSineValues*pwmPeriodConversion*0.05);
				  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,counter_on_channel_1);	//Adjusting x7_U as the voltage_ref of DC Motor

				  counter_on_channel_2 = rangeOfSineValues*pwmPeriodConversion*0.05;
				  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,counter_on_channel_2);	//Adjusting x7_V as 0V
			  }

			  break;
		  }
  	  	  case start_calibrateADC:
		  {
			  HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 0);
			  operationMode = do_calibrateADC;
		  }
  	  	  case do_calibrateADC:
  	  	  {
  	  		  for(int i = 0;i<=6;i++) {
  	  			bufferCalibrated[i] = 0;
  	  			measure(i+1);
  	  			bufferCalibrated[i] = -bufferAverage[i];
  	  		  }

			  WDHTR = overCurrentThreshold/amperePerDigits-bufferCalibrated[0];
			  ADC1->HTR1 = WDHTR;
			  break;
  	  	  }
  	  	  /*case start_control: //Dip 0001: initialization of the controller
  	  	  {
  	  		  HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 1);
  	  		  operationMode = do_control;
  	  		  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
  	  	  }*/

  	  	case start_control: //Dip 0001: initialization of the controller, enables the bridge delayed to prevent effects from the dip switches
  	  	  	  	  {
  	  	  	  		  if(countTimeDelay < 4000)
  	  	  	  		  {
  	  	  	  			  countTimeDelay++;
  	  	  	  		  }
  	  	  	  		  else
  	  	  	  		  {
  	  	  	  			  HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 1);
  	  	  	  		  	  operationMode = do_control;
  	  	  	  		  	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
  	  	  	  		  	  countTimeDelay = 0;
  	  	  	  		  	  referenceValue = 0.4;
  	  	  	  		  }
  	  	  	  	  }
  	  	  case do_control: //routine of the controller
  	  	  {
  	  		  if(countJump < 100000) //simulating a current jump after a certain time
  	  		  {
  	  		  	  countJump++;
  	  		  }
  	  		  	  else
  	  		  {
  	  		  	  referenceValue = 0.6;
  	  		  }
  	  		  timeConstant = 0.03; 		//TA(inductivity, resistance); //(equation to calculate or value for the time constant);
  	  		  amplification = 0.730;		//VR(inductivity, damping, samplingTime); //(equation to calculate or value for the amplification);
  	  		  controlDeviationOld = controlDeviation;		//saving the last measured value
  	  		  if(false == rotationDirectionCW)  //take the current depending on the rotation direction
  	  		  {
  	  			controlDeviation = referenceValue - current_IHB2;
  	  		  }
  	  		  else
  	  		  {
  	  			controlDeviation = referenceValue - current_IHB1;
  	  		  }
  	  		  controlValueOld = controlValue;	//save last output value

  	  		  //equation of our controller to calculate the output value
  	  		  controlValue = controlValueOld + amplification * (controlDeviation - controlDeviationOld) + (amplification / timeConstant) * samplingTime * (controlDeviation + controlDeviationOld) / 2;
  	  		  //boundaries for the output value, limited by the voltage of the power supply
  	  		  if (controlValue > v_bridge_uf)
  	  		  {
  	  			  controlValue = v_bridge_uf;
  	  		  }
  	  		  else if (controlValue < -v_bridge_uf)
  	  		  {
  	  			  controlValue = -v_bridge_uf;
  	  		  }
  	  		  //normalization of the output value to get a new duty cycle for the PWM signal
  	  		  controlValuePWM = maxDutyCycle *(controlValue +v_bridge_uf) / (2 * v_bridge_uf);


  	  		  if(false == rotationDirectionCW) 								//motor rotates counterclockwise
  	  		  {
  	  			  	  	  	  counter_on_channel_1 = rangeOfSineValues*pwmPeriodConversion*0.05;
  	  						  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,counter_on_channel_1);	//Adjusting x7_U as 0V

  	  						counter_on_channel_2 = rangeOfSineValues*pwmPeriodConversion*controlValuePWM
									  +(rangeOfSineValues*pwmPeriodConversion*0.05);
  	  						  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,counter_on_channel_2);	//Adjusting x7_V as the voltage_ref of DC Motor

  	  		  }
  	  		  else															//motor rotates clockwise
  	  		  {
  	  			  	  	  counter_on_channel_1 = rangeOfSineValues*pwmPeriodConversion*controlValuePWM
	  								+(rangeOfSineValues*pwmPeriodConversion*0.05);
  	  						  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,counter_on_channel_1);	//Adjusting x7_U as the voltage_ref of DC Motor

  	  					counter_on_channel_2 = rangeOfSineValues*pwmPeriodConversion*0.05;
  	  						  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,counter_on_channel_2);	//Adjusting x7_V as 0V
  	  		  }
  	  		  break;
  	  	  }

      }

  	  HAL_GPIO_WritePin(Test_pulse_GPIO_Port, Test_pulse_Pin, 0);//turn off PE4
  	  HAL_IWDG_Refresh(&hiwdg1); //refresh the watchdog
  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  if(startInc == 0)	//first value of the incremental encoder
  {
	  startInc = TIM5 -> CNT;
  }
  else
  {
	  actInc = TIM5 -> CNT;	//actual value of the encoder
	  rotVelo = (actInc - startInc) / totalIncrement * secToMin / period; //calculation of the rotation velocity in rotations per minute
	  startInc = actInc;	//setting the actual number of increments as the last number of increments
  }
  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  dipPositionPrevious = dipPositionCurrent;
  dipPositionCurrent = 		HAL_GPIO_ReadPin(DIPSW1_GPIO_Port, DIPSW1_Pin)   +
		  	  	  	  	    HAL_GPIO_ReadPin(DIPSW2_GPIO_Port, DIPSW2_Pin)*2 +
						    HAL_GPIO_ReadPin(DIPSW3_GPIO_Port, DIPSW3_Pin)*4 +
							HAL_GPIO_ReadPin(DIPSW4_GPIO_Port, DIPSW4_Pin)*8;
  if(no_Error == error && dipPositionCurrent == 1) //Erster Dip switch fuer Fernsteuerungsfunktion
  {
	  if(motor_status == 1) {
		  operationMode = start_threePhaseMode;
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
	  } else if(motor_status == 2){
		  operationMode = start_DCMode;
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
	  } else if(motor_status == 3){
		  operationMode = start_calibrateADC;
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
	  } else if(motor_status == 4){
		  operationMode = start_control;
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
	  } else {
		  operationMode = turnOff;
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
	  }
//	if(dipPositionCurrent != dipPositionPrevious)
//	  {
//		  switch (dipPositionCurrent)
//		  {
//			  case 0:
//			  {
//				  operationMode = turnOff;
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
//				  break;
//			  }
//			  case 1:
//			  {
//				  operationMode = start_threePhaseMode;
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_SET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
//				  break;
//			  }
//			  case 2:
//			  {
//				  operationMode = start_DCMode;
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_SET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
//				  break;
//			  }
//			  case 3:
//			  {
//				  operationMode = start_calibrateADC;
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
//				  break;
//			  }
//			  case 8:
//			  {
//				  operationMode = start_control;
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_SET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_SET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
//				  break;
//			  }
//			  default:
//			  {
//				  operationMode = turnOff;
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
//				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
//				  break;
//			  }
//		  }
//	  }
  }
  else
  {
	  HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 0);
	  switch(error)
	  {
		  case invalidValues_Error://one of the entered variables lies outside the permitted range of values
		  {
			  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
			  break;
		  }
		  case overcurrent_Error://too high current
		  {
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
			  break;
		  }
		  case watchdog_Error://watchdog of TIM2
		  {
			  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1);
			  break;
		  }
		  case opticalFiber_Error://The optical fibers aren't correctly connected
		  {
			  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1);
			  break;
		  }
	  }
	  if(0 == dipPositionCurrent)//acknowledge the error message
	  {
		  error = no_Error;
		  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
		  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0);
		  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0);
		  operationMode = turnOff;
	  }
  }
//  checking if all user values are in permitted range
  if((voltage_ref > maxTensionRelationship* v_bridge_uf)
   ||(v_bridge_uf <  10)
   ||(v_bridge_uf > 60)
   ||(frequency <-200  )
   ||(frequency > 200  )
   ||(overCurrentThreshold > 15)//square(2)*10+noise
   ||(numberOfAveragedValues > 100))
  {
	  HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 0); //disable bridge
	  error = invalidValues_Error;
  }

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles SPI2 global interrupt.
  */
void SPI2_IRQHandler(void)
{
  /* USER CODE BEGIN SPI2_IRQn 0 */

  /* USER CODE END SPI2_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi2);
  /* USER CODE BEGIN SPI2_IRQn 1 */

  /* USER CODE END SPI2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */

  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */

  /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void DMA1_Stream0_IRQHandler (void)									// DMA RX
 {
	// Store buffer values in history
	for(int i = 0;i<=6;i++){
		bufferHistory[i][actualBufferHistoryPosition] = buffer[i];
	}
	actualBufferHistoryPosition++;
	if(	actualBufferHistoryPosition > (numberOfAveragedValues - 1)) {
		actualBufferHistoryPosition = 0;
	}

	// Calculate average
	for(int i = 0;i<=6;i++){
		uint32_t sum = 0;
		for(int j = 0; j<numberOfAveragedValues;j++) {
			sum = sum +	bufferHistory[i][j];
		}
		bufferAverage[i] = sum/numberOfAveragedValues;
	}

 DMA1->LIFCR = DMA_LIFCR_CTCIF0;									// clear DMA stream complete interrupt
 DMA1_Stream0->M0AR = (uint32_t)(&(buffer[0]));					// this when NDTR = 300 (buffer 100 * sequences 3)
 DMA1_Stream0->CR |= DMA_SxCR_EN;
 }

void ADC_IRQHandler (void)

//Old declaration of ADC, not used anymore, because the DMA works better
//Interrupt handler for the IDC current measurement:
//the modules U16, U17 and U18 (MCR1101-50-3) measure the three currents that flow into the motor (range of possible measured values: -20V to 20V).
//Proportional to the flowing current, each module generates a tension in the range from 0.33V to 2.97V.
//The ADC_IRQHandler is reading out this tension from the pins: PA4(IHB1_UC), PA6(IHB2_UC), PC4(IHB3_UC) and makes an AD conversion
//Than the digits get converted to the actual current values.
//If the current exceeds the "overCurrentThreshold" (max. 10A) the bridge gets disabled.
{
	//HAL_GPIO_WritePin(Test_pulse_GPIO_Port, Test_pulse_Pin, 1);

//	if (ADC1->ISR & ADC_ISR_EOC)					//conversion ready
//	{	//bufferFlag: Values from 1 to "numberOfAveragedValues" -> store the values for calculating an average
//		//seqFlag:	   Values from 0 to 2 -> store each input [PA4(IHB1_UC), PA6(IHB2_UC), PC4(IHB3_UC)]
//		buffer[bufferFlag][seqFlag] = ADC1->DR;	//reading out current value, writing it in buffer. EOC cleared by reading DR
//		++seqFlag;
//	}
//	if (ADC1->ISR & ADC_ISR_EOS) 										// conversion sequence ready
//	{
//		if(((bufferCalibrated1+buffer[bufferFlag][0])*amperePerDigits>overCurrentThreshold)//checks whether the one of the currents exceed the "overCurrentThreshold"
//	     ||((bufferCalibrated1+buffer[bufferFlag][1])*amperePerDigits>overCurrentThreshold)
//	     ||((bufferCalibrated1+buffer[bufferFlag][2])*amperePerDigits>overCurrentThreshold))
//		{
//			HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 0); //the bridge gets disabled
//			error = 2;														//-> LED 2 gets turned on (TIM4)
//	    }
//		ADC1->ISR	|= ADC_ISR_EOS; 										// EOS cleared by writing 1 to it
//		seqFlag = 0;
//
//		bufferSum_IHB1 = bufferSum_IHB1 + buffer[bufferFlag][2];//for later calculated average, all values are added
//		bufferSum_IHB2 = bufferSum_IHB2 + buffer[bufferFlag][1];
//		bufferSum_IHB3 = bufferSum_IHB3 + buffer[bufferFlag][0];
//
//		++bufferFlag;
//
//		if (bufferFlag == numberOfAveragedValues)
//		{
//
//			bufferAverage_IHB1 = bufferSum_IHB1/numberOfAveragedValues;//averages are calculated
//			bufferAverage_IHB2 = bufferSum_IHB2/numberOfAveragedValues;
//			bufferAverage_IHB3 = bufferSum_IHB3/numberOfAveragedValues;
//		    bufferSum_IHB1 = 0;
//		    bufferSum_IHB2 = 0;
//		    bufferSum_IHB3 = 0;
//
//		    if(99 == operationMode)//when all dip switches turned off, therefore bridge is thisabled, therefore motor is stopped, therefore no current flows, the buffers get calibrated
//		    {
//		    	bufferCalibrated1 = - bufferAverage_IHB1;
//		    	bufferCalibrated2 = - bufferAverage_IHB2;
//		    	bufferCalibrated3 = - bufferAverage_IHB3;
//		    }
//		    current_IHB1 = (bufferCalibrated1+bufferAverage_IHB1)*amperePerDigits;
//		    current_IHB2 = (bufferCalibrated2+bufferAverage_IHB2)*amperePerDigits;
//		    current_IHB3 = (bufferCalibrated3+bufferAverage_IHB3)*amperePerDigits;
//		    bufferFlag = 0;
//		}
//	}
	//HAL_GPIO_WritePin(Test_pulse_GPIO_Port, Test_pulse_Pin, 0);

	if (ADC1->ISR & ADC_ISR_AWD1) //Analog Watchdog, if overCurrentThreshold is reached
	  {
		ADC1->ISR |= ADC_ISR_AWD1;										// clear by writing 1
		HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 0); //the bridge gets disabled
		error = overcurrent_Error;
	  }
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
