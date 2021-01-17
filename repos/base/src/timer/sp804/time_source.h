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

		struct LOAD : Register<0x400, 32>{};
		struct VALUE : Register<0x404, 32>{};
		struct CNTL : Register<0x408, 32>{
			struct MODE      : Bitfield<1, 1>  { };
			struct DIV       : Bitfield<2, 2>  { };
			struct IE        : Bitfield<5, 1>  { };
			struct ENABLE    : Bitfield<7, 1>  { };
			struct DBGHALT   : Bitfield<8, 1>  { };
			struct ENAFREE   : Bitfield<9, 1>  { };
			struct FREEDIV   : Bitfield<16, 8>  { };
		};
		struct IRQCNTL : Register<0x40c, 32>{
			struct INT       : Bitfield<0, 1>  { };
		};
		struct RAWIRQ : Register<0x410, 32>{
			struct INT       : Bitfield<0, 1>  { };
		};
		struct MSKIRQ : Register<0x414, 32>{
			struct INT       : Bitfield<0, 1>  { };
		};
		struct RELOAD : Register<0x418, 32>{};
		struct PREDIV : Register<0x41c, 32>{
			struct DIV       : Bitfield<0, 9>  { };
		};
		struct FREECNT : Register<0x420, 32>{};

		Genode::Irq_connection     _timer_irq;
		Genode::Duration           _curr_time   { Genode::Microseconds(0) };
		unsigned long              _cleared_ticks { 0 };
		unsigned long 			   _curr_timeout { 0 };

		void _initialize();

	public:

		Time_source(Genode::Env &env);


		/*************************
		 ** Genode::Time_source **
		 *************************/

		Genode::Duration curr_time() override;
		void set_timeout(Genode::Microseconds duration,
		                 Genode::Timeout_handler &handler) override;
		Genode::Microseconds max_timeout() const override;
};

#endif /* _TIME_SOURCE_H_ */
