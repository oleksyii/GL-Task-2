#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

bool myTurn = false;
Client* client = new Client(myTurn);

std::vector<std::vector<int>> field;
std::vector<std::vector<int>> fieldOpponent;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Iterate through all widgets in the grid layout
    for (int i = 0; i < ui->gridLayout1->rowCount(); ++i) {
        for (int j = 0; j < ui->gridLayout1->columnCount(); ++j) {
            QLayoutItem *item = ui->gridLayout1->itemAtPosition(i, j);
            if (item) {
                QWidget *widget = item->widget();
                if (widget) {
                    widget->setEnabled(false);
                }
            }
        }
    }

    // Iterate through all widgets in the grid layout
    for (int i = 0; i < ui->gridLayout2->rowCount(); ++i) {
        for (int j = 0; j < ui->gridLayout2->columnCount(); ++j) {
            QLayoutItem *item = ui->gridLayout2->itemAtPosition(i, j);
            if (item) {
                QWidget *widget = item->widget();
                if (widget) {
                    widget->setEnabled(myTurn);
                }
            }
        }
    }


    for (int i = 0; i < 10; i++){
        std::vector<int> row;
        for(int j = 0; j < 10; j++){
            row.push_back(0);
        }
        field.push_back(row);
    }

    for (int i = 0; i < 10; i++){
        std::vector<int> row;
        for(int j = 0; j < 10; j++){
            row.push_back(0);
        }
        fieldOpponent.push_back(row);
    }




//    std::vector<std::vector<int>> res = client->receiveMessage();
}


MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}


void MainWindow::on_button_clickMe_clicked()
{
//    client->sendMessage();
//    std::string str = client->receiveMessage();
//    ui->label_greeting->setText(str.c_str());


}


void MainWindow::on_field1_clicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    // Set the background color of the QPushButton
    clickedButton->setStyleSheet("background-color: red"); // Change 'red' to the desired color

    // Get the current background color of the QPushButton
    QPalette palette = clickedButton->palette();
    QColor currentColor = palette.button().color();

    // Display the current color in the console
    qDebug() << "Current color:" << currentColor;

}


void MainWindow::on_button_setShips_clicked()
{
    field[8][1] = 1;
    field[8][2] = 1;
    field[8][3] = 1;
    field[8][4] = 1;

    field[7][8] = 1;
    field[8][8] = 1;
    field[9][8] = 1;

    field[3][7] = 1;
    field[3][8] = 1;
    field[3][9] = 1;

    field[3][1] = 1;
    field[4][1] = 1;

    field[0][9] = 1;
    field[1][9] = 1;

    field[5][7] = 1;
    field[5][8] = 1;

    field[0][0] = 1;
    field[1][4] = 1;
    field[4][4] = 1;
    field[9][6] = 1;

    client->sendMessage(field);

    setFieldColor();
}

void MainWindow::setFieldColor()
{
    std::cout << field[0][0] << std::endl;

    // Iterate through the field vector
    for (int i = 1; i <= field.size(); i++) {
        for (int j = 1; j <= field[i-1].size(); j++) {
            // Check if the value in the field is 1
            if (field[i-1][j-1] == 1) {
                std::cout << "inside the setFieldColor" << std::endl;
                // Retrieve the widget from the grid layout
                QWidget *widget = ui->gridLayout1->itemAtPosition(i-1, j)->widget();

                // Change the color or perform other actions on the widget
                if (widget) {
                    QPushButton *button = qobject_cast<QPushButton*>(widget);
                    if (button) {
                        button->setStyleSheet("background-color: black"); // Change to your desired color
                    }
                }
            }
        }
    }
}

void MainWindow::updateFieldOpponentColor()
{
    std::cout << fieldOpponent[0][0] << std::endl;

    // Iterate through the field vector
    for (int i = 1; i <= fieldOpponent.size(); i++) {
        for (int j = 1; j <= fieldOpponent[i-1].size(); j++) {
            // Check if the value in the field is 1
            if (fieldOpponent[i-1][j-1] == 1) {
                std::cout << "inside the setFieldColor" << std::endl;
                // Retrieve the widget from the grid layout
                QWidget *widget = ui->gridLayout1->itemAtPosition(i-1, j)->widget();

                // Change the color or perform other actions on the widget
                if (widget) {
                    QPushButton *button = qobject_cast<QPushButton*>(widget);
                    if (button) {
                        button->setStyleSheet("background-color: red"); // Change to your desired color
                    }
                }
            }
        }
    }
}


void MainWindow::on_field1_156_clicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    // Get the position of the QPushButton
    int row, column, colspan, rowspan;
    ui->gridLayout2->getItemPosition(ui->gridLayout2->indexOf(clickedButton), &row, &column, &colspan, &rowspan);
    column--;
//    qDebug() << "Row:" << row << ", Column:" << column;
    fieldOpponent[row][column] = 1;
    client->sendMessage(fieldOpponent);
    fieldOpponent = client->receiveMessage();
    updateFieldOpponentColor();

}
