/*
 * linux/arch/arm/plat-omap/dmtimer.c
 *
 * OMAP Dual-Mode Timers
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * Tarun Kanti DebBarma <tarun.kanti@ti.com>
 * Thara Gopinath <thara@ti.com>
 *
 * dmtimer adaptation to platform_driver.
 *
 * Copyright (C) 2005 Nokia Corporation
 * OMAP2 support by Juha Yrjola
 * API improvements and OMAP2 clock framework support by Timo Teras
 *
 * Copyright (C) 2009 Texas Instruments
 * Added OMAP4 support - Santosh Shilimkar <santosh.shilimkar@ti.com>
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

#include <common.h>
#include <command.h>
#include <asm/errno.h>
#include <asm/io.h>

#include <dmtimer.h>

/* register offsets */
#define _OMAP_TIMER_ID_OFFSET		0x00
#define _OMAP_TIMER_OCP_CFG_OFFSET	0x10
#define _OMAP_TIMER_SYS_STAT_OFFSET	0x14
#define _OMAP_TIMER_STAT_OFFSET		0x18
#define _OMAP_TIMER_INT_EN_OFFSET	0x1c
#define _OMAP_TIMER_WAKEUP_EN_OFFSET	0x20
#define _OMAP_TIMER_CTRL_OFFSET		0x24
#define		OMAP_TIMER_CTRL_GPOCFG		(1 << 14)
#define		OMAP_TIMER_CTRL_CAPTMODE	(1 << 13)
#define		OMAP_TIMER_CTRL_PT		(1 << 12)
#define		OMAP_TIMER_CTRL_TCM_LOWTOHIGH	(0x1 << 8)
#define		OMAP_TIMER_CTRL_TCM_HIGHTOLOW	(0x2 << 8)
#define		OMAP_TIMER_CTRL_TCM_BOTHEDGES	(0x3 << 8)
#define		OMAP_TIMER_CTRL_SCPWM		(1 << 7)
#define		OMAP_TIMER_CTRL_CE		(1 << 6) /* compare enable */
#define		OMAP_TIMER_CTRL_PRE		(1 << 5) /* prescaler enable */
#define		OMAP_TIMER_CTRL_PTV_SHIFT	2 /* prescaler value shift */
#define		OMAP_TIMER_CTRL_POSTED		(1 << 2)
#define		OMAP_TIMER_CTRL_AR		(1 << 1) /* auto-reload enable */
#define		OMAP_TIMER_CTRL_ST		(1 << 0) /* start timer */
#define _OMAP_TIMER_COUNTER_OFFSET	0x28
#define _OMAP_TIMER_LOAD_OFFSET		0x2c
#define _OMAP_TIMER_TRIGGER_OFFSET	0x30
#define _OMAP_TIMER_WRITE_PEND_OFFSET	0x34
#define		WP_NONE			0	/* no write pending bit */
#define		WP_TCLR			(1 << 0)
#define		WP_TCRR			(1 << 1)
#define		WP_TLDR			(1 << 2)
#define		WP_TTGR			(1 << 3)
#define		WP_TMAR			(1 << 4)
#define		WP_TPIR			(1 << 5)
#define		WP_TNIR			(1 << 6)
#define		WP_TCVR			(1 << 7)
#define		WP_TOCR			(1 << 8)
#define		WP_TOWR			(1 << 9)
#define _OMAP_TIMER_MATCH_OFFSET	0x38
#define _OMAP_TIMER_CAPTURE_OFFSET	0x3c
#define _OMAP_TIMER_IF_CTRL_OFFSET	0x40
#define _OMAP_TIMER_CAPTURE2_OFFSET		0x44	/* TCAR2, 34xx only */
#define _OMAP_TIMER_TICK_POS_OFFSET		0x48	/* TPIR, 34xx only */
#define _OMAP_TIMER_TICK_NEG_OFFSET		0x4c	/* TNIR, 34xx only */
#define _OMAP_TIMER_TICK_COUNT_OFFSET		0x50	/* TCVR, 34xx only */
#define _OMAP_TIMER_TICK_INT_MASK_SET_OFFSET	0x54	/* TOCR, 34xx only */
#define _OMAP_TIMER_TICK_INT_MASK_COUNT_OFFSET	0x58	/* TOWR, 34xx only */

