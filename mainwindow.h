#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#define DEFAULT_PIC_WIDTH   3840
#define DEFAULT_PIC_HEIGHT  2160
class UiConfig;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr, int _scroll_interval = 200, int _scroll_speed = 10);
	~MainWindow();

	void updateFromConfig();

	void startScroll();

	void stopScroll();

	void setImage(const QString& path);

	void updateStatusBar();

public:
	// Setter/Getter
	void setScrollSpeed(int speed) { m_scroll_speed = speed; }
	int getScrollSpeed() { return m_scroll_speed; }

	void setScrollInterval(int interval) { m_scroll_interval = interval; }
	int getScrollInterval() { return m_scroll_interval; }

	void setPicSize(QSize size)
	{
		m_pic_size = size;
		this->setMinimumWidth(size.width());
	}

	void setPicSize(int width, int height) { setPicSize(QSize(width, height)); }
	QSize getPicSize() { return m_pic_size; }

private:
	void setImage(const QPixmap& pixmap);

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
	void scrollImage();

	void on_configAct_triggered();

	//void on_startBtn_clicked();

	//void on_speedBtn_clicked();

private:
	Ui::MainWindowClass ui;
	UiConfig* m_config;
	QGraphicsScene* m_scene;
	QGraphicsPixmapItem* m_imageItem;
	QTimer* m_timer;
	int m_scroll_speed;
	int m_scroll_interval;
	bool m_is_scrolling;
	bool m_is_finished;
	QSize m_pic_size;
};
