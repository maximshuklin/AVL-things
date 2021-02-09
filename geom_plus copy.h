#ifndef GEOM_PLUS_H
#define GEOM_PLUS_H
#include "mainwindow.h"
#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <iostream>


int convertToInt(QString s) {
    std::string new_s = s.toStdString();
    int value = 0;
    for (char c : new_s) {
        value = value * 10 + (int)(c - '0');
    }
    return value;
}

std::string convertToCharX(int x) {
    if (x == 0) {
        return "0";
    }
    std::string res = "";
    std::vector<int> temp;
    while (x > 0) {
        res = res + char('0' + x % 10);
        x /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}


struct point
{
    double x, y;
    point() {}
    point(double _x, double _y) {
        x = _x;
        y = _y;
    }
};
point operator+(point a, point b) {
    return point(a.x + b.x, a.y + b.y);
}
point operator-(point a, point b) {
    return point(a.x - b.x, a.y - b.y);
}

point operator*(point a, double k) {
    return point(a.x * k, a.y * k);
}

double dist(point a, point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

struct rect
{
    double x1, y1, x2, y2;
    rect() {}
    rect(double _x1, double _y1, double _x2, double _y2) {
        x1 = _x1;
        x2 = _x2;
        y1 = _y1;
        y2 = _y2;
    }
};

struct Circle
{
    double x, y;
    double R; // radius
    int number;

    Circle() {}
    Circle(point _O, double _R) {
        x = _O.x;
        y = _O.y;
        R = _R;
    }
    Circle(double _x, double _y, double _R) {
        x = _x;
        y = _y;
        R = _R;
    }
    Circle(double _x, double _y, double _R, int _number) {
        x = _x;
        y = _y;
        R = _R;
        number = _number;
    }
};





#endif // GEOM_PLUS_H
