#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->settingApplication();
    this->createSignalSlotConnections();
}

//-------------------------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
}

//-------------------------------------------------------------------

void MainWindow::settingApplication()
{
    this->setWindowTitle("blur-эффект");

    this->setWindowIcon(QIcon(":/icons/ps.png"));

    ui->openFile_action->setIcon(QIcon(":/icons/openFile.png"));
    ui->openFile_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));

    ui->saveFile_action->setIcon(QIcon(":/icons/saveFile.png"));
    ui->saveFile_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    ui->exit_action->setIcon(QIcon(":/icons/cross.ico"));
    ui->exit_action->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));

    ui->slider->setRange(0, 99);

    ui->label->setMinimumSize(300, 200);
}

//-------------------------------------------------------------------

void MainWindow::createSignalSlotConnections()
{
    connect(ui->exit_action, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->openFile_action, SIGNAL(triggered(bool)), this, SLOT(loadImage()));
    connect(ui->saveFile_action, SIGNAL(triggered(bool)), this, SLOT(saveImage()));
    connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(changeImage(int)));
}

//-------------------------------------------------------------------

void MainWindow::loadImage()
{
    QString path = QDir::currentPath() + "/opt/non_blured";
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл jpg", path, "(*.jpg)");
    if (!fileName.isEmpty())
    {
        m_image.load(fileName);

        if (!m_image.isNull())
        {
            m_bluredImage = m_image;
            ui->slider->setValue(0);
            ui->label->setPixmap(QPixmap::fromImage(m_bluredImage.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось загрузить jpg-файл");
        }
    }
}

//-------------------------------------------------------------------

void MainWindow::saveImage()
{
    if(m_bluredImage.isNull())
    {
        return;
    }

    QString path = QDir::currentPath() + "/opt/blured/";
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить картинку", path, "Картинка (*.jpg)");
    if (!fileName.isEmpty())
    {
        if (!m_bluredImage.save(fileName, "JPEG"))
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить картнику.");
        }
    }
}

//-------------------------------------------------------------------

void MainWindow::changeImage(int value)
{
    if(m_image.isNull())
    {
        return;
    }

    QGraphicsScene scene;
    QGraphicsPixmapItem item(QPixmap::fromImage(m_image));

    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(value);
    item.setGraphicsEffect(blur);

    scene.addItem(&item);

    m_bluredImage = QImage(m_image.size(), QImage::Format_RGB32);
    m_bluredImage.fill(Qt::transparent);

    QPainter painter(&m_bluredImage);

    scene.render(&painter);

    ui->label->setPixmap(QPixmap::fromImage(m_bluredImage.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}

//-------------------------------------------------------------------

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(!m_bluredImage.isNull())
    {
        ui->label->setPixmap(QPixmap::fromImage(m_bluredImage.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }

    Q_UNUSED(event)
}
