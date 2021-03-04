#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>
#include <QGraphicsTextItem>
#include <Qt>
#include <QDir>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void setScenario(QTextStream *text);

    private slots:
        void on_tickButton_clicked();
        void on_startButton_clicked();
        void on_prevButton_clicked();

        void on_pushButton_clicked();

private:
        // Constants
        QString inputFileName;
        int fileflag = 0;

        int total_tick_count = 0;
        int index_counter = 0;
        void drawVehicles(int cmd_index);

        QGraphicsScene *scene;

        QVector<QString> scenario;
        Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
