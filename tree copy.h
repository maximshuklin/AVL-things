#ifndef TREE_H
#define TREE_H
#include "mainwindow.h"
#include "geom_plus.h"
#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <iostream>

using namespace std;

double zoom = 1.0;

// AVL-Tree
//---------------------------------------------------------------------------------------------

struct node
{
    int x;
    int h;
    node* left, *right;

    node() {
        x = 0;
        h = 1;
        left = right = NULL;
    }

    node(int _x) {
        x = _x;
        h = 1;
        left = right = NULL;
    }
};

void print(node* v) {
    if (v) {
         if (v->left != NULL) {
             cout << (v->x) << " -> " << (v->left->x) << endl;
         }
         if (v->right != NULL) {
             cout << (v->x) << " -> " << (v->right->x) << endl;
         }
        print(v->left);
        print(v->right);
    }
}

int get_h(node* v) {
    if (v) {
        return v->h;
    }
    return 0;
}

void update(node* &v) {
    if (v) {
        v->h = max(get_h(v->left), get_h(v->right)) + 1;
    }
}

void fixDisbalance(node *&v) {
    // abs(v->l) != abs(v->r)
    int x = get_h(v->left);
    int y = get_h(v->right);

    if (abs(x - y) != 2) {
        cout << "WAAAAA\n";
        exit(0);
    }


    if (x + 2 == y) {
        node* a = v;
        node* L = v->left;
        node* b = v->right;
        node* c = b->left;
        node* r = b->right;
        if (get_h(c) == get_h(r) || get_h(c) + 1 == get_h(r)) {
            // small rotate
            a->right = c;
            b->left = a;

            v = b;
        } else {
            // big rotate
            node* m = c->left;
            node* n = c->right;

            a->right = m;
            b->left = n;
            c->left = a;
            c->right = b;

            v = c;
        }
    } else {
        node* a = v;
        node* b = v->left;
        node* R = v->right;
        node* L = b->left;
        node* c = b->right;
        if (get_h(L) == get_h(c) || get_h(c) + 1 == get_h(L)) {
            // small rotate
            a->left = c;
            b->right = a;

            v = b;
        } else {
            // big rotate
            node* m = c->left;
            node* n = c->right;

            b->right = m;
            a->left = n;
            c->left = b;
            c->right = a;

            v = c;
        }
    }
    update(v->left);
    update(v->right);
    update(v);
}

void insert(node* &v, int x) {
    if (v == NULL) {
        v = new node(x);
    } else if (v->x == x) {
        // no need to insert x
        return;
    } else {
        if (v->x > x) {
            insert(v->left, x);
        } else {
            insert(v->right, x);
        }
        update(v);
        if (abs(get_h(v->left) - get_h(v->right)) > 1) {
            fixDisbalance(v);
        }
    }
}

node* rightest(node* v) {
    node* kek = NULL;
    while (v != NULL) {
        kek = v;
        v = v->right;
    }
    return kek;
}

node* leftest(node* v) {
    node* kek = NULL;
    while (v != NULL) {
        kek = v;
        v = v->left;
    }
    return kek;
}

void del(node *& v, int x) {
    if (v == NULL) {
        return ;
    } else {
        if (v->x < x) {
            del(v->right, x);
        } else if (v->x > x) {
            del(v->left, x);
        } else {
            if ((v->left == NULL) && (v->right == NULL)) {
                v = NULL;
            } else if ((v->left == NULL) && (v->right != NULL)) {
                v = v->right;
            } else if ((v->left != NULL) && (v->right == NULL)){
                v = v->left;
            } else {
                if (get_h(v->left) >= get_h(v->right)) {
                    node* kek = rightest(v->left);
                    v->x = kek->x;
                    del(v->left, v->x);
                } else {
                    node* kek = leftest(v->right);
                    v->x = kek->x;
                    del(v->right, v->x);
                }
            }
        }
    }
    update(v);
    if (v == NULL) {
        return;
    }
    if (abs(get_h(v->left) - get_h(v->right)) > 1) {
        fixDisbalance(v);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------


node* Tree = NULL; // declare a Tree


vector< Circle > Circles;
vector< pair<int, int> > edges;
vector< pair<point, point> > segments;

pair<double, double> DetermineSize(node *v) {
    if (v) {
        if (v->left == NULL && v->right == NULL) {
            return {1, 1};
        }
        pair<double, double> leftRect = DetermineSize(v->left);
        pair<double, double> rightRect = DetermineSize(v->right);
        double w1 = leftRect.first, h1 = leftRect.second;
        double w2 = rightRect.first, h2 = rightRect.second;

        double newW = w1 + w2 + 1;
        double newH = 1.5 + max(h1, h2);
        return {newW, newH};
    }
    return {0, 0};
}


int GO(node *v, rect R, double autozoom) {
    if (v) {
        double x1 = R.x1, y1 = R.y1, x2 = R.x2, y2 = R.y2; // rectangle coordinate;

        pair<double, double> leftRect = DetermineSize(v->left);
        pair<double, double> rightRect = DetermineSize(v->right);
        double w1 = leftRect.first, h1 = leftRect.second;
        double w2 = rightRect.first, h2 = rightRect.second;
        w1 *= autozoom;
        w2 *= autozoom;
        h1 *= autozoom;
        h2 *= autozoom;
        // determine left-right subtree sizes

        // w1 + w2 + 1 == x2 - x1

        double X1 = x1 + w1;
        double X2 = X1 + 1.0 * autozoom;
        double Y1 = y1;
        double Y2 = y1 + 1.0 * autozoom;

        Circle cir = Circle((X1 + X2) / 2.0, (Y1 + Y2) / 2.0, autozoom / 2.0, v->x);
        Circles.push_back(cir);

        int root = (int)Circles.size() - 1;
        int leftSon = GO(v->left, rect(x1, y1 + 1.5 * autozoom, x1 + w1, y2), autozoom);
        int rightSon = GO(v->right, rect(x1 + w1 + 1.0 * autozoom, y1 + 1.5 * autozoom, x2, y2), autozoom);
        if (leftSon != -1) {
            edges.emplace_back(root, leftSon);
        }
        if (rightSon != -1) {
            edges.emplace_back(root, rightSon);
        }
        return root;
    }
    return -1;
}

void Update() {
    Circles.clear();
    edges.clear();
    segments.clear();

    pair<double, double> WH = DetermineSize(Tree);
    double W = WH.first;
    double H = WH.second;

    double autozoom = min((double)Width / W, (double)Height / H);

    GO(Tree, rect(0, 0, Width, Height), autozoom);

    for (auto e : edges) {
        int u = e.first;
        int v = e.second;

        point A = point(Circles[u].x, Circles[u].y);
        point B = point(Circles[v].x, Circles[v].y);
        double len = dist(A, B);
        point n1 = (B - A) *(0.5 * autozoom / len);
        A = A + n1;
        B = B - n1;

        segments.emplace_back(A, B);
    }
}

void addNode(int value) {
    insert(Tree, value);
    Update();
}


void deleteNode(int value) {
    print(Tree);
    del(Tree, value);
    Update();
}

#endif // TREE_H
