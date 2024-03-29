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

#ifndef GENERATORWORKER_HPP
#define GENERATORWORKER_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

class GeneratorWorker : public QObject
{

		Q_OBJECT

	public:

		explicit GeneratorWorker(QObject *parent = nullptr);
		virtual ~GeneratorWorker(void) override;

	public slots:

		void start(const QString& Src, const QString& Dest,
				 const QString& Root, const QString& Data,
				 const QString& Codec, int Index);

	signals:

		void onProgressStart(void);
		void onProgressSetup(int, int);
		void onProgressChange(int);
		void onProgressEnd(void);

};

#endif // GENERATORWORKER_HPP
