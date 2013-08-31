#include "widget.h"
#include "ui_widget.h"
#include "fly_pre.h"
#include "fly.h"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

using namespace std;

#define BIGRAND (rand()*30000+rand())

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    srand(time(0));
    ui->setupUi(this);
    x=y=NULL;n=0;
/*
    QTimer *tt=new QTimer(this);
    connect(tt,SIGNAL(timeout()),this,SLOT(on_getRand_clicked()));
    tt->start(0);
    */
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_getRand_clicked()
{
    int n=ui->randN->text().toInt();
    int s=ui->randX->text().toInt();
    stringstream st;
    st<<n<<endl;
    for (int i=1;i<=n;i++)
        st<<BIGRAND%s<<' '<<BIGRAND%s<<endl;
    ui->inputTest->setPlainText(QString(st.str().c_str()));
    on_runTest_clicked();
}

void Widget::on_runTest_clicked()
{
    stringstream st(ui->inputTest->toPlainText().toStdString().c_str());
    st>>n;
    if (x!=NULL) {
        delete []x;
        delete []y;
    }
    x=new int[n+1];
    y=new int[n+1];
    for (int i=1;i<=n;i++)
        st>>x[i]>>y[i];
    GetTree getTree;
    if (ui->checkL->isChecked()) {
        qDebug()<<"L check";
        getTree.lFlag=1;
    }
    long long tt=clock();
    getTree.work(n,x,y);
    getTree.tree.dp(lineX,lineY);
    stringstream st2;
/*
    pre::GetTree getTree2;
    getTree2.work(n,x,y);
    //getTree2.tree.getMst(lineX,lineY);
    static int find=0;
    st2<<"MST ans: "<<getTree2.w1<<endl;
    st2<<"RSMT ans: "<<getTree2.tree.ans<<endl;
    st2<<find<<endl;
    if (getTree.w1!=getTree2.w1) {
        find=getTree.n;
        ui->inputTest->selectAll();
        ui->inputTest->copy();
    }
*/
    st2<<"cost time: "<<clock()-tt<<endl;
    st2<<"MST ans: "<<getTree.w1<<endl;
    st2<<"RSMT ans: "<<getTree.tree.ans<<endl;
    ui->ansLabel->setText(st2.str().c_str());
    update();
}

void Widget::paintEvent(QPaintEvent *) {
    if (n==0) return;
    int sx=200,sy=10;
    QPainter pt(this);
    pt.setPen(QPen(QBrush(Qt::black),1));
    int scale=1;
    for (size_t i=0;i<lineX.size();i++) if (i%2==0) {
        pt.drawLine(lineX[i]*scale+sx,lineY[i]*scale+sy,lineX[i+1]*scale+sx,lineY[i+1]*scale+sy);
    }
    pt.setBrush(QBrush(Qt::red,Qt::SolidPattern));
    pt.setPen(Qt::NoPen);
    for (int i=1;i<=n;i++)
        pt.drawEllipse(QPoint(x[i]*scale+sx,y[i]*scale+sy),1,2);
}

void Widget::on_pushButton_3_clicked()
{
    QString Nname=QFileDialog::getSaveFileName(this,tr("导出方案..."));
    if (!Nname.isEmpty()) {
        QFile file(Nname);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
          QTextStream out(&file);
          out<<lineX.size()/2<<endl;
          for (size_t i=0;i<lineX.size();i++) if (i%2==0)
              out<<lineX[i]<<' '<<lineY[i]<<' '<<lineX[i+1]<<' '<<lineY[i+1]<<endl;
        } else
          QMessageBox::warning(this,tr("错误"),tr("无法保存文件！"));
    }
}
