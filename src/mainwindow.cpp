#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtDebug>
#include <stdint.h>
#include <CAENVMElib.h>
#include <iostream>
#include <chrono>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Muon Experiment");
    //ui->groupBox->setStyleSheet("background-color : light-red");


    while (CAENVME_Init(cvV1718, 0, 0, &handleChef) != cvSuccess) {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Error",
                                                                tr("Impossible to connect to VME crate!\nPlease, check that the usb cable is connected. If it is connected, check if it is seen in /etc/usb/\nTry again ?"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            throw std::runtime_error("Failed to connect to VME crate");
        }
    }


    readBoardParameters(ui->spinBox_rotary_switches->value());

    readTDCParameters(ui->spinBox_rotary_switches_tdc->value());

    setPulserMode();



    //get current date
    QDate date = QDate::currentDate();
    QString dateString = date.toString();
    qDebug() << dateString.replace(" ", "_");
            //get current time
    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    qDebug() << timeString.replace(":", "_");

	 QString fileNameTmp = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    fileNameTmp += "Muon_Data_";
    fileNameTmp += dateString;
    fileNameTmp += "_at_";
    fileNameTmp += timeString;
    fileNameTmp += ".txt";
	 qDebug() << "This is the location: ";
	 qDebug() << fileNameTmp;
    ui->file_name_lineEdit->setText(fileNameTmp);

    qDebug() << "Reading setting mode";
    readTDCSetting();

    lcdNumber_voltages[0] = ui->lcdNumber_channel_0_voltage;
    lcdNumber_voltages[1] = ui->lcdNumber_channel_1_voltage;
    lcdNumber_voltages[2] = ui->lcdNumber_channel_2_voltage;
    lcdNumber_voltages[3] = ui->lcdNumber_channel_3_voltage;
    lcdNumber_voltages[4] = ui->lcdNumber_channel_4_voltage;
    lcdNumber_voltages[5] = ui->lcdNumber_channel_5_voltage;

    lcdNumber_currents[0] = ui->lcdNumber_channel_0_current;
    lcdNumber_currents[1] = ui->lcdNumber_channel_1_current;
    lcdNumber_currents[2] = ui->lcdNumber_channel_2_current;
    lcdNumber_currents[3] = ui->lcdNumber_channel_3_current;
    lcdNumber_currents[4] = ui->lcdNumber_channel_4_current;
    lcdNumber_currents[5] = ui->lcdNumber_channel_5_current;

    label_state_leds[0] = ui->label_channel_0_state_led;
    label_state_leds[1] = ui->label_channel_1_state_led;
    label_state_leds[2] = ui->label_channel_2_state_led;
    label_state_leds[3] = ui->label_channel_3_state_led;
    label_state_leds[4] = ui->label_channel_4_state_led;
    label_state_leds[5] = ui->label_channel_5_state_led;

    label_ramp_up_leds[0] = ui->label_channel_0_ramp_up_led;
    label_ramp_up_leds[1] = ui->label_channel_1_ramp_up_led;
    label_ramp_up_leds[2] = ui->label_channel_2_ramp_up_led;
    label_ramp_up_leds[3] = ui->label_channel_3_ramp_up_led;
    label_ramp_up_leds[4] = ui->label_channel_4_ramp_up_led;
    label_ramp_up_leds[5] = ui->label_channel_5_ramp_up_led;

    label_ramp_down_leds[0] = ui->label_channel_0_ramp_down_led;
    label_ramp_down_leds[1] = ui->label_channel_1_ramp_down_led;
    label_ramp_down_leds[2] = ui->label_channel_2_ramp_down_led;
    label_ramp_down_leds[3] = ui->label_channel_3_ramp_down_led;
    label_ramp_down_leds[4] = ui->label_channel_4_ramp_down_led;
    label_ramp_down_leds[5] = ui->label_channel_5_ramp_down_led;

    label_over_current_leds[0] = ui->label_channel_0_over_current_led;
    label_over_current_leds[1] = ui->label_channel_1_over_current_led;
    label_over_current_leds[2] = ui->label_channel_2_over_current_led;
    label_over_current_leds[3] = ui->label_channel_3_over_current_led;
    label_over_current_leds[4] = ui->label_channel_4_over_current_led;
    label_over_current_leds[5] = ui->label_channel_5_over_current_led;

    label_over_voltage_leds[0] = ui->label_channel_0_over_voltage_led;
    label_over_voltage_leds[1] = ui->label_channel_1_over_voltage_led;
    label_over_voltage_leds[2] = ui->label_channel_2_over_voltage_led;
    label_over_voltage_leds[3] = ui->label_channel_3_over_voltage_led;
    label_over_voltage_leds[4] = ui->label_channel_4_over_voltage_led;
    label_over_voltage_leds[5] = ui->label_channel_5_over_voltage_led;

    label_under_voltage_leds[0] = ui->label_channel_0_under_voltage_led;
    label_under_voltage_leds[1] = ui->label_channel_1_under_voltage_led;
    label_under_voltage_leds[2] = ui->label_channel_2_under_voltage_led;
    label_under_voltage_leds[3] = ui->label_channel_3_under_voltage_led;
    label_under_voltage_leds[4] = ui->label_channel_4_under_voltage_led;
    label_under_voltage_leds[5] = ui->label_channel_5_under_voltage_led;

    label_maxv_leds[0] = ui->label_channel_0_maxv_led;
    label_maxv_leds[1] = ui->label_channel_1_maxv_led;
    label_maxv_leds[2] = ui->label_channel_2_maxv_led;
    label_maxv_leds[3] = ui->label_channel_3_maxv_led;
    label_maxv_leds[4] = ui->label_channel_4_maxv_led;
    label_maxv_leds[5] = ui->label_channel_5_maxv_led;

    label_maxi_leds[0] = ui->label_channel_0_maxi_led;
    label_maxi_leds[1] = ui->label_channel_1_maxi_led;
    label_maxi_leds[2] = ui->label_channel_2_maxi_led;
    label_maxi_leds[3] = ui->label_channel_3_maxi_led;
    label_maxi_leds[4] = ui->label_channel_4_maxi_led;
    label_maxi_leds[5] = ui->label_channel_5_maxi_led;

    label_trip_leds[0] = ui->label_channel_0_trip_led;
    label_trip_leds[1] = ui->label_channel_1_trip_led;
    label_trip_leds[2] = ui->label_channel_2_trip_led;
    label_trip_leds[3] = ui->label_channel_3_trip_led;
    label_trip_leds[4] = ui->label_channel_4_trip_led;
    label_trip_leds[5] = ui->label_channel_5_trip_led;

    label_over_power_leds[0] = ui->label_channel_0_over_power_led;
    label_over_power_leds[1] = ui->label_channel_1_over_power_led;
    label_over_power_leds[2] = ui->label_channel_2_over_power_led;
    label_over_power_leds[3] = ui->label_channel_3_over_power_led;
    label_over_power_leds[4] = ui->label_channel_4_over_power_led;
    label_over_power_leds[5] = ui->label_channel_5_over_power_led;

    label_disabled_leds[0] = ui->label_channel_0_disabled_led;
    label_disabled_leds[1] = ui->label_channel_1_disabled_led;
    label_disabled_leds[2] = ui->label_channel_2_disabled_led;
    label_disabled_leds[3] = ui->label_channel_3_disabled_led;
    label_disabled_leds[4] = ui->label_channel_4_disabled_led;
    label_disabled_leds[5] = ui->label_channel_5_disabled_led;

    label_interlock_leds[0] = ui->label_channel_0_interlock_led;
    label_interlock_leds[1] = ui->label_channel_1_interlock_led;
    label_interlock_leds[2] = ui->label_channel_2_interlock_led;
    label_interlock_leds[3] = ui->label_channel_3_interlock_led;
    label_interlock_leds[4] = ui->label_channel_4_interlock_led;
    label_interlock_leds[5] = ui->label_channel_5_interlock_led;


    // change color of lcd number for board parameters
    ui->lcdNumber_board_vmax->setPalette(Qt::black);
    ui->lcdNumber_board_imax->setPalette(Qt::black);

    // change color of lcd number for channels parameters
    for (int i=0; i<6; ++i) {
        lcdNumber_voltages[i]->setPalette(Qt::red);
        lcdNumber_currents[i]->setPalette(Qt::red);
    }

    ui->scaler_ch0_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch1_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch2_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch3_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch4_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch5_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch6_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch7_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch8_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch9_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch10_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch11_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch12_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch13_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch14_lcdNumber->setPalette(Qt::black);
    ui->scaler_ch15_lcdNumber->setPalette(Qt::black);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readHV()));
    readHV();
    timer->start(1000); //time specified in ms


    // configure TDC plots

    /*
    maxOfEntries = 1;
    tMin = ui->doubleSpinBox_tmin->value();
    tMax = ui->doubleSpinBox_tmax->value();
    nBins = ui->spinBox_nbins->value();
    for (int i=0; i<nBins; ++i)
    {
      x.push_back( tMin + i * tMax / nBins + 0.5 * (tMax - tMin) / nBins );
      y.push_back(0);
    }


    bars = new QCPBars(ui->widget_qcp->xAxis, ui->widget_qcp->yAxis);
    ui->widget_qcp->addPlottable(bars);
    bars->setWidth(5.0/x.size());
    bars->setData(x, y);
    //bars->setPen(Qt::NoPen);
    //bars->setBrush(QColor(10, 140, 70, 160));

    ui->widget_qcp->rescaleAxes();
    ui->widget_qcp->xAxis->setLabel("Time (ns)");
    ui->widget_qcp->yAxis->setLabel("# entries");
    ui->widget_qcp->xAxis->setRange(tMin*0.99, tMax*1.01);
    ui->widget_qcp->yAxis->setRange(0, 10);
    */
    isLog = false;
    stopRun = false;
    running = false;


    /*
    maxOfEntriesPoisson = 1;
    nBinsPoisson = 100;
    cMin = ui->doubleSpinBox_cmin->value();
    cMax = ui->doubleSpinBox_cmax->value();
    nBins = ui->spinBox_nbins->value();
    for (int i=0; i<nBinsPoisson; ++i)
    {
      xPoisson.push_back( cMin + i * cMax / nBinsPoisson + 0.5 * (cMax - cMin) / nBinsPoisson );
      yPoisson.push_back(0);
    }

    poissonDistr = new QCPBars(ui->poisson_widgets->xAxis, ui->poisson_widgets->yAxis);
    ui->poisson_widgets->addPlottable(poissonDistr);
    poissonDistr->setWidth(5.0/xPoisson.size());
    poissonDistr->setData(xPoisson, yPoisson);
    ui->poisson_widgets->rescaleAxes();
    ui->poisson_widgets->xAxis->setLabel("Count");
    ui->poisson_widgets->yAxis->setLabel("# entries");
    ui->poisson_widgets->xAxis->setRange(cMin*0.99, cMax*1.01);
    ui->poisson_widgets->yAxis->setRange(0, 10);
    */

    ui->total_lcdNumber->display(0);
    ui->rate_lcdNumber->display(0);
    ui->rms_arrival_lcdNumber->display(0);
    ui->mean_arrival_lcdNumber->display(0);

    ui->mean_lifetime_lcdNumber->display(0);
    ui->rms_lifetime_lcdNumber->display(0);
    ui->last_lifetime_lcdNumber->display(0);

    ui->total_lcdNumber->setPalette(Qt::black);
    ui->rate_lcdNumber->setPalette(Qt::black);
    ui->mean_arrival_lcdNumber->setPalette(Qt::black);
    ui->rms_arrival_lcdNumber->setPalette(Qt::black);

    ui->mean_lifetime_lcdNumber->setPalette(Qt::black);
    ui->rms_lifetime_lcdNumber->setPalette(Qt::black);
    ui->last_lifetime_lcdNumber->setPalette(Qt::black);

    if (!running) ui->pushButton_stop_run->setEnabled(false);
    ui->widget_bar_config->setEnabled(true);
    ui->tdc_start_spinBox->setEnabled(true);
    ui->tdc_stop_spinBox->setEnabled(true);



    // configure global view
    this->setCentralWidget(ui->tabWidget);
    ui->tabWidget->setCurrentIndex(0);

    this->setFixedSize(this->size());
    on_reset_scaler_pushButton_clicked();


    hPoisson = new histogram(ui->qcp_Poisson, "Poisson Distribution", "count", "# events");
    hPoisson->adjustPlot(ui->spinBox_nbins_poisson->value(), ui->doubleSpinBox_cmin->value(), ui->doubleSpinBox_cmax->value());
    nExp = 100;

    hTDC = new histogram(ui->qcp_TDC, "TDC Delta t", "time (ns)", "# events");
    hTDC->adjustPlot(ui->spinBox_nbins->value(), ui->doubleSpinBox_tmin->value(), ui->doubleSpinBox_tmax->value());
}

