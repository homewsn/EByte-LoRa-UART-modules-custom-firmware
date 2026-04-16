/*
* Copyright (c) 2022, 2026 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under 
* the terms of GNU General Public License version 2 
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

//--------------------------------------------
#if defined E32_433T20D_32100_V3_0
static const char module_name[] = "E32-433T20D-32100-V3.0";
#elif defined E32_433T20D_10012_V6_0
static const char module_name[] = "E32-433T20D-10012-V6.0";
#elif defined E32_433T30D_10010_V6_2
static const char module_name[] = "E32-433T30D-10010-V6.2";
#elif defined E45_TTL_1W_10024_V2_0
static const char module_name[] = "E45-TTL-1W-10024-V2.0";
#elif defined E220_400T22D_10411_V1_2
static const char module_name[] = "E220-400T22D-10411-V1.2";
#elif defined E220_900T22D_10411_V1_2
static const char module_name[] = "E220-900T22D-10411-V1.2";
#elif defined E32_900T20D_10411_V1_2
static const char module_name[] = "E32-900T20D-10411-V1.2";
#elif defined E32_433T30D_10498_V1_1
static const char module_name[] = "E32-433T30D-10498-V1.1";
#elif defined E22_400T30D_10498_V1_1
static const char module_name[] = "E22-400T30D-10498-V1.1";
#elif defined E220_400T30D_10408_V1_0
static const char module_name[] = "E220-400T30D-10408-V1.0";
#elif defined E220_400T30D_10408_V1_1
static const char module_name[] = "E220-400T30D-10408-V1.1";
#elif defined E78_470LN22S_10447_V1_0
static const char module_name[] = "E78-470LN22S-10447-V1.0";
#elif defined DX_LR03_433T30D_V2_1
static const char module_name[] = "DX-LR03-433T30D-V2.1";
#elif defined E77_400M22S_10526_V1_1
static const char module_name[] = "E77-400M22S-10526-V1.1";
#else
#error "Unknown module"
#endif

//--------------------------------------------
static const char *get_module_name(void)
{
	return module_name;
}
