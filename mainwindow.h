#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QMessageBox>

#include "piyavsky.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    double minY, maxY;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();   

private slots:
    void on_BT_FIND_clicked();

private:
    void drawGraph(Piyavsky *p, double (Piyavsky::*f)(double),double a, double b, double h);
    void drawBrokenLines(QList<QLineF> lines);
    void drawLine(QLineF line);
    void drawPoint(QPointF point);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