MainWindow::~MainWindow()
{
    qDebug() << "Closing connection with VME crate";
    CAENVME_End(handleChef);
    delete ui;
}

void MainWindow::closing() {
    qDebug() << "About to quit";
    if (running) stopRun = true;
    usleep(100);
    qApp->quit();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                                                                tr("Are you sure you want to exit ?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        closing();
        event->accept();
    }
}
bool MainWindow::readTDCParameters(int baseAddress)
{
    baseAddressTDC = baseAddress << 16;
    qDebug() << "New tdc address";
    uint16_t value16;
    // reading firmware revision to make sure rotary switch is ok
    CVErrorCodes ret = CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1026, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Please check the rotary switches for the tdc.";
        QMessageBox::information(this, "Error", "Please check the rotary switches for the TDC.");
        return false;
    }
    return true;

}


void MainWindow::setPulserMode()
{
    CAENVME_SetPulserConf(handleChef, cvPulserA, 3, 2, cvUnit104ms, 1, cvManualSW, cvManualSW);
    CAENVME_SetOutputConf(handleChef, cvOutput0, cvInverted, cvActiveLow, cvMiscSignals);
}

void MainWindow::readBoardParameters(int baseAddress)
{
    baseAddressHV = baseAddress << 16;

    uint16_t value16;
    // read board parameters
    QString model;
    CAENVME_ReadCycle(handleChef, baseAddressHV + 0x8116, &value16, cvA32_U_DATA, cvD16);
    model += QString((char) (value16 & 0xff));
    model += QString((char) ((value16 >> 8) & 0xff));
    CAENVME_ReadCycle(handleChef, baseAddressHV + 0x8118, &value16, cvA32_U_DATA, cvD16);
    model += QString((char) (value16 & 0xff));
    model += QString((char) ((value16 >> 8) & 0xff));
    CAENVME_ReadCycle(handleChef, baseAddressHV + 0x811A, &value16, cvA32_U_DATA, cvD16);
    model += QString((char) (value16 & 0xff));
    model += QString((char) ((value16 >> 8) & 0xff));

    if (!model.startsWith("V653")){
        qDebug() << "Wrong board";
        ui->lineEdit_board_model->setText("UNKNOWN");
        HVenabled = false;
        ui->widget_configuration->setEnabled(false);
        ui->lcdNumber_board_vmax->display(888888);
        ui->lcdNumber_board_imax->display(888888);
    }
    else {
        ui->lineEdit_board_model->setText(model);
        HVenabled = true;
        ui->widget_configuration->setEnabled(true);
        CAENVME_ReadCycle(handleChef, baseAddressHV + 0x0050, &value16, cvA32_U_DATA, cvD16);
        ui->lcdNumber_board_vmax->display(value16);
        CAENVME_ReadCycle(handleChef, baseAddressHV + 0x0054, &value16, cvA32_U_DATA, cvD16);
        ui->lcdNumber_board_imax->display(value16);
    }

}

