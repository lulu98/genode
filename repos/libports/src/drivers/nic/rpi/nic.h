/*
 * \brief  LanRPi4 NIC driver
 * \author Norman Feske
 * \author Stefan Kalkowski
 * \date   2011-05-21
 */

/*
 * Copyright (C) 2011-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__NIC__SPEC__LanRPi4__LanRPi4_H_
#define _DRIVERS__NIC__SPEC__LanRPi4__LanRPi4_H_

#include <base/attached_dataspace.h>
#include <base/log.h>
#include <util/misc_math.h>
#include <irq_session/client.h>
#include <timer_session/connection.h>
#include <nic/component.h>

#include <base/allocator.h>
#include <base/ram_allocator.h>

// Circle library files
#include <circle/bcm54213.h>
// CBcm54213Device		m_Bcm54213;
// CMACAddress 		m_MACAddress;

class LanRPi4 : public Nic::Session_component
{
	private:

		/*
		 * Noncopyable
		 */
		LanRPi4(LanRPi4 const &);
		LanRPi4 &operator = (LanRPi4 const &);

		Genode::Attached_dataspace        _mmio;
		volatile Genode::uint32_t        *_reg_base;
		Timer::Connection                 _timer;
		Nic::Mac_address                  _mac_addr { };
		Genode::Irq_session_client        _irq;
		Genode::Signal_handler<LanRPi4>   _irq_handler;
		Genode::Allocator				 &_allocator;

		CBcm54213Device		m_Bcm54213;
		// CInterruptSystem	m_Interrupt;
		// CTimer				m_Timer;
		CMACAddress 		m_MACAddress;

	protected:

		bool _send()
		{
			if (!_tx.sink()->ready_to_ack())
				return false;

			if (!_tx.sink()->packet_avail())
				return false;

			Genode::Packet_descriptor packet = _tx.sink()->get_packet();
			if (!packet.size() || !_tx.sink()->packet_valid(packet)) {
				Genode::warning("Invalid tx packet");
				return true;
			}

			Genode::size_t const max_size = FRAME_BUFFER_SIZE;
			if (packet.size() > max_size) {
				Genode::error("packet size ", packet.size(), " too large, "
				              "limit is ", max_size);
				return true;
			}

			unsigned nLength = packet.size();
			const Genode::uint8_t *src = (Genode::uint8_t *)_tx.sink()->packet_content(packet);
			if(!m_Bcm54213.SendFrame(src,nLength))
			{
				return false;
			}

			_tx.sink()->acknowledge_packet(packet);
			return true;
		}

		void _handle_packet_stream() override
		{
			while (_rx.source()->ack_avail())
				_rx.source()->release_packet(_rx.source()->get_acked_packet());

			while (_send()) ;
		}

		void _handle_irq()
		{
			using namespace Genode;

			_handle_packet_stream();

			void * FrameBuffer = _allocator.alloc(FRAME_BUFFER_SIZE);
			// DMA_BUFFER (u8, FrameBuffer, FRAME_BUFFER_SIZE);
			unsigned nFrameLength;

			while (_rx.source()->ready_to_submit()) {

				if(m_Bcm54213.ReceiveFrame(FrameBuffer,&nFrameLength))
				{
					/* allocate rx packet buffer */
					Nic::Packet_descriptor p;
					try {
						p = _rx.source()->alloc_packet(nFrameLength);
					} catch (Session::Rx::Source::Packet_alloc_failed) { return; }

					uint8_t *dst = (uint8_t *)_rx.source()->packet_content(p);
					memcpy(dst,FrameBuffer,nFrameLength);

					_rx.source()->submit_packet(p);
				}

			}
			_allocator.free(FrameBuffer,FRAME_BUFFER_SIZE);

			_irq.ack_irq();
		}

	public:

		/**
		 * Exception type
		 */
		class Device_not_supported { };

		/**
		 * Constructor
		 *
		 * \throw  Device_not_supported
		 */
		LanRPi4(Genode::Io_mem_dataspace_capability ds_cap,
		        Genode::Irq_session_capability      irq_cap,
		        Genode::size_t const                tx_buf_size,
		        Genode::size_t const                rx_buf_size,
		        Genode::Allocator                 & rx_block_md_alloc,
		        Genode::Env                       & env)
		: Session_component(tx_buf_size, rx_buf_size, Genode::CACHED,
		                    rx_block_md_alloc, env),
		  _mmio(env.rm(), ds_cap),
		  _reg_base(_mmio.local_addr<Genode::uint32_t>()),
		  _timer(env),
		  _irq(irq_cap),
		  _irq_handler(env.ep(), *this, &LanRPi4::_handle_irq),
		  _allocator(rx_block_md_alloc),
		  m_Bcm54213(CBcm54213Device(env,rx_block_md_alloc)),
		  m_MACAddress(CMACAddress())
		//   m_Bcm54213 (CBcm54213Device()),
		//   m_Interrupt (CInterruptSystem()),
		//   m_Timer (&m_Interrupt),
		{
			_irq.sigh(_irq_handler);
			_irq.ack_irq();

			boolean bOK = TRUE;

			// if (bOK)
			// {
			// 	bOK = m_Interrupt.Initialize ();
			// }

			// if (bOK)
			// {
			// 	bOK = m_Timer.Initialize ();
			// }

			if (bOK)
			{
				bOK = m_Bcm54213.Initialize ();
			}

			if (!bOK)
			{
				Genode::error("Device initialization failed.");
				throw Device_not_supported();
			}

			/* print MAC address */
			m_Bcm54213.GetMACAddress()->CopyTo((u8*) &(_mac_addr.addr));
			Genode::log("MAC address: ", _mac_addr);
		}

		/**
		 * Destructor
		 */
		~LanRPi4()
		{
			Genode::log("disable NIC");
			m_Bcm54213.~CBcm54213Device();
		}

		/**************************************
		 ** Nic::Session_component interface **
		 **************************************/

		Nic::Mac_address mac_address() override
		{
			m_Bcm54213.GetMACAddress()->CopyTo((u8*) &(_mac_addr.addr));
			return _mac_addr;
		}

		bool link_state() override
		{
			return m_Bcm54213.IsLinkUp();
		}
};

#endif /* _DRIVERS__NIC__SPEC__LanRPi4__LanRPi4_H_ */
