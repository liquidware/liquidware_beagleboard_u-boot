/*
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 * Syed Mohammed Khasim <khasim@ti.com>
 *
 * Referred to Linux Kernel DSS driver files for OMAP3 by
 * Tomi Valkeinen from drivers/video/omap2/dss/
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation's version 2 and any
 * later version the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/dss.h>
#include <fat.h>
#include <bmp_layout.h>
#include <lcd.h>
#include <watchdog.h>

/*
 * Configure VENC for a given Mode (NTSC / PAL)
 */
void omap3_dss_venc_config(const struct venc_regs *venc_cfg,
				u32 height, u32 width)
{
	struct venc_regs *venc = (struct venc_regs *) OMAP3_VENC_BASE;
	struct dss_regs *dss = (struct dss_regs *) OMAP3_DSS_BASE;
	struct dispc_regs *dispc = (struct dispc_regs *) OMAP3_DISPC_BASE;

	writel(venc_cfg->status, &venc->status);
	writel(venc_cfg->f_control, &venc->f_control);
	writel(venc_cfg->vidout_ctrl, &venc->vidout_ctrl);
	writel(venc_cfg->sync_ctrl, &venc->sync_ctrl);
	writel(venc_cfg->llen, &venc->llen);
	writel(venc_cfg->flens, &venc->flens);
	writel(venc_cfg->hfltr_ctrl, &venc->hfltr_ctrl);
	writel(venc_cfg->cc_carr_wss_carr, &venc->cc_carr_wss_carr);
	writel(venc_cfg->c_phase, &venc->c_phase);
	writel(venc_cfg->gain_u, &venc->gain_u);
	writel(venc_cfg->gain_v, &venc->gain_v);
	writel(venc_cfg->gain_y, &venc->gain_y);
	writel(venc_cfg->black_level, &venc->black_level);
	writel(venc_cfg->blank_level, &venc->blank_level);
	writel(venc_cfg->x_color, &venc->x_color);
	writel(venc_cfg->m_control, &venc->m_control);
	writel(venc_cfg->bstamp_wss_data, &venc->bstamp_wss_data);
	writel(venc_cfg->s_carr, &venc->s_carr);
	writel(venc_cfg->line21, &venc->line21);
	writel(venc_cfg->ln_sel, &venc->ln_sel);
	writel(venc_cfg->l21__wc_ctl, &venc->l21__wc_ctl);
	writel(venc_cfg->htrigger_vtrigger, &venc->htrigger_vtrigger);
	writel(venc_cfg->savid__eavid, &venc->savid__eavid);
	writel(venc_cfg->flen__fal, &venc->flen__fal);
	writel(venc_cfg->lal__phase_reset, &venc->lal__phase_reset);
	writel(venc_cfg->hs_int_start_stop_x,
				&venc->hs_int_start_stop_x);
	writel(venc_cfg->hs_ext_start_stop_x,
				&venc->hs_ext_start_stop_x);
	writel(venc_cfg->vs_int_start_x, &venc->vs_int_start_x);
	writel(venc_cfg->vs_int_stop_x__vs_int_start_y,
			&venc->vs_int_stop_x__vs_int_start_y);
	writel(venc_cfg->vs_int_stop_y__vs_ext_start_x,
			&venc->vs_int_stop_y__vs_ext_start_x);
	writel(venc_cfg->vs_ext_stop_x__vs_ext_start_y,
			&venc->vs_ext_stop_x__vs_ext_start_y);
	writel(venc_cfg->vs_ext_stop_y, &venc->vs_ext_stop_y);
	writel(venc_cfg->avid_start_stop_x, &venc->avid_start_stop_x);
	writel(venc_cfg->avid_start_stop_y, &venc->avid_start_stop_y);
	writel(venc_cfg->fid_int_start_x__fid_int_start_y,
				&venc->fid_int_start_x__fid_int_start_y);
	writel(venc_cfg->fid_int_offset_y__fid_ext_start_x,
				&venc->fid_int_offset_y__fid_ext_start_x);
	writel(venc_cfg->fid_ext_start_y__fid_ext_offset_y,
				&venc->fid_ext_start_y__fid_ext_offset_y);
	writel(venc_cfg->tvdetgp_int_start_stop_x,
				&venc->tvdetgp_int_start_stop_x);
	writel(venc_cfg->tvdetgp_int_start_stop_y,
				&venc->tvdetgp_int_start_stop_y);
	writel(venc_cfg->gen_ctrl, &venc->gen_ctrl);
	writel(venc_cfg->output_control, &venc->output_control);
	writel(venc_cfg->dac_b__dac_c, &venc->dac_b__dac_c);

	/* Configure DSS for VENC Settings */
	writel(VENC_DSS_CONFIG, &dss->control);

	/* Configure height and width for Digital out */
	writel(((height << DIG_LPP_SHIFT) | width), &dispc->size_dig);
}