void MainWindow::readHV()
{
    uint16_t value16;
    if (!HVenabled) return;
    bool hvOn = false;
    // read channels parameters
    for (int i=0; i<6; ++i) {
        CAENVME_ReadCycle(handleChef, baseAddressHV + 0x80 * i + 0x88, &value16, cvA32_U_DATA, cvD16);
        lcdNumber_voltages[i]->display(value16*0.1);
        CAENVME_ReadCycle(handleChef, baseAddressHV + 0x80 * i + 0x8C, &value16, cvA32_U_DATA, cvD16);
        lcdNumber_currents[i]->display(value16*0.05);
        CAENVME_ReadCycle(handleChef, baseAddressHV + 0x80 * i + 0x94, &value16, cvA32_U_DATA, cvD16);
        label_state_leds[i]->setPixmap(value16 & (0x1 << 0) ? QPixmap(":/green.png") : QPixmap(":/lightgreen.png"));
        hvOn = hvOn || (value16 & (0x1 << 0));
        label_ramp_up_leds[i]->setPixmap(value16 & (0x1 << 1) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
        label_ramp_down_leds[i]->setPixmap(value16 & (0x1 << 2) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
        label_over_current_leds[i]->setPixmap(value16 & (0x1 << 3) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
        label_over_voltage_leds[i]->setPixmap(value16 & (0x1 << 4) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
        label_under_voltage_leds[i]->setPixmap(value16 & (0x1 << 5) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
        label_maxv_leds[i]->setPixmap(value16 & (0x1 << 6) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
        label_maxi_leds[i]->setPixmap(value16 & (0x1 << 7) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
        label_trip_leds[i]->setPixmap(value16 & (0x1 << 8) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
        label_over_power_leds[i]->setPixmap(value16 & (0x1 << 9) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
        label_disabled_leds[i]->setPixmap(value16 & (0x1 << 11) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
        label_interlock_leds[i]->setPixmap(value16 & (0x1 << 12) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    }

    ui->label_high_voltage->setEnabled(hvOn);
    CAENVME_ReadCycle(handleChef, baseAddressHV + 0x0050, &value16, cvA32_U_DATA, cvD16);
    ui->lcdNumber_board_vmax->display(value16);
    CAENVME_ReadCycle(handleChef, baseAddressHV + 0x0054, &value16, cvA32_U_DATA, cvD16);
    ui->lcdNumber_board_imax->display(value16);

}

void MainWindow::on_radioButton_on_off_clicked()
{
    if (ui->radioButton_on_off->isChecked()) {
        ui->radioButton_on_off->setText("ON");
        ui->label_state_led->setPixmap(QPixmap(":/green.png"));
    }
    else {
        ui->radioButton_on_off->setText("OFF");
        ui->label_state_led->setPixmap(QPixmap(":/grey.png"));
    }
}

void MainWindow::on_pushButton_set_clicked()
{

    int channel, state, voltage, current, tripTime, vMax, rampDown, rampUp;
    channel = ui->spinBox_channel->value();
    state = ui->radioButton_on_off->isChecked();
    voltage = ui->spinBox_voltage->value();
    current = ui->spinBox_current->value();
    tripTime = ui->spinBox_trip_time->value();
    vMax = ui->spinBox_vmax->value();
    rampDown = ui->spinBox_ramp_down->value();
    rampUp = ui->spinBox_ramp_up->value();


    if (channel == -1) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to set all the channels ?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }

    if (channel < -1 || channel > 5) {
         qDebug() << "Impossible channel";
    }
    else {
        int start = channel;
        int end = channel+1;
        if (channel == -1) {
            start = 0;
            end = 6;
        }
        for (int i=start; i<end; ++i) {
            uint16_t value16;
            value16 = state ? 0x1 : 0x0;
            CAENVME_WriteCycle(handleChef, baseAddressHV + 0x80 * i + 0x90, &value16, cvA32_U_DATA, cvD16);

            value16 = voltage * 10;
            CAENVME_WriteCycle(handleChef, baseAddressHV + 0x80 * i + 0x80, &value16, cvA32_U_DATA, cvD16);

            value16 = current * 20;
            CAENVME_WriteCycle(handleChef, baseAddressHV + 0x80 * i + 0x84, &value16, cvA32_U_DATA, cvD16);

            value16 = tripTime * 10;
            CAENVME_WriteCycle(handleChef, baseAddressHV + 0x80 * i + 0x98, &value16, cvA32_U_DATA, cvD16);

            value16 = vMax * 10;
            CAENVME_WriteCycle(handleChef, baseAddressHV + 0x80 * i + 0x9C, &value16, cvA32_U_DATA, cvD16);

            value16 = rampDown;
            CAENVME_WriteCycle(handleChef, baseAddressHV + 0x80 * i + 0xA0, &value16, cvA32_U_DATA, cvD16);

            value16 = rampUp;
            CAENVME_WriteCycle(handleChef, baseAddressHV + 0x80 * i + 0xA4, &value16, cvA32_U_DATA, cvD16);
        }
    }

}

void MainWindow::on_spinBox_channel_valueChanged(int arg1)
{
    //QMessageBox msgBox;
    //msgBox.setText("The channel " + QString::number(arg1) + "must be read.");
    //msgBox.exec();
    qDebug() << QString::number(arg1);
}

void MainWindow::on_spinBox_rotary_switches_valueChanged(int baseAddress)
{
    readBoardParameters(baseAddress);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    //qDebug() << QString::number(index);
    if (index == 1) {
        timer->stop();
    }
    else if (index == 2) {
        timer->stop();
    }
    else if (index == 0) {
        timer->start(1000);
    }
}

void MainWindow::on_pushButton_start_run_clicked()
{
    if (running) return;

    if (!readTDCParameters(ui->spinBox_rotary_switches_tdc->value())) {
		  qDebug() << "Could not read TDC parameter";
        return;
    }

    // configure TDC in trigger mode
    if (!setTriggerMode()) {
        qDebug() << "Please check the tdc trigger configuration";

        QMessageBox::information(this, "Error", "Please check the tdc trigger configuration.");

        return;
    }
	 else {
		  readTDCSetting();
	 }

    //get current date
    QDate date = QDate::currentDate();
    QString dateString = date.toString();
    qDebug() << dateString.replace(" ", "_");
    //get current time
    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    qDebug() << timeString.replace(":", "_");


    counter = 0;
    prevTime = time;
    mean_arrival = 0;
    mean_arrival_square = 0;
    mean_lifetime = 0;
    mean_lifetime_square = 0;

    dataFileName = ui->file_name_lineEdit->text();
    QFile file(dataFileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
        qDebug() << "Problem opening or creating the file:";
		  qDebug() << dataFileName;
    }
    else {
        QTextStream stream(&file);

        tdcStartCh = ui->tdc_start_spinBox->value();
        tdcStopCh = ui->tdc_stop_spinBox->value();

        ui->widget_bar_config->setEnabled(false);
        ui->tdc_trigger_widget->setEnabled(false);
        ui->pushButton_start_run->setEnabled(false);
        ui->pushButton_stop_run->setEnabled(true);
        ui->tdc_channel_widget->setEnabled(false);
        ui->spinBox_rotary_switches_tdc->setEnabled(false);

        uint16_t value16;
        uint32_t value32;
        std::vector<uint32_t> differences(100);
        qDebug() << "Reading Output Buffer Register";
        running = true;
        stopRun = false;

        do {
            QCoreApplication::processEvents();
            //std::cout << "Reading Status Register" << std::endl;
            CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1002, &value16, cvA32_U_DATA, cvD16);

            //std::cout << "Any Data Stored: " << (value16 & 0x1) << std::endl;
            //std::cout << "Almost full ? : " << ((value16 >> 1) & 0x1) << std::endl;

            if (value16 & 0x1) {
                //std::cout << "Reading Output Buffer Register" << std::endl;
                CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x0000, &value32, cvA32_U_DATA, cvD32);
                //std::cout << "Data: 0x" << std::hex << ((value32 >> 27) & 0x1F) << std::endl;
                //std::cout << "Trailing ?: 0x" << std::hex << ((value32 >> 26) & 0x1) << std::endl;
                //std::cout << "Channel: 0x" << std::hex << ((value32 >> 21) & 0x1F) << std::endl;
                //std::cout << "TDC Meas: " << std::dec << (value32 & 0x1FFFFF) << std::endl;
                int header = ((value32 >> 27) & 0x1F);
                if (header == 0x8) { // Global header
                    CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x0000, &value32, cvA32_U_DATA, cvD32);
                    header = ((value32 >> 27) & 0x1F);
                    if (header == 0x1) { // TDC header
                        int nChannel = 0;
                        int tdcTimes[16];
                        QString output = "";
                        while (true) {
                            CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x0000, &value32, cvA32_U_DATA, cvD32);
                            header = ((value32 >> 27) & 0x1F);
                            if (header != 0x0) {
                                break; // if not TDC measurement
                            }
                            int channel = ((value32 >> 21) & 0x1F);
                            int tdcTime = (value32 & 0x1FFFFF) * 25;
                            output += QString::number(channel) + " " + QString::number(tdcTime) + "\n";
                            if (channel == tdcStartCh || channel == tdcStopCh) {
                                tdcTimes[channel] = tdcTime;
                                nChannel++;
                            }
                        }
                        if (nChannel == 2) {// 2 for arch exp)
                            qDebug() << output;
                            output += "\n";
                            double difference = (tdcTimes[tdcStopCh] - tdcTimes[tdcStartCh])/1000.0;
									 if (difference < 0) {
									 	difference += 52429.500;
									 }
                            output = QString::number(difference) + "\n";
                            stream << output;
                            qDebug() << "difference: " << QString::number(difference);
                            differences.push_back(difference);
                            if (difference <= hTDC->getXMax() && difference >= hTDC->getXMin()) {
                                // total number of events
                                counter++;
                                ui->total_lcdNumber->display(counter);

                                // rate estimate
                                QTime currentTime = QTime::currentTime();
                                int timeFromBeginingOfRun = time.msecsTo(currentTime);
                                ui->rate_lcdNumber->display((int)((counter*1000.0/(timeFromBeginingOfRun))*1000)/1000.0);

                                // mean arrival estimate
                                int arrivalTime = prevTime.msecsTo(currentTime);
                                mean_arrival *= (counter-1);
                                mean_arrival_square *= (counter-1);
                                mean_arrival += arrivalTime;
                                mean_arrival_square += arrivalTime*arrivalTime;
                                mean_arrival /= counter;
                                mean_arrival_square /= counter;
                                ui->mean_arrival_lcdNumber->display((int)(mean_arrival)/1000.0);
                                ui->rms_arrival_lcdNumber->display((int)sqrt(mean_arrival_square - mean_arrival*mean_arrival)/1000.0);

                                // lifetime
                                mean_lifetime *= (counter - 1);
                                mean_lifetime_square *= (counter - 1);
                                mean_lifetime += (difference / 1000.0);
                                mean_lifetime_square += (difference / 1000.0) * (difference / 1000.0);
                                mean_lifetime /= counter;
                                mean_lifetime_square /= counter;
                                ui->mean_lifetime_lcdNumber->display((int)(mean_lifetime*1000)/1000.0);
                                ui->rms_lifetime_lcdNumber->display((int)(sqrt(mean_lifetime_square - mean_lifetime*mean_lifetime)*1000)/1000.0);
                                ui->last_lifetime_lcdNumber->display((int)(difference)/1000.0);

                                // plot
                                hTDC->updatePlot(difference);
                                prevTime = currentTime;
                            }
                        }
                        else {
                            qDebug() << "No good events, n channel = " << QString::number(nChannel);
                        }
                    }

                }
            }
            usleep(1);
        } while (!stopRun);
        qDebug() << "Run has been stoped";
        file.close();
        ui->widget_bar_config->setEnabled(true);
        ui->tdc_trigger_widget->setEnabled(true);
        ui->tdc_channel_widget->setEnabled(true);


        stopRun = false;
        running = false;
        ui->pushButton_start_run->setEnabled(true);
        ui->spinBox_rotary_switches_tdc->setEnabled(true);
        ui->pushButton_stop_run->setEnabled(false);

    }
}

void MainWindow::on_spinBox_nbins_valueChanged(int nBins_)
{
    hTDC->adjustPlot(nBins_, ui->doubleSpinBox_tmin->value(), ui->doubleSpinBox_tmax->value());
}

void MainWindow::on_doubleSpinBox_tmax_valueChanged(double tMax_)
{
    double tMin_ = ui->doubleSpinBox_tmin->value();
    if (tMax_ <= tMin_) return;
    hTDC->adjustPlot(ui->spinBox_nbins->value(), tMin_, tMax_);
}

void MainWindow::on_doubleSpinBox_tmin_valueChanged(double tMin_)
{
    double tMax_ = ui->doubleSpinBox_tmax->value();
    if (tMin_ >= tMax_) return;
    hTDC->adjustPlot(ui->spinBox_nbins->value(), tMin_, tMax_);
}

void MainWindow::on_spinBox_nbins_poisson_valueChanged(int nBins_)
{
    hPoisson->adjustPlot(nBins_, ui->doubleSpinBox_cmin->value(), ui->doubleSpinBox_cmax->value());
}

void MainWindow::on_doubleSpinBox_cmax_valueChanged(double cMax_)
{
    double cMin_ = ui->doubleSpinBox_cmin->value();
    if (cMax_ <= cMin_) return;
    hPoisson->adjustPlot(ui->spinBox_nbins_poisson->value(), cMin_, cMax_);
}

void MainWindow::on_doubleSpinBox_cmin_valueChanged(double cMin_)
{
    double cMax_ = ui->doubleSpinBox_cmax->value();
    if (cMin_ >= cMax_) return;
    hPoisson->adjustPlot(ui->spinBox_nbins_poisson->value(), cMin_, cMax_);
}


/*
void MainWindow::adjustPlot()
{
    for (int i=0; i<nBins; ++i) {
        x[i] = tMin + i * (tMax - tMin) / nBins + 0.5 * (tMax - tMin) / nBins;
        y[i] = 0;
    }
    maxOfEntries = 1;
    bars->clearData();
    bars->setData(x, y);
    ui->widget_qcp->rescaleAxes();
    ui->widget_qcp->xAxis->setRange(tMin*0.99, tMax*1.01);
    ui->widget_qcp->yAxis->setRange(0, 10);
    ui->widget_qcp->replot();
}

void MainWindow::updatePlot(double value)
{
    int index = (value - tMin) * nBins / (tMax - tMin);
    if (index >= nBins || index < 0) return;

    y[index]++;
    if (y[index] > maxOfEntries) maxOfEntries = y[index];
    bars->clearData();
    bars->setData(x, y);
    ui->widget_qcp->yAxis->setRange(0, 10 + 1.3*maxOfEntries);
    ui->widget_qcp->replot();
}


void MainWindow::updatePoissonPlot(double value)
{
    int index = (value - cMin) * nBinsPoisson / (cMax - cMin);
    if (index >= nBinsPoisson || index < 0) return;

    yPoisson[index]++;
    if (yPoisson[index] > maxOfEntriesPoisson) maxOfEntriesPoisson = yPoisson[index];
    poissonDistr->clearData();
    poissonDistr->setData(xPoisson, yPoisson);
    ui->poisson_widgets->yAxis->setRange(0, 10 + 1.3*maxOfEntriesPoisson);
    ui->poisson_widgets->replot();
}
*/
void MainWindow::on_pushButton_stop_run_clicked()
{
    if (running) stopRun = true;
    else stopRun = false;
}

void MainWindow::on_pushButton_clear_bars_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Clear data", "Are you sure you want to clear the data ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";
        ui->total_lcdNumber->display(0);
        ui->rate_lcdNumber->display(0);
        ui->rms_arrival_lcdNumber->display(0);
        ui->mean_arrival_lcdNumber->display(0);
        ui->last_lifetime_lcdNumber->display(0);
        ui->rms_lifetime_lcdNumber->display(0);
        ui->mean_lifetime_lcdNumber->display(0);

        counter = 0;
        prevTime = QTime::currentTime();
        mean_arrival = 0;
        mean_arrival_square = 0;
        mean_lifetime = 0;
        mean_lifetime_square = 0;

        hTDC->adjustPlot(ui->spinBox_nbins->value(), ui->doubleSpinBox_tmin->value(), ui->doubleSpinBox_tmax->value());
    }
    else {
        qDebug() << "Yes was *not* clicked";
        return;
    }
}

void MainWindow::waitForWriteOK()
{
    uint16_t value16;
    bool WRITE_OK = false;

    do {
        CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1030, &value16, cvA32_U_DATA, cvD16);
        WRITE_OK = ((value16 >> 0) & 0x1);
        usleep(10);
    } while (!WRITE_OK);
    return;

}

void MainWindow::waitForReadOK()
{
    uint16_t value16;
    bool READ_OK = false;

    do {
        CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1030, &value16, cvA32_U_DATA, cvD16);
        READ_OK = ((value16 >> 1) & 0x1);
        usleep(10);
    } while (!READ_OK);
    return;

}



bool MainWindow::setTriggerMode() {
    uint16_t value16;
    CAENVME_API err;

    uint16_t tdcWinWidth = ui->window_width_spinBox->value();
    int16_t tdcWinOffset = ui->window_offset_spinBox->value();

    if (tdcWinOffset + tdcWinWidth < 0) { //1st case: the trigger matching window precedes the trigger arrival
        if (-tdcWinOffset > 102375 || tdcWinOffset == 0) return false;
    }
    else if (tdcWinOffset + tdcWinWidth > 1000) { // 2nd case: the trigger matching is straddling the trigger or delayed with respect to the trigger
        return false;
    }

    // set trigger mode
    waitForWriteOK();
    value16 = 0x0000;
    err = CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (err != cvSuccess) {
        qDebug() << "Oups";
        qDebug() << QString::number(err);
    }

    // set window width
    waitForWriteOK();
    value16 = 0x1000; // OPCODE 10XX set window width
    err = CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    //value16 = 0x0016; // 0.55 us window width value (for the arch exp)
    value16 = tdcWinWidth / 25;
    //value16 = 400; // 10 us window width value (for the lifetime exp)
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x1100; // OPCODE 11XX set window offset
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    //value16 = 0xFFEC; // -0.5 us offset value (for the arch exp)
    value16 = tdcWinOffset / 25; // cast to unsigned
    //value16 = 0xFE6E; // -10.05 us offset value (for the lifetime exp)
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x1200; // OPCODE 12XX set extra search margin
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x0004; // 0.1 us extra search margin value
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x1300; // OPCODE 13XX set reject margin
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x0001; // 0.025 us reject margin value
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    return true;
}

void MainWindow::readTDCSetting() {
    // ask to read trigger conf
    uint16_t value16;

    bool READ_OK = false;

    waitForWriteOK();
    value16 = 0x1600;
    qDebug() << "Writing";
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);

    // read 1/5 words
    do {
        CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1030, &value16, cvA32_U_DATA, cvD16);
        READ_OK = ((value16 >> 1) & 0x1);
        usleep(1);
        //nanosleep(&tim, NULL);
    } while (!READ_OK);

    CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);
    qDebug() << "1/5: 0x" + QString("%1").arg((value16 & 0xfff) , 0, 16) + " (expect 0x0016)";

    // read 2/5 words
    do {
        CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1030, &value16, cvA32_U_DATA, cvD16);
        READ_OK = ((value16 >> 1) & 0x1);
        usleep(1);
        //nanosleep(&tim, NULL);
    } while (!READ_OK);

    CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);
    qDebug() << "2/5: 0x" + QString("%1").arg((value16 & 0xffff) , 0, 16) + " (expect 0xFFEC)";
    // read 3/5 words
    do {
        CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1030, &value16, cvA32_U_DATA, cvD16);
        READ_OK = ((value16 >> 1) & 0x1);
        usleep(1);
        //nanosleep(&tim, NULL);
    } while (!READ_OK);

    CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);
    qDebug() << "3/5: 0x" + QString("%1").arg((value16 & 0xfff) , 0, 16) + " (expect 0x0004)";
    // read 4/5 words
    do {
        CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1030, &value16, cvA32_U_DATA, cvD16);
        READ_OK = ((value16 >> 1) & 0x1);
        usleep(1);
        //nanosleep(&tim, NULL);
    } while (!READ_OK);

    CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);
    qDebug() << "4/5: 0x" + QString("%1").arg((value16 & 0xfff) , 0, 16) + " (expect 0x0002)";

    // read 5/5 words
    do {
        CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x1030, &value16, cvA32_U_DATA, cvD16);
        READ_OK = ((value16 >> 1) & 0x1);
        usleep(1);
        //nanosleep(&tim, NULL);
    } while (!READ_OK);

    CAENVME_ReadCycle(handleChef, baseAddressTDC + 0x102E, &value16, cvA32_U_DATA, cvD16);
    qDebug() << "5/5: 0x" + QString("%1").arg((value16 & 0x1) , 0, 16) + " (expect 0x0)";


}

