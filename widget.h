#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();
	void Load_Briefings();
	void CalculateCommodity(QString Commodity);
	void CalculateStartEnd(QString SStart, QString SEnd);
	void FillComboBox();
	void FillStarSystemLineEdits();
	QVector<QString> commodities;
	QStringList StarSystemNames;
	QStringList myComboBoxStuff;

private slots:
//	void on_comboBox_currentTextChanged(const QString &arg1);
	void on_comboBox_currentTextChanged();

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

private:
	Ui::Widget *ui;
};

#endif // WIDGET_H
