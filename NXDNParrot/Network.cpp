/*
 *   Copyright (C) 2009-2014,2016,2018 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Network.h"

#include <cstdio>
#include <cassert>
#include <cstring>

CNetwork::CNetwork(unsigned int port) :
m_socket(port),
m_address(),
m_port(0U)
{
}

CNetwork::~CNetwork()
{
}

bool CNetwork::open()
{
	::fprintf(stdout, "Opening NXDN network connection\n");

	return m_socket.open();
}

bool CNetwork::write(const unsigned char* data, unsigned int length)
{
	if (m_port == 0U)
		return true;

	assert(data != NULL);

	return m_socket.write(data, length, m_address, m_port);
}

unsigned int CNetwork::read(unsigned char* data, unsigned int len)
{
	in_addr address;
	unsigned int port;
	int length = m_socket.read(data, len, address, port);
	if (length <= 0)
		return 0U;

	m_address.s_addr = address.s_addr;
	m_port = port;

	if (::memcmp(data, "NXDNP", 5U) == 0 && length == 15) {			// A poll
		write(data, length);
		return 0U;
	} else if (::memcmp(data, "NXDND", 5U) == 0 && length == 43) {
		return 43U;
	} else {
		return 0U;
	}
}

void CNetwork::end()
{
	m_port = 0U;
}

void CNetwork::close()
{
	m_socket.close();

	::fprintf(stdout, "Closing NXDN network connection\n");
}
