#include "vc_mipi_modules.h"
#include <linux/v4l2-mediabus.h>


int vc_mod_is_color_sensor(struct vc_desc *desc)
{
	if (desc->sen_type) {
		__u32 len = strnlen(desc->sen_type, 16);
		if (len > 0 && len < 17) {
			return *(desc->sen_type + len - 1) == 'C';
		}
	}
	return 0;
}

static void vc_init_ctrl(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	ctrl->exposure			= (vc_control) { .min =   1, .max = 100000000, .def =  10000 };
	ctrl->gain			= (vc_control) { .min =   0, .max =       255, .def =      0 };
	ctrl->framerate 		= (vc_control) { .min =   0, .max =      1000, .def =   1000 };

	ctrl->csr.sen.mode.l 		= desc->csr_mode;
	ctrl->csr.sen.mode.m 		= 0;

	ctrl->csr.sen.mode_standby	= 0x00; 
	ctrl->csr.sen.mode_operating	= 0x01;
	
	ctrl->csr.sen.shs.l 		= desc->csr_exposure_l;
	ctrl->csr.sen.shs.m 		= desc->csr_exposure_m;
	ctrl->csr.sen.shs.h 		= desc->csr_exposure_h;
	ctrl->csr.sen.shs.u 		= 0;
	
	ctrl->csr.sen.gain.l 		= desc->csr_gain_l;
	ctrl->csr.sen.gain.m 		= desc->csr_gain_h;

	ctrl->csr.sen.h_start.l 	= desc->csr_h_start_l;
	ctrl->csr.sen.h_start.m 	= desc->csr_h_start_h;
	ctrl->csr.sen.v_start.l 	= desc->csr_v_start_l;
	ctrl->csr.sen.v_start.m 	= desc->csr_v_start_h;
	ctrl->csr.sen.o_width.l		= desc->csr_o_width_l;
	ctrl->csr.sen.o_width.m		= desc->csr_o_width_h;
	ctrl->csr.sen.o_height.l	= desc->csr_o_height_l;
	ctrl->csr.sen.o_height.m	= desc->csr_o_height_h;

	ctrl->sen_clk			= desc->clk_ext_trigger;
}

static void vc_init_ctrl_imx183_base(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	ctrl->gain			= (vc_control) { .min =   0, .max =      2047, .def =      0 };

	ctrl->csr.sen.vmax              = (vc_csr4) { .l = 0x7004, .m = 0x7005, .h = 0x7006, .u = 0x0000 };
	ctrl->csr.sen.hmax              = (vc_csr4) { .l = 0x7002, .m = 0x7003, .h = 0x0000, .u = 0x0000 };

	ctrl->expo_period_1H 		= 10000; 	// ns
	ctrl->expo_shs_min		= 5;
	ctrl->expo_vmax			= 3728;

	ctrl->flags			 = FLAG_EXPOSURE_WRITE_VMAX;
	ctrl->flags			|= FLAG_IO_FLASH_ENABLED;
	ctrl->flags			|= FLAG_TRIGGER_EXTERNAL | FLAG_TRIGGER_SELF |
				           FLAG_TRIGGER_SINGLE | FLAG_TRIGGER_SYNC;
}

static void vc_init_ctrl_imx252_base(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	ctrl->gain			= (vc_control) { .min =   0, .max =       511, .def =      0 };

	ctrl->csr.sen.vmax              = (vc_csr4) { .l = 0x0210, .m = 0x0211, .h = 0x0212, .u = 0x0000 };
	ctrl->csr.sen.hmax              = (vc_csr4) { .l = 0x0214, .m = 0x0215, .h = 0x0000, .u = 0x0000 };

	ctrl->expo_period_1H 		= 5000; 	// ns
	ctrl->expo_shs_min		= 10;
	ctrl->expo_vmax			= 2094;

	ctrl->flags			 = FLAG_EXPOSURE_WRITE_VMAX;
	ctrl->flags			|= FLAG_IO_FLASH_ENABLED | FLAG_IO_XTRIG_ENABLED;
	ctrl->flags			|= FLAG_TRIGGER_EXTERNAL | FLAG_TRIGGER_PULSEWIDTH |
					   FLAG_TRIGGER_SELF | FLAG_TRIGGER_SINGLE;
}

