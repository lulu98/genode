/*
 * \brief  Time source that uses the General Purpose Timer (Freescale)
 * \author Stefan Kalkowski
 * \date   2019-04-13
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* local includes */
#include <time_source.h>

using namespace Genode;


void Timer::Time_source::set_timeout(Genode::Microseconds  duration,
                                     Timeout_handler      &handler)
{
	Genode::log("set_timeout");
	unsigned long const ticks = (1ULL * duration.value * TICKS_PER_MS) / 1000;	
	_handler = &handler;

	_timer_irq.ack_irq();
	_cleared_ticks = 0;

	write<CNTL::IE>(0);
	write<CNTL::ENABLE>(0);

	write<IRQCNTL::INT>(1);
	write<LOAD>(ticks);
	_curr_timeout = ticks;
	write<RELOAD>(ticks);

	write<CNTL::ENABLE>(1);
	write<CNTL::IE>(1);
	Genode::log(ticks);
	Genode::log(read<VALUE>());
	// /* clear interrupts */
	// if (read<MSKIRQ::INT>()) {
	// 	write<LOAD>(0xffffffff);
	// 	write<RELOAD>(0xffffffff);
	// 	write<IRQCNTL::INT>(1);
	// 	_timer_irq.ack_irq();
	// }

	// /* set new timeout */
	// write<LOAD>(read<VALUE>() - ticks);
}


Duration Timer::Time_source::curr_time()
{
	unsigned long const uncleared_ticks = _curr_timeout - read<VALUE>() - _cleared_ticks;
	unsigned long const uncleared_us    = timer_ticks_to_us(uncleared_ticks, TICKS_PER_MS);

	/* update time only on IRQs and if rate is under 1000 per second */
	if (_irq || uncleared_us > 1000) {
		_curr_time.add(Genode::Microseconds(uncleared_us));
		_cleared_ticks += uncleared_ticks;
	}
	return _curr_time;
}


Genode::Microseconds Timer::Time_source::max_timeout() const
{
	static unsigned long max = timer_ticks_to_us(0xffffffff, TICKS_PER_MS);
	return Genode::Microseconds(max);
}


void Timer::Time_source::_initialize()
{
	_timer_irq.sigh(_signal_handler);

	// write<CNTL>(0);
	// write<IRQCNTL::INT>(0);
	// write<RAWIRQ::INT>(0);
	// write<MSKIRQ::INT>(0);
	write<PREDIV>(0x07d);

	write<CNTL::MODE>(1);
	// write<LOAD>(0xffffffff);
	// write<RELOAD>(0xffffffff);
	write<CNTL::DIV>(0);
	// write<CNTL::ENABLE>(1);
	// write<CNTL::IE>(1);
}
