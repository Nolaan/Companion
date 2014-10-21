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

#include "fs/reiser4.h"

#include "util/externalcommand.h"

#include <QStringList>
#include <QRegExp>

namespace FS
{
	FileSystem::SupportType reiser4::m_GetUsed = FileSystem::SupportNone;
	FileSystem::SupportType reiser4::m_GetLabel = FileSystem::SupportNone;
	FileSystem::SupportType reiser4::m_Create = FileSystem::SupportNone;
	FileSystem::SupportType reiser4::m_Move = FileSystem::SupportNone;
	FileSystem::SupportType reiser4::m_Check = FileSystem::SupportNone;
	FileSystem::SupportType reiser4::m_Copy = FileSystem::SupportNone;
	FileSystem::SupportType reiser4::m_Backup = FileSystem::SupportNone;

	reiser4::reiser4(qint64 firstsector, qint64 lastsector, qint64 sectorsused, const QString& label) :
		FileSystem(firstsector, lastsector, sectorsused, label, FileSystem::Reiser4)
	{
	}

	void reiser4::init()
	{
		m_GetUsed = m_GetLabel = findExternal("debugfs.reiser4", QStringList(), 16) ? SupportExternal : SupportNone;
		m_Create = findExternal("mkfs.reiser4", QStringList(), 16) ? SupportExternal : SupportNone;
		m_Check = findExternal("fsck.reiser4", QStringList(), 16) ? SupportExternal : SupportNone;
		m_Move = m_Copy = (m_Check != SupportNone) ? SupportInternal : SupportNone;
		m_Backup = SupportInternal;
	}

	qint64 reiser4::readUsedCapacity(const QString& deviceNode) const
	{
		ExternalCommand cmd("debugfs.reiser4", QStringList() << deviceNode);

		if (cmd.run())
		{
			qint64 blocks = -1;
			QRegExp rxBlocks("blocks:\\s+(\\d+)");

			if (rxBlocks.indexIn(cmd.output()) != -1)
				blocks = rxBlocks.cap(1).toLongLong();
			
			qint64 blockSize = -1;
			QRegExp rxBlockSize("blksize:\\s+(\\d+)");

			if (rxBlockSize.indexIn(cmd.output()) != -1)
				blockSize = rxBlockSize.cap(1).toLongLong();

			qint64 freeBlocks = -1;
			QRegExp rxFreeBlocks("free blocks:\\s+(\\d+)");

			if (rxFreeBlocks.indexIn(cmd.output()) != -1)
				freeBlocks = rxFreeBlocks.cap(1).toLongLong();

			if (blocks > - 1 && blockSize > -1 && freeBlocks > -1)
				return (blocks - freeBlocks) * blockSize;
		}

		return -1;
	}

	QString reiser4::readLabel(const QString& deviceNode) const
	{
		ExternalCommand cmd("debugfs.reiser4", QStringList() << deviceNode);

		if (cmd.run())
		{
			QRegExp rxLabel("label:\\s+(<?\\w+>?)");

			if (rxLabel.indexIn(cmd.output()) != -1 && rxLabel.cap(1) != "<none>")
				return rxLabel.cap(1);
		}
		
		return QString();
	}

	bool reiser4::check(Report& report, const QString& deviceNode) const
	{
		ExternalCommand cmd(report, "fsck.reiser4", QStringList() << "--fix" << "-y" << deviceNode);
		return cmd.run(-1) && cmd.exitCode() == 0;
	}

	bool reiser4::create(Report& report, const QString& deviceNode) const
	{
		return ExternalCommand(report, "mkfs.reiser4", QStringList() << "--yes" << deviceNode).run(-1);
	}
}