void MainWindow::on_pushButton_clear_tdc_clicked()
{
    uint16_t value16 = 0x0000;
    CAENVME_WriteCycle(handleChef, baseAddressTDC + 0x1016, &value16, cvA32_U_DATA, cvD16);
}

void MainWindow::on_log_pushButton_clicked()
{
    bool isLogY = hTDC->setLogY();

    if (isLogY) {
        ui->log_pushButton->setText("Linear Scale");
    }
    else {
        ui->log_pushButton->setText("Log Scale");
    }
}



void MainWindow::on_file_name_pushButton_clicked()
{
	 dataFileName = ui->file_name_lineEdit->text();
	 QString newDataFileName =   QFileDialog::getSaveFileName(this, tr("Save File"),
                                QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory)
                                );
	 ui->file_name_lineEdit->setText(newDataFileName == "" ? dataFileName : newDataFileName);
}

void MainWindow::on_pulse_pushButton_clicked()
{
    ui->pulse_pushButton->setEnabled(false);
    unsigned char period;
    double truePeriod = 0; // in ms
    unsigned char width;
    double trueWidth = 0; // in ms
    CVTimeUnits unit;
    unsigned char pulseNo;
    CVIOSources start, reset;
    CAENVME_GetPulserConf(handleChef, cvPulserA, &period, &width, &unit, &pulseNo, &start, &reset);

    switch (unit) {
        case cvUnit25ns:
            truePeriod = (int)(period) * 0.000025;
            trueWidth = (int)(width) * 0.000025;
            break;
        case cvUnit104ms:
            truePeriod = (int)(period) * 104;
            trueWidth = (int)(width) * 104;
            break;
        case cvUnit410us:
            truePeriod = (int)(period) * 0.41;
            trueWidth = (int)(width) * 0.41;
            break;
        case cvUnit1600ns:
            truePeriod = (int)(period) * 0.0016;
            trueWidth = (int)(width) * 0.0016;
            break;
    }

    qDebug() << "Period: " << truePeriod << " ms";
    qDebug() << "Width: " << trueWidth << " ms";

    CAENVME_StartPulser(handleChef, cvPulserA);


    long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    long t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

    while (t < now + truePeriod) {

        t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() ;
        ui->pulse_progressBar->setValue((int)(100*(t-now)/(truePeriod)));
        on_read_scalaer_value_pushButton_clicked();
        QCoreApplication::processEvents();

    }
    ui->pulse_progressBar->setValue(100);
    ui->pulse_pushButton->setEnabled(true);


}

