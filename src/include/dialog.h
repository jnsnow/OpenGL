#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_objLocationButton_clicked();

public slots:

private:
    Ui::Dialog *ui;
    QTimer* tmrTimer;
};

#endif // DIALOG_H
