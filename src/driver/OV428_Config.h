/**************************************************************************//**
 * @file     OV426_Config.h
 * @version  V2.01
 * $Revision: 2 $
 * $Date: 22/10/22 12:00a $
 * @brief    OV426 command script file.
 *
 * @note
 * SPDX-License-Identifier: Null
 * Copyright (C) 2022 Opcom Technology Corp. All rights reserved.
 * 
 * @history
 * 1. 20221021	Fisrt for OV426 script command.							Frank
 * 2. 20221022	Add for close, Mode, Setting TP/AEC/MWB scripts.		Frank
 ******************************************************************************/

#ifndef _OV426_CONFIG_H_
#define _OV426_CONFIG_H_

/*---------------------------------------------------------------------------------------------------------*/
/*  External file include                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "GxCommon.h"
#include "Sensor.h"

/** @addtogroup OPCOM_Function OPCOM Function
  @{
*/

/** @addtogroup OV426_Device_Driver OV426 Device Driver
  @{
*/

/** @addtogroup OV426_EXPORTED_CONSTANTS OV426 Exported Constants
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/* ID Constant Definitions                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define OV426_ID	(UINT32)0x6C /*!< Specify Device ID of call OV426 */

/*---------------------------------------------------------------------------------------------------------*/
/* CMD Constant Definitions                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
/*	Define close script for into standby mode. */
const SENSOR_CMD OV426_CLOSE_CMD[] =
{
	{ 0x0100, 1, {0x00, 0x00}}, // ; Stop Streaming (ISP)
	{ 0x0103, 1, {0x01, 0x00}}, // ; Software Reset (ISP)
	{ 0x0103, 1, {0x00, 0x00}}, // ; reset(ISP)
	{ 0x3205, 1, {0x01, 0x00}}, // ; Sensor Software Reset (Sensor)
	{ 0x3205, 1, {0x00, 0x00}}, // ; Sensor Software Reset (Sensor)
};
/*!< Close and stop the streaming, and standby mode. */

