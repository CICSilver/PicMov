#include "uiconfig.h"
#include "mainwindow.h"
#include "configmanager.h"
#include <QMessageBox>
UiConfig::UiConfig(int _speed, int _interval, int _width, int _height, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.speedEdit->setText(QString::number(_speed));
	ui.intervalEdit->setText(QString::number(_interval));
	ui.widthEdit->setText(QString::number(_width));
	ui.heightEdit->setText(QString::number(_height));
}

UiConfig::~UiConfig()
{}

void UiConfig::on_confirmBtn_clicked()
{
	if (ui.speedEdit->text().isEmpty() ||
		ui.intervalEdit->text().isEmpty() ||
		ui.widthEdit->text().isEmpty() ||
		ui.heightEdit->text().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("输入不能为空"));
		return;
	}

	ConfigData* data = ConfigManager::Get()->GetConfigData();
	data->scroll_speed = ui.speedEdit->text().toInt();
	data->scroll_interval = ui.intervalEdit->text().toInt();
	data->pic_width = ui.widthEdit->text().toInt();
	data->pic_height = ui.heightEdit->text().toInt();
	accept();
}

void UiConfig::on_cancelBtn_clicked()
{
	reject();
}