static void vc_init_ctrl_imx290_base(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	// ctrl->exposure			= (vc_control) { .min =   29, .max = 7602176, .def =  10000 };
	ctrl->exposure			= (vc_control) { .min =   1, .max = 100000000, .def =  10000 };
	ctrl->gain			= (vc_control) { .min =   0, .max =       255, .def =      0 };
	
	ctrl->csr.sen.vmax              = (vc_csr4) { .l = 0x3018, .m = 0x3019, .h = 0x301A, .u = 0x0000 };
	ctrl->csr.sen.mode_standby	= 0x01;
	ctrl->csr.sen.mode_operating	= 0x00;
	
	//  TODO: Exposure time and framerate don't go together. Check this!
	ctrl->expo_period_1H 		= 10000; 	// ns
	ctrl->expo_shs_min              = 1;
	ctrl->expo_vmax 		= 1125;

	ctrl->flags			= FLAG_EXPOSURE_WRITE_VMAX;
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX178/IMX178C

static void vc_init_ctrl_imx178(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX178\n", __FUNCTION__);

	vc_init_ctrl_imx183_base(ctrl, desc);

	ctrl->expo_timing[0] 		= (vc_timing) { 2, FORMAT_RAW08, .clk =  680 };
	ctrl->expo_timing[1] 		= (vc_timing) { 2, FORMAT_RAW10, .clk =  840 };
	ctrl->expo_timing[2] 		= (vc_timing) { 2, FORMAT_RAW12, .clk =  984 };
	ctrl->expo_timing[3] 		= (vc_timing) { 2, FORMAT_RAW14, .clk = 1156 };
	ctrl->expo_timing[4] 		= (vc_timing) { 4, FORMAT_RAW08, .clk =  600 };
	ctrl->expo_timing[5] 		= (vc_timing) { 4, FORMAT_RAW10, .clk =  600 };
	ctrl->expo_timing[6] 		= (vc_timing) { 4, FORMAT_RAW12, .clk =  680 };
	ctrl->expo_timing[7] 		= (vc_timing) { 4, FORMAT_RAW14, .clk = 1156 };

	ctrl->expo_shs_min		= 9;
	ctrl->expo_vmax 		= 2145;
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX183/IMX183C

static void vc_init_ctrl_imx183(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX183\n", __FUNCTION__);

	vc_init_ctrl_imx183_base(ctrl, desc);

	ctrl->expo_timing[0] 		= (vc_timing) { 2, FORMAT_RAW08, .clk = 1440 };
	ctrl->expo_timing[1] 		= (vc_timing) { 2, FORMAT_RAW10, .clk = 1440 };
	ctrl->expo_timing[2] 		= (vc_timing) { 2, FORMAT_RAW12, .clk = 1724 };
	ctrl->expo_timing[3] 		= (vc_timing) { 4, FORMAT_RAW08, .clk =  720 };
	ctrl->expo_timing[4] 		= (vc_timing) { 4, FORMAT_RAW10, .clk =  720 };
	ctrl->expo_timing[5] 		= (vc_timing) { 4, FORMAT_RAW12, .clk =  862 };

	ctrl->expo_vmax			= 3728;
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX226/IMX226C

static void vc_init_ctrl_imx226(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX226\n", __FUNCTION__);

	vc_init_ctrl_imx183_base(ctrl, desc);


	ctrl->expo_vmax 		= 3079;

	ctrl->flags			|= FLAG_TRIGGER_STREAM_EDGE | FLAG_TRIGGER_STREAM_LEVEL;
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX250/IMX250C

static void vc_init_ctrl_imx250(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX250\n", __FUNCTION__);

	vc_init_ctrl_imx252_base(ctrl, desc);
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX252/IMX252C

static void vc_init_ctrl_imx252(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX252\n", __FUNCTION__);

	vc_init_ctrl_imx252_base(ctrl, desc);
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX264/IMX264C

static void vc_init_ctrl_imx264(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX264\n", __FUNCTION__);

	vc_init_ctrl_imx252_base(ctrl, desc);
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX265/IMX265C

static void vc_init_ctrl_imx265(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX265\n", __FUNCTION__);

	vc_init_ctrl_imx252_base(ctrl, desc);

}


// ------------------------------------------------------------------------------------------------
//  Settings for IMX290

static void vc_init_ctrl_imx290(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX290\n", __FUNCTION__);

	vc_init_ctrl_imx290_base(ctrl, desc);

	ctrl->frame.width		= 1920;
	ctrl->frame.height		= 1080;
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX296/IMX296C

static void vc_init_ctrl_imx296(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX296\n", __FUNCTION__);

	ctrl->gain			= (vc_control) { .min =   0, .max =       512, .def =      0 };
	
	ctrl->csr.sen.vmax              = (vc_csr4) { .l = 0x3010, .m = 0x3011, .h = 0x3012, .u = 0x0000 };
	ctrl->csr.sen.mode              = (vc_csr2) { .l = 0x3000, .m = 0x300A };
	ctrl->csr.sen.mode_standby	= 0x01;
	ctrl->csr.sen.mode_operating	= 0x00;

	ctrl->sen_clk			= 54000000;	// Hz
	ctrl->expo_period_1H 		= 14815;	// ns
	ctrl->expo_shs_min              = 5;
	ctrl->expo_vmax 		= 1118;

	ctrl->flags			 = FLAG_EXPOSURE_WRITE_VMAX;
	ctrl->flags			|= FLAG_IO_FLASH_ENABLED | FLAG_IO_XTRIG_ENABLED;
	ctrl->flags			|= FLAG_TRIGGER_EXTERNAL | FLAG_TRIGGER_PULSEWIDTH | FLAG_TRIGGER_SELF;
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX327C

static void vc_init_ctrl_imx327(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX327\n", __FUNCTION__);

	vc_init_ctrl_imx290_base(ctrl, desc);

	ctrl->flags			= FLAG_EXPOSURE_WRITE_VMAX;
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX392/IMX392C

static void vc_init_ctrl_imx392(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX392\n", __FUNCTION__);

	vc_init_ctrl_imx252_base(ctrl, desc);
}

// ------------------------------------------------------------------------------------------------
//  Settings for IMX412C

static void vc_init_ctrl_imx412(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for IMX412\n", __FUNCTION__);
	
	ctrl->exposure			= (vc_control) { .min = 190, .max =    405947, .def =  10000 };
	ctrl->gain			= (vc_control) { .min =   0, .max =      1023, .def =      0 };
	ctrl->framerate 		= (vc_control) { .min =   0, .max =        41, .def =     41 };

	ctrl->expo_factor               = 31755000;
	ctrl->expo_toffset 		= 5975;

	// No VMAX value present. No TRIGGER and FLASH capability.
	ctrl->flags			= FLAG_EXPOSURE_SIMPLE;
	ctrl->flags		       |= FLAG_IO_FLASH_ENABLED;
}

// ------------------------------------------------------------------------------------------------
//  Settings for OV9281
//
//  TODO: 
//  - Set frame length to increase exposure time. 
//    Maximum exposure time is frame length -25 row periods, where frame length is set by registers
//    {0x380E, 0x380F}
//  - Trigger mode could not be activated. Wenn 0x0108 = 0x01 exposure time has no effect.

static void vc_init_ctrl_ov9281(struct vc_ctrl *ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_notice(dev, "%s(): Initialising module control for OV9281\n", __FUNCTION__);
	
	// exposure max => L4T 32.5.0+: 8253, L4T 32.6.1+: 8244 (if > black line in image)
	ctrl->exposure			= (vc_control) { .min =  29, .max =      8244, .def =   1000 };
	ctrl->gain			= (vc_control) { .min =   1, .max =       255, .def =      0 };

	ctrl->csr.sen.flash_duration	= (vc_csr4) { .l = 0x3928, .m = 0x3927, .h = 0x3926, .u = 0x3925 };
	ctrl->csr.sen.flash_offset	= (vc_csr4) { .l = 0x3924, .m = 0x3923, .h = 0x3922, .u = 0x0000 };
	// NOTE: Modules rom table contains swapped address assigment.
	ctrl->csr.sen.gain 		= (vc_csr2) { .l = 0x3509, .m = 0x0000 };
	
	ctrl->sen_clk			= 25000000;
	ctrl->expo_factor               = 1758241; 	// (1000 << 4)/9100
	ctrl->expo_toffset 		= 0;
	ctrl->flash_factor		= ctrl->expo_factor >> 4;
	ctrl->flash_toffset		= 4;

	ctrl->flags		 	= FLAG_EXPOSURE_SIMPLE;
	ctrl->flags		       |= FLAG_IO_FLASH_DURATION;
	ctrl->flags		       |= FLAG_IO_FLASH_ENABLED | FLAG_IO_XTRIG_ENABLED;
	ctrl->flags		       |= FLAG_TRIGGER_EXTERNAL;
}


int vc_mod_ctrl_init(struct vc_ctrl* ctrl, struct vc_desc* desc)
{
	struct device *dev = &ctrl->client_mod->dev;

	vc_init_ctrl(ctrl, desc);

	switch(desc->mod_id) {
	case MOD_ID_IMX178: vc_init_ctrl_imx178(ctrl, desc); break;
	case MOD_ID_IMX183: vc_init_ctrl_imx183(ctrl, desc); break;
	case MOD_ID_IMX226: vc_init_ctrl_imx226(ctrl, desc); break;
	case MOD_ID_IMX250: vc_init_ctrl_imx250(ctrl, desc); break;
	case MOD_ID_IMX252: vc_init_ctrl_imx252(ctrl, desc); break;
	case MOD_ID_IMX264: vc_init_ctrl_imx264(ctrl, desc); break;
	case MOD_ID_IMX265: vc_init_ctrl_imx265(ctrl, desc); break;
	case MOD_ID_IMX290: vc_init_ctrl_imx290(ctrl, desc); break;
	case MOD_ID_IMX296: vc_init_ctrl_imx296(ctrl, desc); break;
	case MOD_ID_IMX327: vc_init_ctrl_imx327(ctrl, desc); break;
	case MOD_ID_IMX392: vc_init_ctrl_imx392(ctrl, desc); break;
	case MOD_ID_IMX412: vc_init_ctrl_imx412(ctrl, desc); break;
	case MOD_ID_OV9281: vc_init_ctrl_ov9281(ctrl, desc); break;
	default:
		vc_err(dev, "%s(): Detected module not supported!\n", __FUNCTION__);
		return 1;
	}

	return 0;
}