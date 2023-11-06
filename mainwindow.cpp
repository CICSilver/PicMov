#include "mainwindow.h"
#include "uiconfig.h"
#include <QPixmap>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QScrollBar>
#include <QLabel>
#include <QKeyEvent>
#include "configmanager.h"
#ifdef __DEBUG
#include <QDebug>
#endif
MainWindow::MainWindow(QWidget *parent, int _interval, int _speed)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // 初始化成员函数
    m_scene = new QGraphicsScene(this);
    m_config = nullptr;
    ui.picView->setScene(m_scene);
    m_scroll_speed = _speed;
    m_scroll_interval = _interval;
    m_imageItem = nullptr;
    m_is_scrolling = false;
    m_is_finished = false;
    m_timer = new QTimer(this);
    setPicSize(QSize(DEFAULT_PIC_WIDTH, DEFAULT_PIC_HEIGHT));

    // 读取配置
    updateFromConfig();

    // 注册事件过滤器
    ui.picView->installEventFilter(this);
    ui.statusBar->installEventFilter(this);

    // 初始化界面
    ui.centralWidget->setContentsMargins(QMargins(0, 0, 0, 0));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(scrollImage()));

    // 更新状态栏
    updateStatusBar();
}

void MainWindow::updateFromConfig()
{
    ConfigData* data = ConfigManager::Get()->GetConfigData();
    m_scroll_speed = data->scroll_speed;
    m_scroll_interval = data->scroll_interval;
    m_pic_size = QSize(data->pic_width, data->pic_height);
}

void MainWindow::setImage(const QPixmap& pixmap)
{
    m_imageItem = m_scene->addPixmap(pixmap);
    m_scene->setSceneRect(QRectF(QPointF(0, 0), pixmap.size()));
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == ui.picView && event->type() == QEvent::MouseButtonPress)
    {
        if (!m_is_scrolling)
            startScroll();
        else
            stopScroll();
        m_is_scrolling = !m_is_scrolling;
    }
    return QMainWindow::eventFilter(obj, event); 
}

void MainWindow::on_configAct_triggered()
{
    m_config = new UiConfig(m_scroll_speed, m_scroll_interval, m_pic_size.width(), m_pic_size.height(), this);
    auto result = m_config->exec();
    if (result == QDialog::Accepted)
    {
        updateStatusBar();
        // 应用配置并保存
        updateFromConfig();
        ConfigManager::Get()->SaveConfigFile();
    }
}

void MainWindow::setImage(const QString& path)
{
    QImage image(path);
    int i_width = image.width() * 4;
    int i_height = image.height() * 4;
    image = image.scaled(i_width, i_height).scaled(m_pic_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap scaled_pixmap = QPixmap::fromImage(image);
    
    this->setImage(scaled_pixmap);
}

void MainWindow::updateStatusBar()
{
    // 设置状态栏
    QLabel* statusLabel = new QLabel;
    statusLabel->setMinimumSize(150, 20);
    statusLabel->setFrameShape(QFrame::WinPanel);
    statusLabel->setFrameShadow(QFrame::Sunken);
    ui.statusBar->addWidget(statusLabel);
    QString text = QString("%1=%2    %3=%4")
        .arg(QStringLiteral("滚动速度"))
        .arg(QString::number(m_scroll_speed))
        .arg(QStringLiteral("滚动间隔"))
        .arg(QString::number(m_scroll_interval));
    statusLabel->setText(text);
}

void MainWindow::startScroll()
{
	m_timer->start(m_scroll_interval);
}

void MainWindow::stopScroll()
{
    m_timer->stop();
}

void MainWindow::scrollImage()
{
    auto pScrollbar = ui.picView->verticalScrollBar();
    int cur_scroll_pos = pScrollbar->value();
    if (cur_scroll_pos >= pScrollbar->maximum()) {
        stopScroll();
    }
    else
    {
        pScrollbar->setValue(cur_scroll_pos + 10);
    }
}

MainWindow::~MainWindow()
{}
