/*
 * arch/arm/plat-omap/include/mach/dmtimer.h
 *
 * OMAP Dual-Mode Timers
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * Tarun Kanti DebBarma <tarun.kanti@ti.com>
 * Thara Gopinath <thara@ti.com>
 *
 * Platform device conversion and hwmod support.
 *
 * Copyright (C) 2005 Nokia Corporation
 * Author: Lauri Leukkunen <lauri.leukkunen@nokia.com>
 * PWM and clock framwork support by Timo Teras.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __ASM_ARCH_DMTIMER_H
#define __ASM_ARCH_DMTIMER_H

#include <linux/list.h>

/* clock sources */
#define OMAP_TIMER_SRC_SYS_CLK			0x00
#define OMAP_TIMER_SRC_32_KHZ			0x01
#define OMAP_TIMER_SRC_EXT_CLK			0x02

/* timer interrupt enable bits */
#define OMAP_TIMER_INT_CAPTURE			(1 << 2)
#define OMAP_TIMER_INT_OVERFLOW			(1 << 1)
#define OMAP_TIMER_INT_MATCH			(1 << 0)

/* trigger types */
#define OMAP_TIMER_TRIGGER_NONE			0x00
#define OMAP_TIMER_TRIGGER_OVERFLOW		0x01
#define OMAP_TIMER_TRIGGER_OVERFLOW_AND_COMPARE	0x02

/*
 * IP revision identifier so that Highlander IP
 * in OMAP4 can be distinguished.
 */
#define OMAP_TIMER_IP_VERSION_1			0x1
#define OMAP_TIMER_IP_VERSION_2			0x2

#define OMAP_DMTIMER_1		(1)
#define OMAP_DMTIMER_2		(2)
#define OMAP_DMTIMER_3		(3)
#define OMAP_DMTIMER_4		(4)
#define OMAP_DMTIMER_5		(5)
#define OMAP_DMTIMER_6		(6)
#define OMAP_DMTIMER_7		(7)
#define OMAP_DMTIMER_8		(8)
#define OMAP_DMTIMER_9		(9)
#define OMAP_DMTIMER_10		(10)
#define OMAP_DMTIMER_11		(11)

#define DMTIMER1_FCLK		(0x4A307840)
#define DMTIMER2_FCLK		(0x4A009438)
#define DMTIMER3_FCLK		(0x4A009440)
#define DMTIMER4_FCLK		(0x4A009448)
#define DMTIMER5_FCLK		(0x4A004568)
#define DMTIMER6_FCLK		(0x4A004570)
#define DMTIMER7_FCLK		(0x4A004578)
#define DMTIMER8_FCLK		(0x4A004580)
#define DMTIMER9_FCLK		(0x4A009450)
#define DMTIMER10_FCLK		(0x4A009428)
#define DMTIMER11_FCLK		(0x4A009430)

struct omap_dm_timer {
	int id;
	unsigned int io_base;
	unsigned int fclk_reg;
	unsigned int fclk;
	unsigned reserved:1;
	unsigned enabled:1;
	unsigned posted:1;
	unsigned needs_manual_reset:1;
	u8 func_offset;
	u8 intr_offset;
	struct list_head node;
};

struct dmtimer_platform_data {
	int timer_ip_type;
	u32 needs_manual_reset:1;
	int id;
	unsigned int io_base;
	unsigned int fclk_reg;
};

struct omap_dm_timer *omap_dm_timer_request_specific(int timer_id, unsigned int fclk);
int omap_dm_timer_free(struct omap_dm_timer *timer);
int omap_dm_timer_enable(struct omap_dm_timer *timer);
int omap_dm_timer_disable(struct omap_dm_timer *timer);

int omap_dm_timer_start(struct omap_dm_timer *timer);
int omap_dm_timer_stop(struct omap_dm_timer *timer);

int omap_dm_timer_set_source(struct omap_dm_timer *timer, int source);
int omap_dm_timer_set_load(struct omap_dm_timer *timer, int autoreload,
	unsigned int value);
int omap_dm_timer_set_load_start(struct omap_dm_timer *timer,
	int autoreload, unsigned int value);
int omap_dm_timer_set_match(struct omap_dm_timer *timer, int enable,
	unsigned int match);
int omap_dm_timer_set_pwm(struct omap_dm_timer *timer, int def_on,
	int toggle, int trigger);
int omap_dm_timer_set_prescaler(struct omap_dm_timer *timer, int prescaler);

int omap_dm_timer_init(struct dmtimer_platform_data *pdata);

#endif /* __ASM_ARCH_DMTIMER_H */