/* Define initialization mode1 script into streaming mode. */
const SENSOR_CMD OV426_OPEN_MODE1_CMD[] =
{
	{ 0x0103, 1, {0x01, 0x00} },  
	{ 0x0100, 1, {0x01, 0x00} },  
	
	{ 0x3025, 1, {0x00, 0x00} }, //Sampling Setting: 0x00 = 32MHz, 0x04 = 8MHz
	{ 0x3026, 1, {0x1C, 0x00} },
	{ 0x3A00, 1, {0x58, 0x00} }, //disable AEC/AGC freeze mode,enable EXP < 1 line (disable debug)

	
	{ 0x302a, 1, {0x01, 0x00}}, //; (00)1x @@@@@ 	//0x01 1.25

	//AE window
  	{ 0x5680, 1, {0x00, 0x00}}, // X START
	{ 0x5681, 1, {0x12, 0x00}}, // X START (150 px)
	{ 0x5682, 1, {0x00, 0x00}}, // Y START
	{ 0x5683, 1, {0x12, 0x00}}, // Y START (150 px)
	{ 0x5684, 1, {0x01, 0x00}}, // X Window (Manual)
	{ 0x5685, 1, {0x00, 0x00}}, // (100 px)
	{ 0x5686, 1, {0x01, 0x00}}, // Y Window (Manual)
	{ 0x5687, 1, {0x00, 0x00}}, //


	{ 0x5980, 1, {0x00, 0x00}},
	{ 0x5981, 1, {0x00, 0x00}},
	{ 0x5982, 1, {0x00, 0x00}},
	{ 0x5983, 1, {0x00, 0x00}},

	{ 0x5984, 1, {0x01, 0x00}}, // X Window (Manual) (400 px)
	{ 0x5985, 1, {0x90, 0x00}}, //400 px
	{ 0x5986, 1, {0x01, 0x00}}, // Y Window (Manual)
	{ 0x5987, 1, {0x90, 0x00}}, // 400 px
	{ 0x5988, 1, {0x00, 0x00}}, // window manual enable
	{ 0x5690, 1, {0x03, 0x00}}, // window average manual

/*
	{ 0x5688, 1, {0x11, 0x00}},		//Sub_window 1	0
	{ 0x5689, 1, {0x11, 0x00}},		//Sub_window 3	2
	{ 0x568A, 1, {0xFF, 0x00}},		//Sub_window 5	4
	{ 0x568B, 1, {0x1F, 0x00}},		//Sub_window 7	6
	{ 0x568C, 1, {0xFF, 0x00}},		//Sub_window 9	8
	{ 0x568D, 1, {0x1F, 0x00}},		//Sub_window 11	10
	{ 0x568E, 1, {0x11, 0x00}},		//Sub_window 13	12
	{ 0x568F, 1, {0x11, 0x00}},		//Sub_window 15	14
*/

	{ 0x5688, 1, {0xF1, 0x00}},		//Sub_window 1	0
	{ 0x5689, 1, {0xF1, 0x00}},		//Sub_window 3	2
	{ 0x568A, 1, {0x1F, 0x00}},		//Sub_window 5	4
	{ 0x568B, 1, {0x1F, 0x00}},		//Sub_window 7	6
	{ 0x568C, 1, {0xF1, 0x00}},		//Sub_window 9	8
	{ 0x568D, 1, {0xF1, 0x00}},		//Sub_window 11	10
	{ 0x568E, 1, {0x1F, 0x00}},		//Sub_window 13	12
	{ 0x568F, 1, {0x1F, 0x00}},		//Sub_window 15	14

	/* EWB control */
	{ 0x5186, 1, {0x04, 0x00}}, 	//WB Red Gain Msb
	{ 0x5187, 1, {0x00, 0x00}}, 	//WB Red Gain Lsb
	{ 0x5188, 1, {0x04, 0x00}}, 	//WB Green Gain Msb
	{ 0x5189, 1, {0x00, 0x00}}, 	//WB Green Gain Lsb
	{ 0x518A, 1, {0x04, 0x00}}, 	//WB Blue Gain Msb
	{ 0x518B, 1, {0x00, 0x00}}, 	//WB Blue Gain Lsb

	/* window control */

	{ 0x3024, 1, {0x04, 0x00} },	//adc_reg[7:0]
 	{ 0x3209, 1, {0x03, 0x00} },	//spi_reg : aec & agc auto update
 	{ 0x3701, 1, {0x01, 0x00} },	//async : Mirror , aec_auto_wr
	{ 0x3702, 1, {0x4B, 0x00} },  	//async : rd_data_dly
	{ 0x3003, 1, {0x32, 0x00} }, 	//0x22 0x21 Sampling Setting: 0x32 = 32MHz, 0x31 = 8MHz, sc_commn_reg : oversample = /2 , sclk2x_sel , adclk_inv
	{ 0x3004, 1, {0x00, 0x00} }, 	//Sampling Position for 32Mhz, try 0/1/2/3
	{ 0x3706, 1, {0x2E, 0x00} }, 	// sync width (6946)

	{ 0x3204, 1, {0x87, 0x00} },  	//spi_reg : ana[7:0]
	{ 0x3028, 1, {0xb0, 0x00} }, 	//sc_cmmn_pwc_reg : Vrbg = 0 , black_ref = 3 , Vcom = 2
	{ 0x3027, 1, {0x30, 0x00} }, 	//sc_cmmn_pwc_reg : sink 3.3mA
	{ 0x5a40, 1, {0x05, 0x00} },	//gainf_ana_num : gainf_ana_bit_num
	{ 0x3a19, 1, {0x0a, 0x00} },	//aec_gain_ceiling : gain_ceiling[7:0]
	{ 0x5a00, 1, {0x04, 0x00} },	//digc_ctrl0 : dig_comp_bypass
	{ 0x4009, 1, {0x18, 0x00} },	//black_level : [7:0]
	{ 0x4005, 1, {0x1A, 0x00} },	//blc_ctrl05 : remove_none_imagedata , blc en , always update
	{ 0x3020, 1, {0x09, 0x00} },	//adc_reg[39:32]
	{ 0x3024, 1, {0x00, 0x00} },	//adc_reg[7:0]
	{ 0x3203, 1, {0x23, 0x00} },	//adc_reg[15:8]
	{ 0x4052, 1, {0x01, 0x00} },	//blc_one_channel
	{ 0x3503, 1, {0x20, 0x00} }, 	//aec_pk_ctrl0 : aec manual en , agc manual en , gain delay

	//===========Johnny20221219===========
	{ 0x3a05, 1, {0x30, 0x00} },	//Set AEC/AGC auto speed mode
	{ 0x3a11, 1, {0xcc, 0x00} },	//vpt_hight
	//{ 0x3a1b, 1, {0x2c, 0x00} },	//wpt2
	{ 0x3a1b, 1, {0x38, 0x00} },	//wpt2
	//{ 0x3a0f, 1, {0x28, 0x00} },	//wpt
	{ 0x3a0f, 1, {0x30, 0x00} },	//wpt
	//{ 0x3a10, 1, {0x18, 0x00} },	//bpt
	{ 0x3a10, 1, {0x20, 0x00} },	//bpt
	//{ 0x3a1e, 1, {0x1c, 0x00} },	//bpt2
	{ 0x3a1e, 1, {0x14, 0x00} },	//bpt2
	{ 0x3a1f, 1, {0x05, 0x00} },	//vpt_low
	//====================================

	{ 0x302a, 1, {0x00, 0x00} },
	//{ 0x3500, 1, {0x01, 0x00} },	//aec_11_pk_expo[15:8]
	{ 0x3501, 1, {0x1C, 0x00} },	//aec_11_pk_expo[15:8]
	{ 0x3502, 1, {0xD0, 0x00} },	//aec_11_pk_expo[7:0]
	//{ 0x350C, 1, {0x20, 0x00} },	//aec_pk_vts[15:8]
	{ 0x350A, 1, {0x00, 0x00} },	//aec_pk_gain : debug
	{ 0x350B, 1, {0x1F, 0x00} },	//AEC real_gain[7:0]
	{ 0x3A18, 1, {0x03, 0x00} },  //aec gain ceiling
	{ 0x3A19, 1, {0x1F, 0x00} },  //aec gain ceiling
	//{ 0x5001, 1, {0x92, 0x00}},	//Test patter on				// 20221208 On
	//{ 0x3205, 1, {0x00, 0x00} },
};
/*!< Open the OV426 mode1 streaming. */

