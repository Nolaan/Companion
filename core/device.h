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

#if !defined(DEVICE__H)

#define DEVICE__H

#include <QString>
#include <QObject>
#include <qglobal.h>

class PartitionTable;
class CreatePartitionTableOperation;
class LibParted;

/** @brief A device.

	Represents a device like /dev/sda.

	Devices are the outermost entity; they contain a PartitionTable that itself contains Partitions.

	@see PartitionTable, Partition, LibParted::scanDevices()
	@author vl@fidra.de
*/
class Device : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(Device)

	friend class CreatePartitionTableOperation;
	friend class LibParted;

	public:
		Device(const QString& name, const QString& devicenode, qint32 heads, qint32 numSectors, qint32 cylinders, qint64 sectorSize);
		~Device();

	public:
		const QString& name() const { return m_Name; } /**< @return the Device's name, usually some manufacturer string */
		const QString& deviceNode() const { return m_DeviceNode; } /**< @return the Device's node, for example "/dev/sda" */
		PartitionTable* partitionTable() { return m_PartitionTable; } /**< @return the Device's PartitionTable */
		const PartitionTable* partitionTable() const { return m_PartitionTable; } /**< @return the Device's PartitionTable */
		qint32 heads() const { return m_Heads; } /**< @return the number of heads on the Device in CHS notation */
		qint32 cylinders() const { return m_Cylinders; } /**< @return the number of cylinders on the Device in CHS notation */
		qint32 sectorsPerTrack() const { return m_SectorsPerTrack; } /**< @return the number of sectors on the Device in CHS notation */
		qint32 sectorSize() const { return m_SectorSize; } /**< @return the sector size the Device claims to use */
		qint64 totalSectors() const { return static_cast<qint64>(heads()) * cylinders() * sectorsPerTrack(); } /**< @return the total number of sectors on the device */
		qint64 capacity() const { return totalSectors() * sectorSize(); } /**< @return the Device's capacity in bytes */
		qint64 cylinderSize() const { return static_cast<qint64>(heads()) * sectorsPerTrack(); } /**< @return the size of a cylinder on this Device in sectors */
        QString m_DeviceNode;

	protected:
		void setPartitionTable(PartitionTable* ptable) { m_PartitionTable = ptable; }

	private:
		QString m_Name;

		PartitionTable* m_PartitionTable;
		qint32 m_Heads;
		qint32 m_SectorsPerTrack;
		qint32 m_Cylinders;
		qint32 m_SectorSize;
};

#endif