void MainWindow::on_read_scalaer_value_pushButton_clicked()
{
    baseAddressScaler = ui->spinBox_rotary_switches_scaler->value() << 16;

    //uint32_t value32 = 0;
    //CAENVME_ReadCycle(handleChef, baseAddressScaler + 0x10, &value32, cvA32_U_DATA, cvD32);
    //ui->scaler_ch0_lcdNumber->display((int)value32);
    //CAENVME_ReadCycle(handleChef, baseAddressScaler + 0x14, &value32, cvA32_U_DATA, cvD32);
    //ui->scaler_ch1_lcdNumber->display((int)value32);

    unsigned char values[64];
    int sizeOut = 0;
    CAENVME_BLTReadCycle(handleChef, baseAddressScaler + 0x10, values, 64, cvA32_U_DATA, cvD32, &sizeOut);


    int * ch = (int*) values;
    ui->scaler_ch0_lcdNumber->display(*ch);
    ch = (int*) (values + 4);
    ui->scaler_ch1_lcdNumber->display(*ch);
    ch = (int*) (values + 8);
    ui->scaler_ch2_lcdNumber->display(*ch);
    ch = (int*) (values + 12);
    ui->scaler_ch3_lcdNumber->display(*ch);
    ch = (int*) (values + 16);
    ui->scaler_ch4_lcdNumber->display(*ch);
    ch = (int*) (values + 20);
    ui->scaler_ch5_lcdNumber->display(*ch);
    ch = (int*) (values + 24);
    ui->scaler_ch6_lcdNumber->display(*ch);
    ch = (int*) (values + 28);
    ui->scaler_ch7_lcdNumber->display(*ch);
    ch = (int*) (values + 32);
    ui->scaler_ch8_lcdNumber->display(*ch);
    ch = (int*) (values + 36);
    ui->scaler_ch9_lcdNumber->display(*ch);
    ch = (int*) (values + 40);
    ui->scaler_ch10_lcdNumber->display(*ch);
    ch = (int*) (values + 44);
    ui->scaler_ch11_lcdNumber->display(*ch);
    ch = (int*) (values + 48);
    ui->scaler_ch12_lcdNumber->display(*ch);
    ch = (int*) (values + 52);
    ui->scaler_ch13_lcdNumber->display(*ch);
    ch = (int*) (values + 56);
    ui->scaler_ch14_lcdNumber->display(*ch);
    ch = (int*) (values + 60);
    ui->scaler_ch15_lcdNumber->display(*ch);

}