/* Define initialization mode2 script into streaming mode. */
const SENSOR_CMD OV426_OPEN_MODE2_CMD[] =
{

};
/*!< Open the OV426 mode2 streaming. */

/* Define read information command script. */
const SENSOR_CMD OV426_RUN_READ_CMD[] =
{
	{ 0x5693, 1, {0x00, 0x00}}, 	//AVG
//	{ 0x302a, 1, {0x00, 0x00}}, 	//PRE GAIN
//	{ 0x3a0f, 1, {0x00, 0x00}}, 	//WPT
//	{ 0x5186, 1, {0x00, 0x00}}, 	//WB Red Gain Msb
//	{ 0x5188, 1, {0x00, 0x00}}, 	//WB Green Gain Msb
//	{ 0x518A, 1, {0x00, 0x00}}, 	//WB Blue Gain Msb
	{ 0x300B, 1, {0x00, 0x00}},		//DEVICE ID LOW		4F
};
/*!< Read the OV426 information. */

/* Define setting enable/disable test pattern command script. */
const SENSOR_CMD OV426_TP_CMD[] =
{
	{ 0x5001, 1, {0x00, 0x00}},//Test patter Off
	{ 0x5001, 1, {0x82, 0x00}},//Test patter On		//Color-square
	{ 0x5001, 1, {0x92, 0x00}},//Test patter On		//Black-White
};
/*!< Set the test pattern. */

/* Define setting manual white balance command script. */
const SENSOR_CMD OV426_MWB_CMD[] =
{
	{ 0x5186, 1, {0x00, 0x00}}, 	//WB Red Gain Msb
	{ 0x5187, 1, {0x00, 0x00}}, 	//WB Red Gain Lsb
	{ 0x5188, 1, {0x00, 0x00}}, 	//WB Green Gain Msb
	{ 0x5189, 1, {0x00, 0x00}}, 	//WB Green Gain Lsb
	{ 0x518A, 1, {0x00, 0x00}}, 	//WB Blue Gain Msb
	{ 0x518B, 1, {0x00, 0x00}}, 	//WB Blue Gain Lsb
};
/*!< Set the EWB. */

/* Define setting auto exposure control command script. */
const SENSOR_CMD OV426_AEC_CMD[5][4] =
{
	{
		{ 0x3a1b, 1, {/*0x1c*/0x18, 0x00} },	//wpt2
		{ 0x3a0f, 1, {/*0x18*/0x10, 0x00} },	//wpt
		{ 0x3a10, 1, {/*0x08*/0x04, 0x00} },	//bpt
		{ 0x3a1e, 1, {/*0x05*/0x01, 0x00} },	//bpt2
	},
	{
		{ 0x3a1b, 1, {/*0x24*/0x28, 0x00} },	//wpt2
		{ 0x3a0f, 1, {/*0x20*/0x20, 0x00} },	//wpt
		{ 0x3a10, 1, {/*0x10*/0x14, 0x00} },	//bpt
		{ 0x3a1e, 1, {/*0x08*/0x10, 0x00} },	//bpt2
	},
	{
		{ 0x3a1b, 1, {/*0x3c*/0x38, 0x00} },	//wpt2
		{ 0x3a0f, 1, {/*0x38*/0x30, 0x00} },	//wpt
		{ 0x3a10, 1, {/*0x28*/0x24, 0x00} },	//bpt
		{ 0x3a1e, 1, {/*0x20*/0x20, 0x00} },	//bpt2
	},
	{
		{ 0x3a1b, 1, {/*0x46*/0x48, 0x00} },	//wpt2
		{ 0x3a0f, 1, {/*0x3E*/0x40, 0x00} },	//wpt
		{ 0x3a10, 1, {/*0x2A*/0x34, 0x00} },	//bpt
		{ 0x3a1e, 1, {/*0x22*/0x30, 0x00} },	//bpt2
	},
	{
		{ 0x3a1b, 1, {/*0x7D*/0x58, 0x00} },	//wpt2
		{ 0x3a0f, 1, {/*0x64*/0x50, 0x00} },	//wpt
		{ 0x3a10, 1, {/*0x5A*/0x44, 0x00} },	//bpt
		{ 0x3a1e, 1, {/*0x50*/0x40, 0x00} },	//bpt2
	},

};
/*!< Set the AEC. */

/*@}*/ /* end of group OV426_EXPORTED_CONSTANTS */

/*@}*/ /* end of group OV426_Device_Driver */

/*@}*/ /* end of group OPCOM_Function */


#endif //_OV426_CONFIG_H_

/*** (C) COPYRIGHT 2022 OPCOM Technology Corp. ***/