/*
 * Fill frame buffer with initial screen data
 */
void omap3_setup_framebuffer(unsigned int fb_addr, unsigned int width, unsigned int height)
{
        struct dispc_regs *dispc = (struct dispc_regs *) OMAP3_DISPC_BASE;
        unsigned long fb_size = width * height * 3;
        unsigned int v = 0;
        /* Initialize framebuffer memory to black pattern */
        memset((unsigned char*)fb_addr, 0x00, fb_size);
        /* copy splash image row data to the frame buffer */
        //memcpy((unsigned char*)fb_addr, bit_data, size);

        /* set pointer to the frame buffer */
        writel(fb_addr, &dispc->ba0);
        writel(fb_addr, &dispc->ba1);

        v |= ((height-1) << 16) | (width -1); // 800x480 = 0x01df031f
        writel(v, &dispc->size);
        //dss_write_reg(DISPC_GFX_ATTRIBUTES, 0x00000013); /* 0x413 */
        //writel(0x00000091, &dispc->attributes);
        writel(0x00000013, &dispc->attributes);
        //dss_write_reg(DISPC_GFX_FIFO_THRESHOLD, 0x00fc00c0);
        writel(0x03c003ff, &dispc->fifo_threshold);
        writel(0x00000400, &dispc->fifo_size_status);
        writel(0x00000001, &dispc->row_inc);
        writel(0x00000001, &dispc->pixel_inc);
        writel(0x00000000, &dispc->window_skip);
        //dss_write_reg(DISPC_GFX_TABLE_BA, 0x807ff000);
        writel(0x00000000, &dispc->table_ba);
}

int omap3_display_bitmap(ulong bmp_image, ulong lcd_base, int x, int y, int panel_width, int panel_height)
{
#if !defined(CONFIG_MCC200)
	ushort *cmap = NULL;
#endif
	ushort *cmap_base = NULL;
	ushort i, j;
	uchar *fb;
	bmp_image_t *bmp=(bmp_image_t *)bmp_image;
	uchar *bmap;
	ushort padded_line;
	unsigned long width, height, byte_width;
	unsigned long pwidth = panel_width; //panel_info.vl_col;
	unsigned long pheight = panel_height;
	unsigned long lcd_line_length = (panel_width * 8*3) / 8;
	unsigned colors, bpix, bmp_bpix;
	unsigned long compression;
#if defined CONFIG_PXA250 || defined CONFIG_PXA27X || defined CONFIG_CPU_MONAHANS
	struct pxafb_info *fbi = &panel_info.pxa;
#elif defined(CONFIG_MPC823)
	volatile immap_t *immr = (immap_t *) CONFIG_SYS_IMMR;
	volatile cpm8xx_t *cp = &(immr->im_cpm);
#endif

	if (!((bmp->header.signature[0]=='B') &&
		(bmp->header.signature[1]=='M'))) {
		printf ("Error: no valid bmp image at %lx\n", bmp_image);
		return 1;
	}

	width = le32_to_cpu (bmp->header.width);
	height = le32_to_cpu (bmp->header.height);
	bmp_bpix = le16_to_cpu(bmp->header.bit_count);
	colors = 1 << bmp_bpix;
	compression = le32_to_cpu (bmp->header.compression);

	bpix = 24; //NBITS(panel_info.vl_bpix);

	if (bpix != 24) {
		printf ("Error: %d bit/pixel mode, but BMP has %d bit/pixel\n",
			bpix, bmp_bpix);
		return 1;
	}

	/* We support displaying 8bpp BMPs on 16bpp LCDs */
	if (bpix != bmp_bpix) {
		printf ("Error: %d bit/pixel mode, but BMP has %d bit/pixel\n",
			bpix,
			le16_to_cpu(bmp->header.bit_count));
		return 1;
	}

	printf ("Display-bmp: %d x %d  with %d colors, panel bpix=%d, bmp_bpix=%d\n",
		(int)width, (int)height, (int)colors, bpix, bmp_bpix);

	/*
	 *  BMP format for Monochrome assumes that the state of a
	 * pixel is described on a per Bit basis, not per Byte.
	 *  So, in case of Monochrome BMP we should align widths
	 * on a byte boundary and convert them from Bit to Byte
	 * units.
	 *  Probably, PXA250 and MPC823 process 1bpp BMP images in
	 * their own ways, so make the converting to be MCC200
	 * specific.
	 */

	padded_line = (width&0x3) ? ((width&~0x3)+4) : (width);

#ifdef CONFIG_SPLASH_SCREEN_ALIGN
	if (x == BMP_ALIGN_CENTER)
		x = max(0, (pwidth - width) / 2);
	else if (x < 0)
		x = max(0, pwidth - width + x + 1);

	if (y == BMP_ALIGN_CENTER)
		y = max(0, (panel_info.vl_row - height) / 2);
	else if (y < 0)
		y = max(0, panel_info.vl_row - height + y + 1);
#endif /* CONFIG_SPLASH_SCREEN_ALIGN */

	if ((x + width)>pwidth)
		width = pwidth - x;
	if ((y + height)>pheight)
		height = pheight - y;

	printf("bmp - clamping to width=%d, height=%d, padded_line=%d, lcd_line_length=%d\n",
		   width, height, padded_line, lcd_line_length);

	bmap = (uchar *)bmp + le32_to_cpu (bmp->header.data_offset);
	fb   = (uchar *) (lcd_base + (panel_width * panel_height *3) - 1 );

    byte_width = width;

    for (i = 0; i < height; ++i) {
        WATCHDOG_RESET();
        for (j = 0; j < width; j++) {
                *(fb - 2) = *(bmap++);
                *(fb - 1) = *(bmap++);
                *(fb - 0) = *(bmap++);
                fb-=3;
        }
    }

	return (0);
}

