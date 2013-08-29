#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>

using std::vector;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    ~Widget();
    
private slots:
    void on_getRand_clicked();

    void on_runTest_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
    vector<int> lineX,lineY;
    int n;
    int *x,*y;
};

#endif // WIDGET_H