void MainWindow::on_reset_scaler_pushButton_clicked()
{
    baseAddressScaler = ui->spinBox_rotary_switches_scaler->value() << 16;

    uint16_t value16 = 0;
    CAENVME_ReadCycle(handleChef, baseAddressScaler + 0x50, &value16, cvA32_U_DATA, cvD16);

    on_read_scalaer_value_pushButton_clicked();

    ui->pulse_progressBar->setValue(0);

}

void MainWindow::on_poisson_pushButton_clicked()
{


    unsigned char period;
    double truePeriod = 0; // in ms
    unsigned char width;
    double trueWidth = 0; // in ms
    CVTimeUnits unit;
    unsigned char pulseNo;
    CVIOSources start, reset;
    CAENVME_GetPulserConf(handleChef, cvPulserA, &period, &width, &unit, &pulseNo, &start, &reset);

    switch (unit) {
        case cvUnit25ns:
            truePeriod = (int)(period) * 0.000025;
            trueWidth = (int)(width) * 0.000025;
            break;
        case cvUnit104ms:
            truePeriod = (int)(period) * 104;
            trueWidth = (int)(width) * 104;
            break;
        case cvUnit410us:
            truePeriod = (int)(period) * 0.41;
            trueWidth = (int)(width) * 0.41;
            break;
        case cvUnit1600ns:
            truePeriod = (int)(period) * 0.0016;
            trueWidth = (int)(width) * 0.0016;
            break;
    }
    baseAddressScaler = ui->spinBox_rotary_switches_scaler->value() << 16;

    for (int i = 0; i < nExp; i++) {
        ui->pulse_progressBar->setValue((int)(100*i*1.0/nExp));
        uint16_t value16 = 0;
        CAENVME_ReadCycle(handleChef, baseAddressScaler + 0x50, &value16, cvA32_U_DATA, cvD16);

        usleep(200000);
        CAENVME_StartPulser(handleChef, cvPulserA);

        long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        long t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

        qDebug() << "Starting while loop ... ";
        while (t < now + truePeriod) {

            t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() ;
            QCoreApplication::processEvents();
        }

        qDebug() << "Waiting loop finished";
        uint32_t value32 = 0;
        CAENVME_ReadCycle(handleChef, baseAddressScaler + 0x10, &value32, cvA32_U_DATA, cvD32);
        qDebug() << "Finishing reading: " << value32;

        ui->scaler_ch0_lcdNumber->display((int)value32);

        hPoisson->updatePlot((int)value32);




    }
    ui->pulse_progressBar->setValue(100);

}





void MainWindow::on_spinBox_nevents_poissons_valueChanged(int nevents_)
{
    nExp = nevents_;
}
