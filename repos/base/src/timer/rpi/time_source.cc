/*
 * \brief  Time source that uses the Enhanced Periodic Interrupt Timer (Freescale)
 * \author Norman Feske
 * \author Martin Stein
 * \author Stefan Kalkowski
 * \author Alexander Boettcher
 * \date   2009-06-16
 */

/*
 * Copyright (C) 2009-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* local includes */
#include <time_source.h>

using namespace Genode;


void Timer::Time_source::set_timeout(Genode::Microseconds     duration,
                                     Genode::Timeout_handler &handler)
{
	_handler = &handler;
	/* set to minimum ticks value to not miss a too short timeout */
	Genode::uint32_t const ticks =
		Genode::max(1UL, (duration.value * TICKS_PER_MS) / 1000);

	/* clear interrupts */
	if (read<Cs>()) {
		write<Cs::M1>(1);
		_timer_irq.ack_irq();
	}

	write<C1>(read<Clo>() + ticks);
}


Duration Timer::Time_source::curr_time()
{
	Clo::access_t cur_cnt = read<Clo>();
	Genode::Microseconds us(timer_ticks_to_us(cur_cnt - _last_cnt,
	                                          TICKS_PER_MS));
	_last_cnt = cur_cnt;
	_curr_time.add(us);
	return _curr_time;
}
