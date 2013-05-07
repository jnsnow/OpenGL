#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

#include <QFileDialog>
#include <QtCore>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->userDefinedBox->setVisible(false);
    ui->tornadoParamsGroup->setVisible( false );

    connect(ui->numberOfParticlesSpinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigChangeNumberOfParticles(int)));
    connect(ui->freezeParticlesCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(sigFreezeParticles(bool)));
    connect(ui->morphingEnabler, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableMorphing(bool)));
    connect(ui->morphMatchingCheckbox, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableMorphMatching(bool)));

    //VR connections
    connect(ui->vrMorphControl, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableVRMorphControl(bool)));
    connect(ui->vrParticleControl, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableVRParticleControl(bool)));
    connect(ui->vrMorphSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigMorphPercentage(int)));
    connect(ui->vrParticleSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigChangeNumberOfParticles(int)));


    connect(ui->morphPercentageSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigMorphPercentage(int)));
    connect(ui->rayTracerEnabler, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableRaytracing(bool)));
    connect(ui->particleSystemEnabler, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableParticleSystem(bool)));

    connect(ui->WineBottleRadioButton, SIGNAL(clicked()),
            this, SIGNAL(sigMorphToWineBottle()));
    connect(ui->WhiskyBottleRadioButton, SIGNAL(clicked()),
            this, SIGNAL(sigMorphToWhiskyBottle()));
    connect(ui->maxAccelerationSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigSetMaxAcceleration(int)));
    connect(ui->frictionHorizontalSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigSetFrictionMagnitude(int)));
	connect(ui->speedHorizontalSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigCameraSpeed(int)));

    connect(ui->maxSpeedSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigSetMaxSpeed(int)));
    
    connect(ui->userDefinedShowButton, SIGNAL(clicked()),
            this, SIGNAL(sigUserDefVecParams(Parameters*)));

//    connect(ui->tornadoShowButton, SIGNAL(clicked()),
//            this, SIGNAL(sigTornadoVecParams(Parameters*)));

    //connect(ui->particleFieldFunctionComboBox, SIGNAL(currentIndexChanged(int)),
    //        this, SIGNAL(sigParticleFieldFunction(int)));

    /* Attempt to get the 'connect' work the other way */
    //connect(this, SIGNAL(sigMorphPercentageOut(int)),
    //        ui->morphPercentageSlider, SLOT(setValue(int)));

    ui->currentViewComboBox->addItem("Ortho");
    ui->currentViewComboBox->addItem("Ortho2D");
    ui->currentViewComboBox->addItem("Identity");
    ui->currentViewComboBox->addItem("Frustum");

    // Make sure that the Particle Parameters display default values at start.
    ui->minLifeInput->setPlainText( "9.0" );
    ui->maxLifeInput->setPlainText( "12.0" );
    ui->paramATextInput->setPlainText( "100.0" );
    ui->paramBTextInput->setPlainText( "100.0" );
    ui->paramCTextInput->setPlainText( "4.0" );
    ui->xFieldTextInput->setPlainText( "0.0" );
    ui->yFieldTextInput->setPlainText( "0.45" );
    ui->zFieldTextInput->setPlainText( "0.0" );
    ui->powerSlider->setSliderPosition( 10 );
    ui->scaleSlider->setSliderPosition( 1 );
    ui->rangeSlider->setSliderPosition( 24 );

    connect(ui->currentViewComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(sigChangeCurrentView(int)));
    capWebcam.open(0);

    if(capWebcam.isOpened() == false)
    {
        return;
    }

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    tmrTimer->start(20);
}