/*
 * Configure Panel Specific Parameters
 */
void omap3_dss_panel_config(const struct panel_config *panel_cfg)
{
    #define FB_ORIGIN 0x8fddc000
    #define FB_WIDTH 800
    #define FB_HEIGHT 480
    #define BMP_ADDR 0x83000000
    #define BMP_ADDR_STR "0x83000000"

	struct dispc_regs *dispc = (struct dispc_regs *) OMAP3_DISPC_BASE;
    cmd_tbl_t *bcmd;
   	char * const argv_mmc[6] = { "mmc", "part", "0", NULL, NULL, NULL };
    char * const argv_fatload[6] = { "fatload", "mmc", "0:1", BMP_ADDR_STR, "logo.bmp", NULL };

	writel(panel_cfg->timing_h, &dispc->timing_h);
	writel(panel_cfg->timing_v, &dispc->timing_v);
	writel(panel_cfg->pol_freq, &dispc->pol_freq);
	writel(panel_cfg->divisor, &dispc->divisor);

	writel(panel_cfg->lcd_size, &dispc->size_lcd);
	writel((panel_cfg->load_mode << FRAME_MODE_SHIFT), &dispc->config);
	writel(((panel_cfg->panel_type << TFTSTN_SHIFT) |
		(panel_cfg->data_lines << DATALINES_SHIFT)), &dispc->control);
	writel(panel_cfg->panel_color, &dispc->default_color0);

    omap3_setup_framebuffer(FB_ORIGIN, FB_WIDTH, FB_HEIGHT);

	bcmd = find_cmd("mmc");
	if (!bcmd) {
		printf("Error - 'mmc' command not present.\n");
        return;
    }
	if (do_mmcops(bcmd, 0, 5, argv_mmc) != 0) {
        printf("Error - mmc - cannot execute 'mmc part 0' \n");
        return;
    }

    bcmd = find_cmd("fatload");
	if (!bcmd) {
		printf("Error - 'fatload' command not present.\n");
        return;
    }
    if (do_fat_fsload(bcmd, 0, 5, argv_fatload) != 0) {
        printf("Error - fatload - cannot find logo.bmp\n");
        return;
    }
    if (omap3_display_bitmap(BMP_ADDR, FB_ORIGIN, 0, 0, FB_WIDTH, FB_HEIGHT) != 0) {
        printf("Error - bmp - cannot draw bitmap\n");
        return;
    }
}

/*
 * Enable LCD and DIGITAL OUT in DSS
 */
void omap3_dss_enable(void)
{
	struct dispc_regs *dispc = (struct dispc_regs *) OMAP3_DISPC_BASE;
	u32 l = 0;

	l = readl(&dispc->control);
	l |= DISPC_ENABLE;
	writel(l, &dispc->control);
}


