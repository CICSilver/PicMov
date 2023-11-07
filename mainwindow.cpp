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
#include <QScreen>
#include <QLabel>
#include <QFileDialog>
#ifdef __DEBUG
#include <QDebug>
#endif
MainWindow::MainWindow(QWidget *parent, int _interval, int _speed)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // ��ʼ����Ա����
    m_scene = new QGraphicsScene(this);
    m_config = nullptr;
    ui.picView->setScene(m_scene);
    m_scroll_speed = _speed;
    m_scroll_interval = _interval;
    m_imageItem = nullptr;
    m_is_scrolling = false;
    m_is_finished = false;
    m_statusLabel = nullptr;
    m_timer = new QTimer(this);

    // ��ȡ����
    updateFromConfig();

    if (m_pic_size.width() == DEFAULT_PIC_WIDTH)
    {
        // Ĭ��ʹ����Ļ���
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();
        int width = screenGeometry.width();
        int height = width * sqrt(2);
        setPicSize(QSize(width, height));
    }

    // ������ԴĿ¼
    QDir dir;
    dir.mkdir("./resource");

    // ע���¼�������
    ui.picView->installEventFilter(this);
    ui.statusBar->installEventFilter(this);

    // ��ʼ������
    ui.centralWidget->setContentsMargins(QMargins(0, 0, 0, 0));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(scrollImage()));

    // ����״̬��
    updateStatusBar();

    this->showMaximized();
}

void MainWindow::updateFromConfig()
{
    ConfigData* data = ConfigManager::Get()->GetConfigData();
    m_scroll_speed = data->scroll_speed;
    m_scroll_interval = data->scroll_interval;
    m_pic_size = QSize(data->pic_width, data->pic_height);
    setPicSize(m_pic_size);
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

void MainWindow::on_cfgBtn_clicked()
{
    m_config = new UiConfig(m_scroll_speed, m_scroll_interval, m_pic_size.width(), m_pic_size.height(), this);
    auto result = m_config->exec();
    if (result == QDialog::Accepted)
    {
        updateStatusBar();
        // Ӧ�����ò�����
        updateFromConfig();
        ConfigManager::Get()->SaveConfigFile();
    }
}

void MainWindow::on_openBtn_clicked()
{
    QFileDialog fileDialog(this);
    QString path = fileDialog.getOpenFileName(this, QStringLiteral("ѡ��ͼƬ"), "./resource", QStringLiteral("ͼƬ�ļ�(*.jpg *.png)"));
    if (!path.isEmpty())
    {
	    setImage(path);
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
    // ����״̬��
    if (!m_statusLabel)
    {
        // ��ʼ��
        m_statusLabel = new QLabel;
        m_statusLabel->setMinimumSize(150, 20);
        m_statusLabel->setFrameShape(QFrame::WinPanel);
        m_statusLabel->setFrameShadow(QFrame::Sunken);
        ui.statusBar->addWidget(m_statusLabel);
    }
    QString text = QString("%1=%2    %3=%4")
        .arg(QStringLiteral("�����ٶ�"))
        .arg(QString::number(m_scroll_speed))
        .arg(QStringLiteral("�������"))
        .arg(QString::number(m_scroll_interval));
    m_statusLabel->setText(text);
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