void MainWindow::setMorphPercentageOut(int pct)
{
    ui->morphPercentageSlider->setValue(pct);
    ui->vrMorphSlider->setValue(pct);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getDefaultNumOfParticles()
{
    return ui->numberOfParticlesSpinBox->value();
}

void MainWindow::on_addObjectButton_clicked()
{
    Dialog addObjectDialog;
    addObjectDialog.setModal(true);
    addObjectDialog.exec();
}

//void MainWindow::on_updateVectorFieldButton_clicked()
//{
//    std::string temp[3] = {ui->vxTextInput->toPlainText().toStdString(), ui->vyTextInput->toPlainText().toStdString(), ui->vzTextInput->toPlainText().toStdString()};
//    sigUpdateVectorField(temp);
//}

void MainWindow::on_particleFieldFunctionComboBox_currentIndexChanged(int index)
{
    // Clear the window
    ui->userDefinedBox->setVisible(false);
    ui->flameParamsGroup->setVisible(false);
    ui->tornadoParamsGroup->setVisible(false);

    // Show the appropriate widget box for each selection.
    // Index 0 = Flame function
    // Index 1 = Tornado function
    // index 2 = User defined function
    switch (index) {
        case 0:
            ui->flameParamsGroup->setVisible(true);
            break;
        case 1:
            ui->tornadoParamsGroup->setVisible(true);
            break;
        case 2:
            ui->userDefinedBox->setVisible(true);
            break;
    }
}


void MainWindow::on_flameShowButton_clicked()
{
    double vec3Pos[3] = { ui->xFieldTextInput->toPlainText().toDouble(),
                          ui->yFieldTextInput->toPlainText().toDouble(),
                          ui->zFieldTextInput->toPlainText().toDouble() };
    double scale = ui->scaleSlider->value() / 100.0;
    float power = ui->powerSlider->value() / 100.0;
    float range = ui->rangeSlider->value() / 100.0;

    sigFlameVecParams(vec3Pos, scale, power, range);
}

void MainWindow::on_flameDefaultButton_clicked()
{
    sigFlameVecParams();

    // Set UI to display the defaults
    ui->xFieldTextInput->setPlainText( "0.0" );
    ui->yFieldTextInput->setPlainText( "0.45" );
    ui->zFieldTextInput->setPlainText( "0.0" );
    ui->powerSlider->setSliderPosition( 10 );
    ui->scaleSlider->setSliderPosition( 1 );
    ui->rangeSlider->setSliderPosition( 24 );
}

void MainWindow::on_tornadoShowButton_clicked()
{
    float a = ui->paramATextInput->toPlainText().toDouble();
    float b = ui->paramBTextInput->toPlainText().toDouble();
    float c = ui->paramCTextInput->toPlainText().toDouble();

    sigTornadoVecParams( a, b, c );
}

void MainWindow::on_tornadoDefaultButton_clicked()
{
    sigTornadoVecParams();
    ui->paramATextInput->setPlainText( "100.0" );
    ui->paramBTextInput->setPlainText( "100.0" );
    ui->paramCTextInput->setPlainText( "4.0" );
}

void MainWindow::on_setLifespansButton_clicked()
{
    float min = ui->minLifeInput->toPlainText().toDouble();
    float max = ui->maxLifeInput->toPlainText().toDouble();

    sigSetParticleLife( min, max );
}

void MainWindow::on_defaultLifespansButton_clicked()
{
    sigSetParticleLife( 9.0, 12.0 );
    ui->minLifeInput->setPlainText( "9.0" );
    ui->maxLifeInput->setPlainText( "12.0" );
}

void MainWindow::processFrameAndUpdateGUI()
{
    capWebcam.read(matOriginal);

    if(matOriginal.empty() == true) return;

    cv::inRange(matOriginal, cv::Scalar(0,0,175), cv::Scalar(100,100,256), matProcessed);
    cv::GaussianBlur(matProcessed, matProcessed, cv::Size(9,9), 1.5);
    cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows / 4, 100, 50, 10, 400);

    for(itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++)
    {
        //ui->txtXYRadius->appendPlainText(QString("ball position x = ")  + QString::number((*itrCircles)[0]).rightJustified(4, ' ') +
        //        QString(", y = ") + QString::number((*itrCircles)[1]).rightJustified(4, ' ') +
        //        QString(", radius = ") + QString::number((*itrCircles)[2], 'f', 3).rightJustified(7, ' '));
        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), (int)(*itrCircles)[2], cv::Scalar(0, 0, 255), 3);
    }

    cv::cvtColor(matOriginal, matOriginal, CV_BGR2RGB);

    QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage qimgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_Indexed8);

    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
    ui->lblProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    if ( ui->audioOnCheckbox->isChecked() )
        sigMusicVolume( position );
}

void MainWindow::on_audioOnCheckbox_stateChanged(int arg1)
{
    if ( arg1 )
        sigMusicVolume( ui->volumeSlider->value() );
    else
        sigMusicVolume( 0 );
}