/* register offsets with the write pending bit encoded */
#define	WPSHIFT					16

#define OMAP_TIMER_ID_REG			(_OMAP_TIMER_ID_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_OCP_CFG_REG			(_OMAP_TIMER_OCP_CFG_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_SYS_STAT_REG			(_OMAP_TIMER_SYS_STAT_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_STAT_REG			(_OMAP_TIMER_STAT_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_INT_EN_REG			(_OMAP_TIMER_INT_EN_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_WAKEUP_EN_REG		(_OMAP_TIMER_WAKEUP_EN_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_CTRL_REG			(_OMAP_TIMER_CTRL_OFFSET \
							| (WP_TCLR << WPSHIFT))

#define OMAP_TIMER_COUNTER_REG			(_OMAP_TIMER_COUNTER_OFFSET \
							| (WP_TCRR << WPSHIFT))

#define OMAP_TIMER_LOAD_REG			(_OMAP_TIMER_LOAD_OFFSET \
							| (WP_TLDR << WPSHIFT))

#define OMAP_TIMER_TRIGGER_REG			(_OMAP_TIMER_TRIGGER_OFFSET \
							| (WP_TTGR << WPSHIFT))

#define OMAP_TIMER_WRITE_PEND_REG		(_OMAP_TIMER_WRITE_PEND_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_MATCH_REG			(_OMAP_TIMER_MATCH_OFFSET \
							| (WP_TMAR << WPSHIFT))

#define OMAP_TIMER_CAPTURE_REG			(_OMAP_TIMER_CAPTURE_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_IF_CTRL_REG			(_OMAP_TIMER_IF_CTRL_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_CAPTURE2_REG			(_OMAP_TIMER_CAPTURE2_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_TICK_POS_REG			(_OMAP_TIMER_TICK_POS_OFFSET \
							| (WP_TPIR << WPSHIFT))

#define OMAP_TIMER_TICK_NEG_REG			(_OMAP_TIMER_TICK_NEG_OFFSET \
							| (WP_TNIR << WPSHIFT))

#define OMAP_TIMER_TICK_COUNT_REG		(_OMAP_TIMER_TICK_COUNT_OFFSET \
							| (WP_TCVR << WPSHIFT))

#define OMAP_TIMER_TICK_INT_MASK_SET_REG				\
		(_OMAP_TIMER_TICK_INT_MASK_SET_OFFSET | (WP_TOCR << WPSHIFT))

#define OMAP_TIMER_TICK_INT_MASK_COUNT_REG				\
		(_OMAP_TIMER_TICK_INT_MASK_COUNT_OFFSET | (WP_TOWR << WPSHIFT))

/*
 * OMAP4 IP revision has different register offsets
 * for interrupt registers and functional registers.
 */
#define VERSION2_TIMER_WAKEUP_EN_REG_OFFSET     0x14
#define VERSION2_TIMER_STAT_REG_OFFSET          0x10

#define MAX_WRITE_PEND_WAIT		10000 /* 10ms timeout delay */

#define DT_LOG(fmt, args...) printf("dmtimer: " fmt, ## args)

static LIST_HEAD(omap_timer_list);

/**
 * omap_test_timeout - busy-loop, testing a condition
 * @cond: condition to test until it evaluates to true
 * @timeout: maximum number of microseconds in the timeout
 * @index: loop index (integer)
 *
 * Loop waiting for @cond to become true or until at least @timeout
 * microseconds have passed.  To use, define some integer @index in the
 * calling code.  After running, if @index == @timeout, then the loop has
 * timed out.
 */
#define omap_test_timeout(cond, timeout, index)			\
({								\
	for (index = 0; index < timeout; index++) {		\
		if (cond)					\
			break;					\
		udelay(1);					\
	}							\
})

/**
 * omap_dm_timer_read_reg - read timer registers in posted and non-posted mode
 * @timer:      timer pointer over which read operation to perform
 * @reg:        lowest byte holds the register offset
 *
 * The posted mode bit is encoded in reg. Note that in posted mode write
 * pending bit must be checked. Otherwise a read of a non completed write
 * will produce an error.
 */
