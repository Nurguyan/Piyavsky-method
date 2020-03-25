#ifndef PIYAVSKY_H
#define PIYAVSKY_H

#include <QMainWindow>
#include <QLine>
#include "tinyexpr-master/tinyexpr.h"
#include <QList>

class Piyavsky
{
public:
    double a, b, eps, h, L;
    Piyavsky(QString function,  bool max, double A, double B, double Eps, double H);
    double f(double x);
    void calculate();
    QList<QLineF> getLines();
    QPointF getGlobalExtremum();
    ~Piyavsky();

private:
    te_expr *expr;
    double te_var;
    QList<QLineF> lines;

    QPointF extremum;

    int parseInputFunction(QString function);
    double calculateLipschitzConst();
    double getPointIntersection(double Lx, double Rx);
    int max_diff(QList<double> list);

};

#endif // PIYAVSKY_H
