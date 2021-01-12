/*
 * \brief  Time source for i.MX7 (GPT1)
 * \author Stefan Kalkowski
 * \date   2019-04-13
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <drivers/defs/rpi4.h>
/* local include */
#include <time_source.h>

using namespace Genode;

enum {
	MMIO_BASE = Rpi4::SYSTEM_TIMER_MMIO_BASE,
	MMIO_SIZE = Rpi4::SYSTEM_TIMER_MMIO_SIZE,
	IRQ       = Rpi4::SYSTEM_TIMER_IRQ,
};


Timer::Time_source::Time_source(Env &env)
: Attached_mmio(env, MMIO_BASE, MMIO_SIZE),
  Signalled_time_source(env),
  _timer_irq(env, IRQ) { _initialize(); }
