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

	this->progressBar = new QProgressBar(this);
	this->generateButton = new QPushButton(QIcon::fromTheme("system-run"),
								    tr("Generate files"), this);

	this->progressBar->setVisible(false);
	this->generateButton->setEnabled(false);

	ui->statusBar->addPermanentWidget(generateButton);
	ui->statusBar->addPermanentWidget(progressBar);

	worker->moveToThread(&WorkerThread);
	WorkerThread.start();

	for (const auto& c : QTextCodec::availableCodecs())
		ui->encCombo->addItem(QString::fromLocal8Bit(c));

	ui->encCombo->model()->sort(0);

	connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateButtonClicked);

	connect(worker, &GeneratorWorker::onProgressStart, this, &MainWindow::processStarted);
	connect(worker, &GeneratorWorker::onProgressEnd, this, &MainWindow::processEnded);

	connect(worker, &GeneratorWorker::onProgressSetup, this->progressBar, &QProgressBar::setRange);
	connect(worker, &GeneratorWorker::onProgressChange, this->progressBar, &QProgressBar::setValue);

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
	const QString Path = QFileDialog::getExistingDirectory(this, tr("Select destination path"), QString());

	if (!Path.isEmpty()) ui->destEdit->setText(Path);
}

void MainWindow::generateButtonClicked(void)
{
	this->generateButton->setEnabled(false);

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

	this->generateButton->setEnabled(OK);
}

void MainWindow::processStarted(void)
{
	this->generateButton->setEnabled(false);
	this->generateButton->setVisible(false);
	this->progressBar->setVisible(true);
}

void MainWindow::processEnded(void)
{
	this->generateButton->setEnabled(true);
	this->generateButton->setVisible(true);
	this->progressBar->setVisible(false);

	QMessageBox::information(this, tr("Progress"), tr("Job done"));
}
