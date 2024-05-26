// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2019-2020 Synaptics Incorporated */

#include <linux/bootconfig.h>
#include "avio_util.h"

#include "avio_core.h"
#include "avio_debug.h"
#include "avio_ioctl.h"
#include "avio_sub_module.h"

static int is_bootup_quiescent;

long avio_util_ioctl_unlocked(struct file *filp, unsigned int cmd, unsigned long arg)
{
	AVIO_CTX *hAvioCtx =
		(AVIO_CTX *)avio_sub_module_get_ctx(AVIO_MODULE_TYPE_AVIO);
	long retVal = 0;

	switch (cmd) {
	case POWER_IOCTL_WAIT_RESUME:
	{
		sema_init(&hAvioCtx->resume_sem, 0);
		retVal = down_interruptible(&hAvioCtx->resume_sem);
		break;
	}

	case POWER_IOCTL_START_RESUME:
	{
		up(&hAvioCtx->resume_sem);
		break;
	}

	default:
		break;
	}

	return retVal;
}

int avio_util_get_quiescent_flag(void) {
	return is_bootup_quiescent;
}
EXPORT_SYMBOL(avio_util_get_quiescent_flag);
#if 0
#ifdef CONFIG_BOOT_CONFIG
static int __init bootup_quiescent_setup(char *str)
{
	struct xbc_node *root;
	root = xbc_find_node("androidboot");
	is_bootup_quiescent = xbc_node_find_value(root, "quiescent", NULL) ? 1 : 0;

	avio_trace("cmdline:androidboot.quiescent=%d\n", is_bootup_quiescent);

	return 1;
}

__setup("bootconfig", bootup_quiescent_setup);
#else
static int __init bootup_quiescent_setup(char *str)
{
	get_option(&str, &is_bootup_quiescent);

	avio_trace("cmdline:androidboot.quiescent=%d\n", is_bootup_quiescent);

	return 1;
}

__setup("androidboot.quiescent=", bootup_quiescent_setup);
#endif
#endif
