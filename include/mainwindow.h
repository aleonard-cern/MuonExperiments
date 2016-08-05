#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLCDNumber>
#include <QLabel>
#include <qcustomplot.h>

#include <unistd.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_radioButton_on_off_clicked();

    void on_pushButton_set_clicked();

    void readHV();

    void readBoardParameters(int baseAddress);

    bool readTDCParameters(int baseAddress);

    void on_spinBox_channel_valueChanged(int arg1);

    void on_spinBox_rotary_switches_valueChanged(int arg1);

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_start_run_clicked();

    void adjustPlot();

    void updatePlot(double value);

    void on_spinBox_nbins_valueChanged(int arg1);

    void on_doubleSpinBox_tmax_valueChanged(double arg1);

    void on_doubleSpinBox_tmin_valueChanged(double arg1);

    void on_pushButton_stop_run_clicked();

    void on_pushButton_clear_bars_clicked();

    void closing();

    void closeEvent (QCloseEvent *event);


    void on_pushButton_clear_tdc_clicked();

    void on_log_pushButton_clicked();


    void on_file_name_pushButton_clicked();

    void on_pulse_pushButton_clicked();

    void on_read_scalaer_value_pushButton_clicked();

    void on_reset_scaler_pushButton_clicked();

private:
    bool isLog;
    Ui::MainWindow *ui;
    QTimer *timer;
    int32_t handleChef;
    uint32_t baseAddressHV;
    uint32_t baseAddressTDC;
    uint32_t baseAddressScaler;
    bool HVenabled;

    std::vector<double> bins;


    int VMAX;

    QLCDNumber* lcdNumber_voltages[6];
    QLCDNumber* lcdNumber_currents[6];
    QLabel* label_state_leds[6];
    QLabel* label_ramp_up_leds[6];
    QLabel* label_ramp_down_leds[6];
    QLabel* label_over_current_leds[6];
    QLabel* label_over_voltage_leds[6];
    QLabel* label_under_voltage_leds[6];
    QLabel* label_maxv_leds[6];
    QLabel* label_maxi_leds[6];
    QLabel* label_trip_leds[6];
    QLabel* label_over_power_leds[6];
    QLabel* label_disabled_leds[6];
    QLabel* label_interlock_leds[6];

    QVector<double> x, y;
    QCPBars *bars;
    int maxOfEntries;
    int nBins;
    double tMin, tMax;


    int tdcStartCh;
    int tdcStopCh;

    bool stopRun;
    bool running;


    int counter;
    QTime prevTime;
    double mean_arrival;
    double mean_arrival_square;
    double mean_lifetime;
    double mean_lifetime_square;

    void waitForReadOK();
    void waitForWriteOK();
    bool setTriggerMode();
    void readTDCSetting();
    void setPulserMode();

    bool tdcTriggerSettingIsGood;
    QString dataFileName;

};

#endif // MAINWINDOW_H
