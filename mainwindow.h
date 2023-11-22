#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_button_clickMe_clicked();

    void on_field1_clicked();

    void on_button_setShips_clicked();

    void on_field1_156_clicked();

private:
    Ui::MainWindow *ui;

    void setFieldColor();

    void updateFieldOpponentColor();
};
#endif // MAINWINDOW_H
