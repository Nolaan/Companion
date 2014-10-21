/***************************************************************************
 *   Copyright (C) 2008 by Volker Lanz <vl@fidra.de>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#include "core/copysourcedevice.h"
#include "core/copytarget.h"
#include "core/copytargetdevice.h"
#include "core/device.h"

#include <parted/parted.h>

#include <kdebug.h>

/** Constructs a CopySource on the given Device
	@param d Device from which to copy
	@param firstsector First sector that will be copied
	@param lastsector Last sector that will be copied
*/
CopySourceDevice::CopySourceDevice(Device& d, qint64 firstsector, qint64 lastsector) :
	CopySource(),
	m_Device(d),
	m_FirstSector(firstsector),
	m_LastSector(lastsector),
	m_PedDevice(NULL)
{
}

/** Destructs a CopySourceDevice */
CopySourceDevice::~CopySourceDevice()
{
	ped_device_close(m_PedDevice);
}

/** Opens the Device
	@return true if the Device could be successfully opened
*/
bool CopySourceDevice::open()
{
	m_PedDevice = ped_device_get(device().deviceNode().toAscii());
	return m_PedDevice != NULL && ped_device_open(m_PedDevice);
}

/** Returns the Device's sector size
	@return the sector size
*/
qint32 CopySourceDevice::sectorSize() const
{
	return device().sectorSize();
}

/** Returns the length of this CopySource
	@return length of the copy source
*/
qint64 CopySourceDevice::length() const
{
	return lastSector() - firstSector() + 1;
}

/** Reads a given number of sectors from the Device into the given buffer.

	Note that @p readOffset must be greater or equal than zero.

	@param buffer the buffer to store the read sectors in
	@param readOffset the offset to begin reading
	@param numSectors the number of sector to read

	@return true if successful
*/
bool CopySourceDevice::readSectors(void* buffer, qint64 readOffset, qint64 numSectors)
{
	Q_ASSERT(readOffset >= 0);
	return ped_device_read(m_PedDevice, buffer, readOffset, numSectors);
}

/** Checks if this CopySourceDevice overlaps with the given CopyTarget
	@param target the CopyTarget to check overlapping with
	@return true if overlaps
*/
bool CopySourceDevice::overlaps(const CopyTarget& target) const
{
	try
	{
		const CopyTargetDevice& t = dynamic_cast<const CopyTargetDevice&>(target);
		
		if (device().deviceNode() != t.device().deviceNode())
			return false;

		// overlapping at the front?
		if (firstSector() <= t.firstSector() && lastSector() >= t.firstSector())
			return true;

		// overlapping at the back?
		if (firstSector() <= t.lastSector() && lastSector() >= t.lastSector())
			return true;
	}
	catch (...)
	{
	}

	return false;
}
