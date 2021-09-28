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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	worker(new GeneratorWorker)
{
	ui->setupUi(this);

	ui->progressBar->setVisible(false);
	ui->generateButton->setEnabled(false);

	worker->moveToThread(&WorkerThread);
	WorkerThread.start();

	for (const auto& c : QTextCodec::availableCodecs())
		ui->encCombo->addItem(QString::fromLocal8Bit(c));

	ui->encCombo->model()->sort(0);

	connect(worker, &GeneratorWorker::onProgressStart, this, &MainWindow::processStarted);
	connect(worker, &GeneratorWorker::onProgressEnd, this, &MainWindow::processEnded);

	connect(worker, &GeneratorWorker::onProgressSetup, ui->progressBar, &QProgressBar::setRange);
	connect(worker, &GeneratorWorker::onProgressChange, ui->progressBar, &QProgressBar::setValue);

	connect(this, &MainWindow::onGenerateRequest, worker, &GeneratorWorker::start);
}

MainWindow::~MainWindow(void)
{
	WorkerThread.exit();
	WorkerThread.wait();

	delete worker;
	delete ui;
}

void MainWindow::srcButtonClicked(void)
{
	const QString Path = QFileDialog::getOpenFileName(this, tr("Open data file"), QString(),
											tr("Text files (*.txt);;CSV files (*.csv)"));

	if (!Path.isEmpty())
	{
		QFile File(Path);
		File.open(QFile::ReadOnly | QFile::Text);

		const QStringList List = QString::fromLocal8Bit(File.readLine().trimmed()).split('\t');

		ui->indexCombo->clear();
		ui->indexCombo->addItems(List);
		ui->srcEdit->setText(Path);
	}
}

void MainWindow::destButtonClicked(void)
{
	const QString Path = QFileDialog::getExistingDirectory(this, tr("Select destonation path"), QString());

	if (!Path.isEmpty()) ui->destEdit->setText(Path);
}

void MainWindow::generateButtonClicked(void)
{
	ui->generateButton->setEnabled(false);

	emit onGenerateRequest(ui->srcEdit->text(),
					   ui->destEdit->text(),
					   ui->rootEdit->text(),
					   ui->dataEdit->document()->toPlainText(),
					   ui->encCombo->currentText(),
					   ui->indexCombo->currentIndex());
}

void MainWindow::validateDialogParams(void)
{
	const bool OK = !ui->srcEdit->text().isEmpty() &&
				 !ui->destEdit->text().isEmpty() &&
				 !ui->rootEdit->text().isEmpty() &&
				 !ui->encCombo->currentText().isEmpty() &&
				 ui->indexCombo->currentIndex() != -1;

	ui->generateButton->setEnabled(OK);
}

void MainWindow::processStarted(void)
{
	ui->generateButton->setEnabled(false);
	ui->generateButton->setVisible(false);
	ui->progressBar->setTextVisible(true);
}

void MainWindow::processEnded(void)
{
	ui->generateButton->setEnabled(true);
	ui->generateButton->setVisible(true);
	ui->progressBar->setTextVisible(false);

	QMessageBox::information(this, tr("Progress"), tr("Job done"));
}
