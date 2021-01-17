/*
 * \brief  MMIO and IRQ definitions for the Raspberry Pi
 * \author Norman Feske
 * \date   2013-04-05
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__DEFS__RPI_H_
#define _INCLUDE__DRIVERS__DEFS__RPI_H_

/* Genode includes */
#include <util/mmio.h>

namespace Rpi {
	enum {
		RAM_0_BASE = 0x00000000,
		RAM_0_SIZE = 0x20000000, /* XXX ? */

		MMIO_0_BASE = 0xfe000000,
		MMIO_0_SIZE = 0x01800000,

		SYSTEM_TIMER_IRQ       = 97,
		SYSTEM_TIMER_MMIO_BASE = 0xfe003000,
		SYSTEM_TIMER_MMIO_SIZE = 0x1000,
		SYSTEM_TIMER_CLOCK     = 1000000,

		SP804_TIMER_IRQ       = 64,
		SP804_TIMER_MMIO_BASE = 0xfe00b000,
		SP804_TIMER_MMIO_SIZE = 0x1000,
		SP804_TIMER_CLOCK     = 1000000,
		
        PL011_0_IRQ       = 153,
		PL011_0_MMIO_BASE = 0xfe201000,
		PL011_0_MMIO_SIZE = 0x1000,
		PL011_0_CLOCK     = 3000000,

		PL011_1_IRQ       = 125,
		PL011_1_MMIO_BASE = 0xfe215000,
		PL011_1_MMIO_SIZE = 0x1000,
		PL011_1_CLOCK     = 3000000,

		IRQ_CONTROLLER_BASE = 0x40041000,
		IRQ_CONTROLLER_SIZE = 0x7000,

		GPIO_CONTROLLER_BASE = 0xfe200000,
		GPIO_CONTROLLER_SIZE = 0x1000,

		USB_DWC_OTG_BASE = 0xfe980000,
		USB_DWC_OTG_SIZE = 0x10000,

		/* USB host controller */
		DWC_IRQ = 105,

		/* SD card */
		SDHCI_BASE = MMIO_0_BASE + 0x300000,
		SDHCI_SIZE = 0x100,
		SDHCI_IRQ  = 158,
	};

	enum Videocore_cache_policy { NON_COHERENT = 0,
	                              COHERENT     = 1,
	                              L2_ONLY      = 2,
	                              UNCACHED     = 3 };
};

#endif /* _INCLUDE__DRIVERS__DEFS__RPI_H_ */
