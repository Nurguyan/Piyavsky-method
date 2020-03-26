#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BT_FIND_clicked()
{
    QString function = ui->ET_FX->text();
    QString begin = ui->ET_A->text();
    QString end = ui->ET_B->text();
    QString epsilon = ui->ET_EPS->text();
    QString h_str = ui->ET_H->text();
    bool max = false;
    if (ui->RB_MAX->isChecked()) max = true;
    if (function.isEmpty() || begin.isEmpty() || end.isEmpty() || epsilon.isEmpty() || h_str.isEmpty()) return;

    const double eps = epsilon.toDouble();
    const double a = begin.toDouble();  //Interval start Ox
    const double b = end.toDouble();    //Interval end Ox
    const double h = h_str.toDouble();  //Step Ox

    if (a > b ) {QMessageBox::critical(0, "Error", "a must be less than b!"); return;}
    ui->widget->clearGraphs();

    Piyavsky piyavsky(function, max, a, b, eps, h);

    qDebug() << "Lipschitz constant = " << piyavsky.L;

    drawGraph(&piyavsky, &Piyavsky::f, a, b, h);


    QList<QLineF> lines = piyavsky.getLines();
    drawBrokenLines(lines);

    if (!max)
    {
        //if min
        ui->LB_ACTUAL->setText("Min f(x) (actual)");
        ui->LB_PIYAVSKY->setText("Min f(x) (Piyavsky method)");
        ui->widget->yAxis->setRange(lines.value(0).y2(), 2*maxY);
        ui->ET_ACTUAL->setText(QString::number(minY));
        qDebug("f(x) min (actual): %1.2f", minY);
    }
    else
    {
        //if max
        ui->LB_ACTUAL->setText("Max f(x) (actual)");
        ui->LB_PIYAVSKY->setText("Max f(x) (Piyavsky method)");
        ui->widget->yAxis->setRange(2*minY, lines.value(0).y2());
        ui->ET_ACTUAL->setText(QString::number(maxY));
        qDebug("f(x) max (actual): %1.2f", maxY);
    }

    ui->ET_PIYAVSKY->setText(QString::number(piyavsky.getGlobalExtremum().y()));

    qDebug("Global extremum (Piyavsky method):\nx = %1.2f\ny = %1.2f", piyavsky.getGlobalExtremum().x(), piyavsky.getGlobalExtremum().y());
    drawPoint(piyavsky.getGlobalExtremum()); //draw extremum point on the graph

    //Подписываем оси Ox и Oy
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    //И перерисуем график на нашем widget
    ui->widget->replot();
}

void MainWindow::drawGraph(Piyavsky *p, double (Piyavsky::*f)(double),double a, double b, double h){
    int N = (b - a) / h + 2;
    QVector<double> x(N), y(N); //coordinates
    minY = maxY = (p->*f)(a);
    int i = 0;
    for (double X = a; X <= b; X += h)
    {
        x[i] = X;
        y[i] = (p->*f)(X);
        if (y[i] > maxY) maxY = y[i];
        if (y[i] < minY) minY = y[i];
        i++;
    }

    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);
    ui->widget->graph(0)->setPen(QPen(QColor(170, 0, 0)));
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->widget->graph(0)->setName("f(x)");
    ui->widget->xAxis->setRange(a, b); //range Ox
}

void MainWindow::drawBrokenLines(QList<QLineF> lines){
    for (int i = 0; i < lines.size(); i++) {
        drawLine(lines.value(i));
    }
}

void MainWindow::drawLine(QLineF line){
    //qDebug() << line;
    QVector<double> lineX, lineY;
    int k = ui->widget->graphCount();
    lineX.push_back(line.x1());
    lineX.push_back(line.x2());
    lineY.push_back(line.y1());
    lineY.push_back(line.y2());
    ui->widget->addGraph();
    ui->widget->graph(k)->setData(lineX, lineY);
}

void MainWindow::drawPoint(QPointF point){
    QVector<double> x, y;
    x.push_back(point.x());
    y.push_back(point.y());
    int k = ui->widget->graphCount();
    ui->widget->addGraph();
    ui->widget->graph(k)->setPen(QColor(0, 0, 0, 255));
    ui->widget->graph(k)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(k)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
    ui->widget->graph(k)->setName("Extremum");
    ui->widget->graph(k)->setData(x, y);
}
