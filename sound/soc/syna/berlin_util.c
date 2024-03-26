// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2018-2020 Synaptics Incorporated */

#include <linux/module.h>
#include <sound/soc.h>

#include "berlin_pcm.h"
#include "aio_hal.h"
#include "berlin_util.h"

#define APLL_RATE_32K (16384000 * 8)
#define APLL_RATE_44_1K (22579200 * 8)
#define APLL_RATE_48K (24576000 * 8)

static int berlin_get_pll(u32 fs)
{
	unsigned long apll;

	switch (fs) {
	case 11025:
	case 22050:
	case 44100:
	case 88200:
	case 176400:
		apll = APLL_RATE_44_1K;
		break;
	case 8000:
	case 16000:
	case 32000:
	case 64000:
		apll = APLL_RATE_32K;
		break;
	case 12000:
	case 24000:
	case 48000:
	case 96000:
	case 192000:
	case 384000:
		apll = APLL_RATE_48K;
		break;
	default:
		apll = APLL_RATE_48K;
		break;
	}
	return apll;
}

int berlin_set_pll(void *aio_handle, u32 apll_id, u32 fs)
{
	unsigned long apll;

	if (apll_id >= AIO_APLL_NUM) {
		pr_err("apll%d not supported", apll_id);
		return 0;
	}

	apll = berlin_get_pll(fs);
	if (aio_get_clk_rate(aio_handle, apll_id) != apll) {
		aio_clk_enable(aio_handle, apll_id, false);
		aio_set_clk_rate(aio_handle, apll_id, apll);
		aio_clk_enable(aio_handle, apll_id, true);
		pr_info("set apll%d to %lu, fs %d", apll_id, apll, fs);
	} else {
		pr_info("apll%d already set %lu, fs %d", apll_id, apll, fs);
	}

	return 0;
}
EXPORT_SYMBOL(berlin_set_pll);

u32 berlin_get_div(u32 fs)
{
	u32 div;

	switch (fs) {
	case 8000:
	case 11025:
	case 12000:
		div = AIO_DIV32;
		break;
	case 16000:
	case 22050:
	case 24000:
		div = AIO_DIV16;
		break;
	case 32000:
	case 44100:
	case 48000:
		div = AIO_DIV8;
		break;
	case 64000:
	case 88200:
	case 96000:
		div = AIO_DIV4;
		break;
	default:
		div = AIO_DIV8;
		break;
	}

	return div;
}
EXPORT_SYMBOL(berlin_get_div);

u32 berlin_get_bclk_div(u32 mclk, u32 bclk)
{
	u32 bclk_div;

	bclk_div = mclk/(bclk);
	bclk_div = ilog2(bclk_div);
	pr_debug("%s bclk_div %d mclk %d bclk %d", __func__, bclk_div, mclk, bclk);
	return bclk_div;
}
EXPORT_SYMBOL(berlin_get_bclk_div);

/*
 * Get the channel resolution (number of valid bits in a half period of FSYNC)
 */
u32 berlin_get_sample_resolution(u32 word_w)
{
	u32 dfm;

	switch (word_w) {
	case 16:
		dfm = AIO_16DFM;
		break;
	case 24:
		dfm = AIO_24DFM;
		break;
	case 32:
	default:
		dfm = AIO_32DFM;
		break;
	}

	return dfm;
}
EXPORT_SYMBOL(berlin_get_sample_resolution);

/*
 * Get the half period of FSYNC (sampling rate) in terms of number of bit-clocks
 */
u32 berlin_get_sample_period_in_bclk(u32 word_s)
{
	u32 cfm;

	switch (word_s) {
	case 16:
		cfm = AIO_16CFM;
		break;
	case 24:
		cfm = AIO_24CFM;
		break;
	case 32:
	default:
		cfm = AIO_32CFM;
		break;
	}

	return cfm;
}
EXPORT_SYMBOL(berlin_get_sample_period_in_bclk);

u32 berlin_get_cfm(u32 word_s)
{
	u32 cfm;

	switch (word_s) {
	case 16:
		cfm = AIO_16CFM;
		break;
	case 24:
		cfm = AIO_24CFM;
		break;
	case 32:
	default:
		cfm = AIO_32CFM;
		break;
	}

	return cfm;
}
EXPORT_SYMBOL(berlin_get_cfm);

