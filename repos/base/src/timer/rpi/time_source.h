/*
 * \brief  Time source that uses the Enhanced Periodic Interrupt Timer (Freescale)
 * \author Norman Feske
 * \author Martin Stein
 * \author Stefan Kalkowski
 * \date   2009-06-16
 */

/*
 * Copyright (C) 2009-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _TIME_SOURCE_H_
#define _TIME_SOURCE_H_

/* Genode includes */
#include <irq_session/connection.h>
#include <os/attached_mmio.h>
#include <drivers/timer/util.h>

/* local includes */
#include <signalled_time_source.h>

namespace Timer { class Time_source; }


class Timer::Time_source : private Genode::Attached_mmio,
                           public Genode::Signalled_time_source
{
	private:

		enum { TICKS_PER_MS = 1000 };

		struct Cs : Register<0x00, 32>{
			struct M0      : Bitfield<0, 1>  { };
			struct M1      : Bitfield<1, 1>  { };
			struct M2      : Bitfield<2, 1>  { };
			struct M3      : Bitfield<3, 1>  { };
		};
		struct Clo : Register<0x04, 32>{};
		struct Chi : Register<0x08, 32>{};
		struct C0 : Register<0x0c, 32>{};
		struct C1 : Register<0x10, 32>{};
		struct C2 : Register<0x14, 32>{};
		struct C3 : Register<0x18, 32>{};

		Genode::Irq_connection     _timer_irq;
		Genode::Duration           _curr_time     { Genode::Microseconds(0) };
		Clo::access_t              _last_cnt      { 0 };
		Genode::Microseconds const _max_timeout   { Genode::timer_ticks_to_us(0xffffffff, TICKS_PER_MS) };

	public:

		Time_source(Genode::Env &env);


		/*************************
		 ** Genode::Time_source **
		 *************************/

		Genode::Duration curr_time() override;
		void set_timeout(Genode::Microseconds, Genode::Timeout_handler &) override;
		Genode::Microseconds max_timeout() const override { return _max_timeout; };
};

#endif /* _TIME_SOURCE_H_ */
