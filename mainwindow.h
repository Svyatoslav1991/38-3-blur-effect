#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QImage m_image;
    QImage m_bluredImage;

    void settingApplication();
    void createSignalSlotConnections();

private slots:
    void loadImage();
    void saveImage();
    void changeImage(int value);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event);
};
#endif // MAINWINDOW_H
