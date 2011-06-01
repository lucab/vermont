/*
 * Vermont PCAP Exporter
 * Copyright (C) 2009 Vermont Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef IDS_LOADBALANCER_HPP
#define IDS_LOADBALANCER_HPP

#include "core/Module.h"
#include "idsloadbalancer/HashPacketSelector.hpp"
#include "idsloadbalancer/IpPacketSelector.hpp"
#include "modules/ipfix/IpfixRecord.hpp"
#include "PCAPExporterMem.h"

#include <common/msg.h>

#include <string>
#include <ctime>
#include <common/SignalInterface.h>


class Packet;


class IDSLoadbalancer : public Module, public Destination<Packet *>, public Source<Packet *>, ExporterNotificationHandler
{
public:
	IDSLoadbalancer(BasePacketSelector* _selector, uint64_t updateinterval);
	~IDSLoadbalancer();
	virtual void receive(Packet* packet);
	void performStart();
	void performShutdown();
	virtual void queueUtilization(uint32_t maxsize, uint32_t cursize, Packet* packet);

private:
	BasePacketSelector *selector;
	PriorityPacketSelector* ppselector;
	uint32_t qcount;
	std::map<uint32_t, int> src;
	std::map<uint32_t, int> dst;
	Thread thread;
	bool shutdownThread;
	uint64_t updateInterval; /**< update interval in milliseconds of internal IP address list (if needed) */
	int curPacketQueueID;
	Destination<IpfixRecord*>* ipfixModule;

	static void* threadWrapper(void* data);
	void updateWorker();
	void updateBalancingLists(struct timeval curtime);
};

#endif