static inline u32 omap_dm_timer_read_reg(struct omap_dm_timer *timer, u32 reg)
{
	int i = 0;

	if (reg >= OMAP_TIMER_WAKEUP_EN_REG)
		reg += timer->func_offset;
	else if (reg >= OMAP_TIMER_STAT_REG)
		reg += timer->intr_offset;

	if (timer->posted) {
		omap_test_timeout(!(__raw_readl(timer->io_base +
			((OMAP_TIMER_WRITE_PEND_REG +
			timer->func_offset) & 0xff)) & (reg >> WPSHIFT)),
			MAX_WRITE_PEND_WAIT, i);

		if (i == MAX_WRITE_PEND_WAIT)
			DT_LOG("read timeout.\n");
	}

	return __raw_readl(timer->io_base + (reg & 0xff));
}

/**
 * omap_dm_timer_write_reg - write timer registers in posted and non-posted mode
 * @timer:      timer pointer over which write operation is to perform
 * @reg:        lowest byte holds the register offset
 * @value:      data to write into the register
 *
 * The posted mode bit is encoded in reg. Note that in posted mode the write
 * pending bit must be checked. Otherwise a write on a register which has a
 * pending write will be lost.
 */
static void omap_dm_timer_write_reg(struct omap_dm_timer *timer, u32 reg,
						u32 value)
{
	int i = 0;

	if (reg >= OMAP_TIMER_WAKEUP_EN_REG)
		reg += timer->func_offset;
	else if (reg >= OMAP_TIMER_STAT_REG)
		reg += timer->intr_offset;

	if (timer->posted) {
		omap_test_timeout(!(__raw_readl(timer->io_base +
			((OMAP_TIMER_WRITE_PEND_REG +
			timer->func_offset) & 0xff)) & (reg >> WPSHIFT)),
			MAX_WRITE_PEND_WAIT, i);

		if (i == MAX_WRITE_PEND_WAIT)
			DT_LOG("write timeout.\n");
	}

	__raw_writel(value, timer->io_base + (reg & 0xff));
}

static void __timer_enable(struct omap_dm_timer *timer)
{
	if (!timer->enabled) {
		timer->enabled = 1;
	}
}

static void __timer_disable(struct omap_dm_timer *timer)
{
	if (timer->enabled) {
		timer->enabled = 0;
	}
}

static void omap_dm_timer_wait_for_reset(struct omap_dm_timer *timer)
{
	int c;

	c = 0;
	while (!(omap_dm_timer_read_reg(timer, OMAP_TIMER_SYS_STAT_REG) & 1)) {
		c++;
		if (c > 100000) {
			DT_LOG("Timer failed to reset\n");
			return;
		}
	}
}

static void omap_dm_timer_reset(struct omap_dm_timer *timer)
{
	u32 l;

	if (timer->id != 1) {
		omap_dm_timer_write_reg(timer, OMAP_TIMER_IF_CTRL_REG, 0x06);
		omap_dm_timer_wait_for_reset(timer);
	}

	l = omap_dm_timer_read_reg(timer, OMAP_TIMER_OCP_CFG_REG);
	l |= 0x02 << 3;  /* Set to smart-idle mode */
	l |= 0x2 << 8;   /* Set clock activity to perserve f-clock on idle */
	omap_dm_timer_write_reg(timer, OMAP_TIMER_OCP_CFG_REG, l);
}

static int omap_dm_timer_prepare(struct omap_dm_timer *timer)
{
	if (timer->needs_manual_reset)
		omap_dm_timer_reset(timer);

	omap_dm_timer_set_source(timer, OMAP_TIMER_SRC_32_KHZ);

	/* Match hardware reset default of posted mode */
	omap_dm_timer_write_reg(timer, OMAP_TIMER_IF_CTRL_REG,
			OMAP_TIMER_CTRL_POSTED);
	timer->posted = 1;

	return 0;
}

struct omap_dm_timer *omap_dm_timer_request_specific(int id, unsigned int fclk)
{
	struct omap_dm_timer *timer = NULL, *t;
	int ret;

	list_for_each_entry(t, &omap_timer_list, node) {
		if (t->id == id && !t->reserved) {
			timer = t;
			timer->reserved = 1;
			timer->enabled = 0;
			break;
		}
	}

	if (!timer) {
		DT_LOG("timer%d not available.\n", id);
		return NULL;
	}

	timer->fclk = fclk;

	ret = omap_dm_timer_prepare(timer);
	if (ret) {
		timer->reserved = 0;
		return NULL;
	}

