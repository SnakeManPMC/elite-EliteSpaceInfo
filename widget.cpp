#include <QtWidgets>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	// load briefings!? :) heh read the EM.TXT file and kind of launch this program...
	Load_Briefings();
}

Widget::~Widget()
{
	delete ui;
}

void Widget::Load_Briefings()
{
	QFile file( ("d://coding//test_files//em.txt") );

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this, tr("Unable to open file"),
		file.errorString());
		return;
	}

	QString line;
	QTextStream in(&file);

	while (!in.atEnd())
	{
		line = in.readLine();
		commodities.push_back(line);
	}
	file.close();

	// fill our drop down list combobox with all the commodities
	FillComboBox();

	// fill our start/end line edits with all the star systems
	FillStarSystemLineEdits();
}

void Widget::FillComboBox()
{
	QStringList list;
	int i = 0;
	while (i < commodities.size())
	{
		list = commodities[i].split(",");
		i++;

		// check if our commodity is already in the myComboBoxStuff list
		if (!myComboBoxStuff.contains(list[2]))
		{
			// if not, then add it!
			myComboBoxStuff.push_back(list[2]);
		}
	}

	// sort them in alphabetical order
	myComboBoxStuff.sort();

	// fill combobox with the commodities :)
	ui->comboBox->addItems(myComboBoxStuff);
}

void Widget::FillStarSystemLineEdits()
{
	QStringList list;
	int i = 0;
	while (i < commodities.size())
	{
		list = commodities[i].split(",");
		i++;

		// check if our commodity is already in the myComboBoxStuff list
		if (!StarSystemNames.contains(list[0]))
		{
			// if not, then add it!
			StarSystemNames.push_back(list[0]);
		}
	}

	// sort them in alphabetical order
	StarSystemNames.sort();

	QCompleter *completer = new QCompleter(StarSystemNames, this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui->StarSystem_Start_lineEdit->setCompleter(completer);
	ui->StarSystem_End_lineEdit->setCompleter(completer);
}

void Widget::CalculateCommodity(QString Commodity)
{
	QStringList list;
	QString stationlowest, stationhighest;

	int i = 0;
	bool ok;
	int sell,buy,lowest=32000,highest=0;

	while (i < commodities.size())
	{
		list = commodities[i].split(",");
		//ui->textEdit->append("Star System: " + list[0] + ", Station: " + list[1] + ", Commodity: " + list[2] + ", Sell: " + list[3] + ", Buy: " + list [4]);
		sell = list[3].toInt(&ok, 10);
		buy = list[4].toInt(&ok, 10);
		i++;

		if (list[2].compare(Commodity)==0)
		{
			if (buy > 0 && buy < lowest)
			{
				lowest = buy;
				stationlowest = list[0];
			}

			if (sell > highest)
			{
				highest = sell;
				stationhighest = list[0];
			}
		}
	}

	// check that we dont have invalid data somehow, we have positive profit result
	if (highest-lowest < 0)
	{
		ui->textEdit->append("Sorry no proper profit data found.");
	}
	else

	// display our results
	ui->textEdit->append(Commodity + " price lowest: " + stationlowest + " : " + QString::number(lowest) + ", and highest: " + stationhighest + " : " +
			     QString::number(highest) + ", unit profit: " + QString::number(highest-lowest));
}

// update textEdit when we change commodities in commodity drop down list
void Widget::on_comboBox_currentTextChanged()
{
	CalculateCommodity(ui->comboBox->currentText());
}

// search button clicked, we update shit ;)
void Widget::on_pushButton_clicked()
{
	CalculateStartEnd(ui->StarSystem_Start_lineEdit->text(),ui->StarSystem_End_lineEdit->text());
}

// this is the "Search" pushbutton calculation
void Widget::CalculateStartEnd(QString SStart, QString SEnd)
{
	ui->textEdit->append("YEAH! Start Star System: " + SStart + ", End Star System: " + SEnd);
}

// highest profit button
void Widget::on_pushButton_2_clicked()
{

	QStringList list;
	QString stationlowest, stationhighest, Commodity, highestCommodity, startsystem, endsystem;

	int i=0,cx=0,highprofit=0;
	bool ok;
	int sell,buy,lowest=32000,highest=0;

	// loop #1 - all commodities
	while (cx < myComboBoxStuff.size())
	{
		Commodity = myComboBoxStuff[cx];

		// loop #2 - all systems, stations and commodites
		while (i < commodities.size())
		{
			list = commodities[i].split(",");
			//ui->textEdit->append("Star System: " + list[0] + ", Station: " + list[1] + ", Commodity: " + list[2] + ", Sell: " + list[3] + ", Buy: " + list [4]);
			sell = list[3].toInt(&ok, 10);
			buy = list[4].toInt(&ok, 10);
			i++;

			// we have the same commodity
			if (list[2].compare(Commodity)==0)
			{
				if (buy > 0 && buy < lowest)
				{
					lowest = buy;
					stationlowest = list[0];
				}

				if (sell > highest)
				{
					highest = sell;
					stationhighest = list[0];
				}
			}
			// set profit
			if (highest - lowest > highprofit)
			{
				highprofit = (highest - lowest);
				startsystem = stationlowest;
				endsystem = stationhighest;
				highestCommodity = Commodity;
			}
		} // end of loop #2
		i = 0;
		lowest=32000,highest=0;
		cx++;
	} // end of loop #1

	// display our results
	ui->textEdit->append(startsystem + " -> " + endsystem + ": " + highestCommodity + " " + QString::number(highprofit) + "cr / unit");
}

/*
EliteOCR CSV file parser

	QStringList list;
	int i = 0;
	while (i < commodities.size())
	{
		list = commodities[i].split(";");
		i++;

		ui->textEdit->append("System: " + list[0] + "\nStation: " + list[1] + "\nCommodity: " + list[2] + "\nSell: " + list[3] + "\nBuy: " + list[4]
				+ "\nDemand: " + list[5] + "\nDemand chars???: " + list[6] + "\nSupply: " + list[7] + "\nSupply chars???: " + list[8]
				+ "\nDateTime: " + list[9]);
	}
*/
