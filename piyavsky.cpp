#include "piyavsky.h"
#include <qdebug.h>
#include <math.h>

Piyavsky::Piyavsky(QString function, bool max, double A, double B, double Eps, double H){
    parseInputFunction(function);
    a = A;
    b = B;
    eps = Eps;
    h = H;
    L = calculateLipschitzConst();
    if (max) L *= -1;
    qDebug("a=%1.2f, b=%1.2f, eps=%1.2f, h=%1.2f", a, b, eps, h);
}

int Piyavsky::parseInputFunction(QString function){
    te_variable vars[] = {{"x", &te_var}};
    int err = 0;
    //Compile the expression with variables.
    expr = te_compile(function.toStdString().c_str(), vars, 1, &err);
    return err;
}

double Piyavsky::f(double x){
    if (expr) {
        te_var = x;
        return te_eval(expr);
    }
    return 0;
}

QList<QLineF> Piyavsky::getLines(){
    return lines;
}

struct greater
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

void Piyavsky::calculate(){
    extremum.setX(a);
    extremum.setY(f(a));

    double Lx = a;
    double Rx = b;
    double PIx, PILx, PIRx;

    QList<double> U, g;
    U.append(a);
    U.append(b);

    PIx = getPointIntersection(Lx, Rx);

    double tempY = -L * (PIx-a) + f(Lx);//
    QLineF line;

    line.setLine(Lx, f(Lx), PIx, tempY);
    lines.append(line);

    line.setLine(PIx, tempY, Rx, f(Rx));
    lines.append(line);

    do
    {
        PIx = getPointIntersection(Lx, Rx);
        PILx = getPointIntersection(Lx, PIx);
        PIRx = getPointIntersection(PIx, Rx);
        U.append(PIx);
        std::sort(U.begin(), U.end());
        g.clear();

        for (int i = 1; i < U.size(); i++) {
            g.append(U[i-1]);
            g.append(getPointIntersection(U[i-1], U[i]));
            g.append(U[i]);
        }

        std::sort(g.begin(), g.end());
        tempY = -L * (PIRx-PIx) + f(PIx);//

        line.setLine(Lx, f(Lx), PILx, tempY);
        lines.append(line);

        line.setLine(PILx, tempY, PIx, f(PIx));
        lines.append(line);

        line.setLine(PIx, f(PIx), PIRx, tempY);
        lines.append(line);

        line.setLine(PIRx, tempY, Rx, f(Rx));
        lines.append(line);

        int gi_max = max_diff(g);
        Rx = g[gi_max+1];
        Lx = g[gi_max-1];
    }
    while(abs(f(PIx) - f(getPointIntersection(Lx, Rx))) > eps);

    extremum.setX(getPointIntersection(Lx, Rx));
    extremum.setY(f(getPointIntersection(Lx, Rx)));
}

QPointF Piyavsky::getGlobalExtremum(){
    return extremum;
}

double Piyavsky::calculateLipschitzConst()
{
    double tmpL, l = 0;
    for(double x = a + h; x <= b; x += h)
    {
        tmpL = abs(f(x) - f(x - h));
        if(tmpL > l) { l = tmpL; }
    }
    l /= h;
    return l;
}

double Piyavsky::getPointIntersection(double Lx, double Rx)
{
    return (f(Lx) - f(Rx))/(2*L) + (Lx + Rx)/2;
}


int Piyavsky::max_diff(QList<double> list){
    if (list.size() < 3) return 1;
    double max = abs(f(list[1]) - L * (list[1]-list[0]) + f(list[0]));
    int imax = 1;
    for (int i = 1; i+2 <= list.size(); i+=2) {
        double tmp = abs(f(list[i-1]) - L * (list[i]-list[i-1]) + f(list[i-1]));
        if (tmp > max){
            imax = i;
            max = tmp;
        }
    }
    return imax;
}

Piyavsky::~Piyavsky(){
    te_free(expr);
}
