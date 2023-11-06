#pragma once
#include <QDialog>
#include <QMetaObject>
#include "ui_uiconfig.h"
class UiConfig : public QDialog
{
	Q_OBJECT

public:
	UiConfig(int _speed, int _interval, int width, int height, QWidget *parent = nullptr);
	~UiConfig();

public slots:

	void on_confirmBtn_clicked();

	void on_cancelBtn_clicked();

private:
	Ui::UiConfigClass ui;
};
