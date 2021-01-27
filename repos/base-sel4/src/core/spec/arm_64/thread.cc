/*
 * \brief  Utilities for thread creation on seL4
 * \author Norman Feske
 * \date   2015-05-12
 *
 * This file is used by both the core-specific implementation of the Thread API
 * and the platform-thread implementation for managing threads outside of core.
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* base includes */
#include <base/thread_state.h>

/* core includes */
#include <thread_sel4.h>
#include <platform_thread.h>

void Genode::start_sel4_thread(Cap_sel tcb_sel, addr_t ip, addr_t sp,
                               unsigned cpu)
{
	/* set register values for the instruction pointer and stack pointer */
	seL4_UserContext regs;
	Genode::memset(&regs, 0, sizeof(regs));
	size_t const num_regs = sizeof(regs)/sizeof(seL4_Word);

	regs.pc = ip;
	regs.sp = sp;

	long const ret = seL4_TCB_WriteRegisters(tcb_sel.value(), false, 0,
	                                         num_regs, &regs);
	ASSERT(ret == 0);

	affinity_sel4_thread(tcb_sel, cpu);

	seL4_TCB_Resume(tcb_sel.value());
}

void Genode::affinity_sel4_thread(Cap_sel const &, unsigned cpu)
{
	if (cpu != 0)
		error("could not set affinity of thread");
}

Genode::Thread_state Genode::Platform_thread::state()
{
	seL4_TCB   const thread         = _info.tcb_sel.value();
	seL4_Bool  const suspend_source = false;
	seL4_Uint8 const arch_flags     = 0;
	seL4_UserContext registers;
	seL4_Word  const register_count = sizeof(registers) / sizeof(registers.pc);

	long const ret = seL4_TCB_ReadRegisters(thread, suspend_source, arch_flags,
	                                        register_count, &registers);
	if (ret != seL4_NoError) {
		error("reading thread state ", ret);
		throw Cpu_thread::State_access_failed();
	}

	Thread_state state;
	Genode::memset(&state, 0, sizeof(state));

	state.r[0]   = registers.x0;
	state.r[1]   = registers.x1;
	state.r[2]   = registers.x2;
	state.r[3]   = registers.x3;
	state.r[4]   = registers.x4;
	state.r[5]   = registers.x5;
	state.r[6]   = registers.x6;
	state.r[7]   = registers.x7;
	state.r[8]   = registers.x8;
	state.r[9]   = registers.x9;
	state.r[10]  = registers.x10;
	state.r[11]  = registers.x11;
	state.r[12]  = registers.x12;
	state.r[13]  = registers.x13;
	state.r[14]  = registers.x14;
	state.r[15]  = registers.x15;
	state.r[16]  = registers.x16;
	state.r[17]  = registers.x17;
	state.r[18]  = registers.x18;
	state.r[19]  = registers.x19;
	state.r[20]  = registers.x20;
	state.r[21]  = registers.x21;
	state.r[22]  = registers.x22;
	state.r[23]  = registers.x23;
	state.r[24]  = registers.x24;
	state.r[25]  = registers.x25;
	state.r[26]  = registers.x26;
	state.r[27]  = registers.x27;
	state.r[28]  = registers.x28;
	state.r[29]  = registers.x29;
	state.r[30]  = registers.x30;
	state.sp   = registers.sp;
	state.ip   = registers.pc;

	return state;
}
