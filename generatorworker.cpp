/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  {description}                                                          *
 *  Copyright (C) 2018  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "generatorworker.hpp"

GeneratorWorker::GeneratorWorker(QObject *parent) : QObject(parent) {}

GeneratorWorker::~GeneratorWorker(void) {}

void GeneratorWorker::start(const QString& Src, const QString& Dest, const QString& Root, const QString& Data, int Index)
{
	QFile File(Src); QTextStream Stream(&File); QList<QStringList> Lines; int P(0);
	const QString Rooth = Data.isEmpty() ? Root : Root + ' ' + Data;

	emit onProgressStart();
	emit onProgressSetup(0, 0);

	File.open(QFile::ReadOnly | QFile::Text);
	Stream.setLocale(QLocale());

	const QStringList Headers = Stream.readLine().trimmed().split('\t');
	const int Count = Headers.size();

	while (!Stream.atEnd()) Lines.append(Stream.readLine().split('\t'));

	emit onProgressSetup(0, Lines.size());

	for (const auto& Data : Lines)
	{
		QFileInfo Info(Dest + '/' + Data.value(Index));
		QDir().mkpath(Info.absolutePath());
		QFile Outfile(Info.absoluteFilePath());
		QTextStream Outstream(&Outfile);

		Outfile.open(QFile::WriteOnly | QFile::Text);
		Outstream.setCodec("UTF-8");

		Outstream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << NEWLINE;
		Outstream << "<" << Rooth << ">" << NEWLINE;

		for (int i = 0; i < Count; ++i) if (i != Index)
		{
			Outstream << "\t<" << Headers.value(i) << ">"
					<< Data.value(i)
					<< "</" << Headers.value(i) << ">"
					<< NEWLINE;
		}

		Outstream << "</" << Root << ">" << NEWLINE;

		emit onProgressChange(++P);
	}

	emit onProgressEnd();
}
