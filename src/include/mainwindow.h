#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

signals:
	/* General Settings */
	/* Engine */
	void sigEnableMorphing(bool isEnabled);
	void sigEnableRaytracing(bool isEnabled);
	void sigEnableParticleSystem(bool isEnabled);
	
	/* Camera */
	void sigChangeAspectRatio(int value);
	void sigChangeCurrentView(int value);
	void sigSetMaxSpeed(int value);
	void sigSetMaxAcceleration(int value);
	void sigSetFrictionMagnitude(int value);
	void sigCameraSpeed(int value);
	
	/* Scene Graph */
	
	
	/* Particles */
	void sigChangeNumberOfParticles(int value);
	void sigFreezeParticles(bool isEnabled);
    void sigFlameEquation(QString equation);
	
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
