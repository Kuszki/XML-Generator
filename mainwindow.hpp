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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets>
#include <QtCore>

#include "generatorworker.hpp"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{

		Q_OBJECT

	private:

		QThread WorkerThread;

		Ui::MainWindow* ui;
		GeneratorWorker* worker;

		QProgressBar* progressBar;
		QPushButton* generateButton;

	public:

		explicit MainWindow(QWidget* parent = nullptr);
		virtual ~MainWindow(void) override;

	private slots:

		void srcButtonClicked(void);
		void destButtonClicked(void);

		void generateButtonClicked(void);

		void validateDialogParams(void);

		void processStarted(void);
		void processEnded(void);

	signals:

		void onGenerateRequest(const QString&, const QString&,
						   const QString&, const QString&,
						   const QString&, int);

};

#endif // MAINWINDOW_HPP
