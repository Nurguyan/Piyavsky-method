#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tinyexpr-master/tinyexpr.h"
#include "piyavsky.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    double f(double x);
    te_expr *expr;
    double te_var;

    double minY, maxY;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();   

private slots:
    void on_BT_FIND_clicked();

private:
    int parseInputFunction(QString function);
    void drawGraph(double a, double b, double h);
    void drawBrokenLines(QList<QLineF> lines);
    void drawLine(QLineF line);
    void drawPoint(QPointF point);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