	return timer;
}

int omap_dm_timer_free(struct omap_dm_timer *timer)
{
	if (!timer)
		return -EINVAL;

	if (!timer->reserved) {
		return -EINVAL;
	}

	__timer_disable(timer);
	timer->reserved = 0;

	return 0;
}

int omap_dm_timer_enable(struct omap_dm_timer *timer)
{
	if (!timer)
		return -EINVAL;

	__timer_enable(timer);

	return 0;
}

int omap_dm_timer_disable(struct omap_dm_timer *timer)
{
	if (!timer)
		return -EINVAL;

	__timer_disable(timer);

	return 0;
}

int omap_dm_timer_start(struct omap_dm_timer *timer)
{
	u32 l;

	if (!timer)
		return -EINVAL;

	__timer_enable(timer);
	l = omap_dm_timer_read_reg(timer, OMAP_TIMER_CTRL_REG);
	if (!(l & OMAP_TIMER_CTRL_ST)) {
		l |= OMAP_TIMER_CTRL_ST;
		omap_dm_timer_write_reg(timer, OMAP_TIMER_CTRL_REG, l);
	}

	return 0;
}

int omap_dm_timer_stop(struct omap_dm_timer *timer)
{
	u32 l;

	if (!timer)
		return -EINVAL;

	l = omap_dm_timer_read_reg(timer, OMAP_TIMER_CTRL_REG);
	if (l & OMAP_TIMER_CTRL_ST) {
		l &= ~0x1;
		omap_dm_timer_write_reg(timer, OMAP_TIMER_CTRL_REG, l);

		if (!timer->needs_manual_reset) {
			/* Readback to make sure write has completed */
			omap_dm_timer_read_reg(timer, OMAP_TIMER_CTRL_REG);
			/*
			* Wait for functional clock period x 3.5 to make
			* sure that timer is stopped
			*/
			udelay(3500000 / timer->fclk + 1);
		}
	}

	/* Ack possibly pending interrupt */
	omap_dm_timer_write_reg(timer, OMAP_TIMER_STAT_REG,
			OMAP_TIMER_INT_OVERFLOW);
	__timer_disable(timer);

	return 0;
}

int omap_dm_timer_set_source(struct omap_dm_timer *timer, int source)
{
	int ret;
	unsigned int data;

	if (!timer)
		return -EINVAL;

	if (source < 0 || source >= 3)
		return -EINVAL;

	__timer_disable(timer);

	//FIXME: Workaround to set clock
	/* change the timer clock source */
	data = __raw_readl(timer->fclk_reg);
	if (source == OMAP_TIMER_SRC_32_KHZ)
		data |= (1 << 24);
	else
		data &= ~(1 << 24);
	data |= (1 << 1); //enable clock
	__raw_writel(data, timer->fclk_reg);

	/*
	 * When the functional clock disappears, too quick writes seem
	 * to cause an abort. XXX Is this still necessary?
	 */
	udelay(300000);

	return ret;
}

int omap_dm_timer_set_load(struct omap_dm_timer *timer, int autoreload,
			    unsigned int load)
{
	u32 l;

	if (!timer)
		return -EINVAL;

	__timer_enable(timer);

	l = omap_dm_timer_read_reg(timer, OMAP_TIMER_CTRL_REG);
	if (autoreload)
		l |= OMAP_TIMER_CTRL_AR;
	else
		l &= ~OMAP_TIMER_CTRL_AR;
	omap_dm_timer_write_reg(timer, OMAP_TIMER_CTRL_REG, l);
	omap_dm_timer_write_reg(timer, OMAP_TIMER_LOAD_REG, load);
	omap_dm_timer_write_reg(timer, OMAP_TIMER_TRIGGER_REG, 0);
	__timer_disable(timer);

	return 0;
}

int omap_dm_timer_set_load_start(struct omap_dm_timer *timer, int autoreload,
                            unsigned int load)
{
	u32 l;

	if (!timer)
		return -EINVAL;

	__timer_enable(timer);

	l = omap_dm_timer_read_reg(timer, OMAP_TIMER_CTRL_REG);
	if (autoreload) {
		l |= OMAP_TIMER_CTRL_AR;
		omap_dm_timer_write_reg(timer, OMAP_TIMER_LOAD_REG, load);
	} else {
		l &= ~OMAP_TIMER_CTRL_AR;
	}
	l |= OMAP_TIMER_CTRL_ST;
	omap_dm_timer_write_reg(timer, OMAP_TIMER_COUNTER_REG, load);
	omap_dm_timer_write_reg(timer, OMAP_TIMER_CTRL_REG, l);

	return 0;
}

