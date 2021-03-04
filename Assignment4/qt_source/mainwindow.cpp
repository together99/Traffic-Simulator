#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Vehicle Platoon Simulation");

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    //ui->label_2->setPixmap(QPixmap::fromImage(*empty_cell).scaled(70,50,Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tickButton_clicked()
{
    if (total_tick_count == 0)
    {
        QMessageBox::information(0, "Error", "Simulation needs to start first.");
        return;
    }

    // Go to the next command counter
    index_counter++;
    if (index_counter >= total_tick_count)
    {
        QMessageBox::information(0, "Error", "The end of simulation.");
        index_counter--;
        return;
    }

    drawVehicles(index_counter);
}



void MainWindow::on_startButton_clicked()
{
    total_tick_count = 0;
    index_counter = 0;
    scenario.clear();

    if (fileflag == 0){
        QMessageBox::information(0, "Error", "Select the input file.");
        return;
    }
    // open the result file
    QFile file(inputFileName);
    QDir dir;
    qDebug() << dir.absolutePath() << endl;
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(0, dir.absolutePath(), "Cannot find the result.txt file.");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        scenario.append(line);
        total_tick_count++;
    }

    drawVehicles(0);
}

void MainWindow::on_prevButton_clicked()
{
    if (total_tick_count == 0)
    {
        QMessageBox::information(0, "Error", "Simulation needs to start first.");
        return;
    }

    // Go to the previous command counter
    index_counter--;
    if (index_counter < 0)
    {
        QMessageBox::information(0, "Error", "It is the beginning of the simulation.");
        index_counter = 0;
        return;
    }

    drawVehicles(index_counter);
}


void MainWindow::drawVehicles(int cmd_index){
    // clear the scene first
    scene->clear();

    QString line = scenario[cmd_index];
    QStringList list = line.split(";");

    // update the time value
    QString time = list[0];
    ui->tickCounter->setText(time);

    QBrush redBrush(Qt::red);
    QBrush whiteBrush(Qt::white);
    QPen blackpen(Qt::black);
    blackpen.setWidth(4);

    // remove all the vehicles, and simply redraw them all
    // here clear the vehicles
    // iterate over the command
    int i = 0;
    int max_x = 0;
    int max_lane = 0;
    for (QStringList::iterator it = list.begin(); it != list.end(); ++it){
        // just to skip the first element
        if (i == 0) {
            i++;
            continue;
        }
        QString s = *it;

        // remove the curly bracket
        s.remove("(");
        s.remove(")");

        QStringList items = s.split(",");
        QString label = "id:" + items[0] + "\r\nlane:" + items[1] + "\r\npos:" + items[2];
        int lane = items[1].toInt();
        int pos = items[2].toInt();
        int light = items[3].toInt();

        int x = pos * 70;
        int y = lane * 50 + lane * 10; // 10 is a padding

        if (x > max_x){
            max_x = x;
        }
        if (lane > max_lane){
            max_lane = lane;
        }

        scene->addRect(x, y, 70, 50, blackpen, whiteBrush);


        if (light == 0){
            scene->addEllipse(x+50, y+5, 15, 15, blackpen, whiteBrush);
            scene->addEllipse(x+50, y+30, 15, 15, blackpen, whiteBrush);
        }
        else if (light == 1){
            scene->addEllipse(x+50, y+5, 15, 15, blackpen, redBrush);
            scene->addEllipse(x+50, y+30, 15, 15, blackpen, whiteBrush);
        }
        else{
            scene->addEllipse(x+50, y+5, 15, 15, blackpen, whiteBrush);
            scene->addEllipse(x+50, y+30, 15, 15, blackpen, redBrush);
        }
        QGraphicsTextItem *text = scene->addText(label);
        text->setPos(x,y);

        qDebug() << s << endl;
    }

    blackpen.setWidth(2);
    for (int i = 0; i <= max_lane+1; i++){
        scene->addLine(-100, 60*i-5, max_x+200, 60*i-5,blackpen);
    }

    for (int i = 0; i <= (max_x + 200)/70; i++){
        scene->addLine(i*70,0,i*70,max_lane*60+60,blackpen);
    }

}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/",
                "Text File (*.txt)"
                );

    inputFileName = filename;
    fileflag = 1;
}