int omap_dm_timer_set_match(struct omap_dm_timer *timer, int enable,
			     unsigned int match)
{
	u32 l;

	if (!timer)
		return -EINVAL;

	__timer_enable(timer);
	l = omap_dm_timer_read_reg(timer, OMAP_TIMER_CTRL_REG);
	if (enable)
		l |= OMAP_TIMER_CTRL_CE;
	else
		l &= ~OMAP_TIMER_CTRL_CE;
	omap_dm_timer_write_reg(timer, OMAP_TIMER_CTRL_REG, l);
	omap_dm_timer_write_reg(timer, OMAP_TIMER_MATCH_REG, match);
	__timer_disable(timer);

	return 0;
}

int omap_dm_timer_set_pwm(struct omap_dm_timer *timer, int def_on,
			   int toggle, int trigger)
{
	u32 l;

	if (!timer)
		return -EINVAL;

	__timer_enable(timer);
	l = omap_dm_timer_read_reg(timer, OMAP_TIMER_CTRL_REG);
	l &= ~(OMAP_TIMER_CTRL_GPOCFG | OMAP_TIMER_CTRL_SCPWM |
	       OMAP_TIMER_CTRL_PT | (0x03 << 10));
	if (def_on)
		l |= OMAP_TIMER_CTRL_SCPWM;
	if (toggle)
		l |= OMAP_TIMER_CTRL_PT;
	l |= trigger << 10;
	omap_dm_timer_write_reg(timer, OMAP_TIMER_CTRL_REG, l);
	__timer_disable(timer);

	return 0;
}

int omap_dm_timer_set_prescaler(struct omap_dm_timer *timer, int prescaler)
{
	u32 l;

	if (!timer)
		return -EINVAL;

	__timer_enable(timer);
	l = omap_dm_timer_read_reg(timer, OMAP_TIMER_CTRL_REG);
	l &= ~(OMAP_TIMER_CTRL_PRE | (0x07 << 2));
	if (prescaler >= 0x00 && prescaler <= 0x07) {
		l |= OMAP_TIMER_CTRL_PRE;
		l |= prescaler << 2;
	}
	omap_dm_timer_write_reg(timer, OMAP_TIMER_CTRL_REG, l);
	__timer_disable(timer);

	return 0;
}

int omap_dm_timer_init(struct dmtimer_platform_data *pdata)
{
	int ret = 0;
	struct omap_dm_timer *timer;

	if (!pdata) {
		DT_LOG("No platform data.\n");
		return -ENODEV;
	}

	timer = malloc(sizeof(struct omap_dm_timer));
	if (!timer) {
		DT_LOG("No memory for omap_dm_timer.\n");
		ret = -ENOMEM;
		return ret;
	}

	timer->id = pdata->id;
	if (timer->id < OMAP_DMTIMER_1 || timer->id > OMAP_DMTIMER_11) {
		DT_LOG("Out of timer range\n");
		ret = -ENODEV;
		goto err_free_mem;
	}

	timer->io_base = pdata->io_base;
	if (!timer->io_base) {
		DT_LOG("iobase failed.\n");
		ret = -ENOMEM;
		goto err_free_mem;
	}

	timer->fclk_reg = pdata->fclk_reg;
	if (!timer->fclk_reg) {
		DT_LOG("fclk failed.\n");
		ret = -ENOMEM;
		goto err_free_mem;
	}

	if (pdata->timer_ip_type == OMAP_TIMER_IP_VERSION_2) {
		timer->func_offset = VERSION2_TIMER_WAKEUP_EN_REG_OFFSET;
		timer->intr_offset = VERSION2_TIMER_STAT_REG_OFFSET;
	} else {
		timer->func_offset = 0;
		timer->intr_offset = 0;
	}

	timer->needs_manual_reset = pdata->needs_manual_reset;
	/* add the timer element to the list */
	list_add_tail(&timer->node, &omap_timer_list);
	return 0;

err_free_mem:
	free(timer);
	return ret;
}

