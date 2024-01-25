#ifndef SYSMONQT_H
#define SYSMONQT_H

#include <QFont>
#include <QFile>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QPoint>
#include <QProgressBar>
#include <QSettings>

class sysmon_qt : public QMainWindow
{
   Q_OBJECT

public:
   sysmon_qt();
   ~sysmon_qt();

public slots:
   void updateFont ( void );
   void updateColor( void );
   void updateTemp ( void );

private:
   void    setFrame();

   QLabel* label    ( const QString&, int =  0, int = QFont::Normal );
   QLabel* textlabel( const QString&, int = -1, int = QFont::Normal );
   QLabel* banner   ( const QString&, int =  0, int = QFont::Bold );

   int        tick            = 0;     // tick every second
   int        cpu_refresh     = 3;
   int        memory_refresh  = 3;
   int        temp_refresh    = 5;
   int        tempsCount;
   int        cpuPercentage   = 0;
   bool       pauseUpdate     = false;
   bool       mFrame          = false; // turn frame on and off
   bool       pauseTempUpdate = false; // turn temperature update on and off
   QString    TZ;                      // time zone
   QPoint     position;                // location of widget
             
   QLabel*    lbl_hostname;
   QLabel*    lbl_time;     // time of day
   QLabel*    lbl_date;     
   QLabel*    lbl_uptime;
   QLabel*    lbl_cpu;      // title
   QLabel*    lbl_loads;    // label for memory
   QLabel*    lbl_memory;   // title
             
   QFile*     fUptime;
   QFile*     fLoadAvg;
   QFile*     fMemInfo;
   QFile*     fStat;

   QProgressBar* memory;
   QProgressBar* load;
   QGridLayout*  tempsLayout;
   
   QStringList      tempDesc;
   QList< QLabel* > degrees;

   QSettings settings;       // $HOME/.config/LinuxFromScratch/sysmon-qt.conf
                             
   QString   font_family;
   int       font_size;
   QFont     font_normal;

   QPalette  banner_palette; 
   QPalette  data_palette;

   QMap< QString, int > sensor_data; 

   void setup_all    ( void );
   void setup_time   ( void );
   void setup_date   ( void );
   void setup_uptime ( void );
   void setup_cpuLoad( void );
   void setup_memory ( void );

   void update_cpu   ( void );
   void update_memory( void );
   void update_uptime( void );
   void update_date  ( void );
   void update_time  ( void );
   void update_temps ( void );
   void delete_temps ( void );
   void set_palettes ( void );

   void update_cpu_percentage( void );
  
   void delete_all   ( void );

   QVBoxLayout* layout;       // Top level layout
   QGridLayout* set_tempsLayout( void );

private Q_SLOTS:
   void changeFrame ( void );
   void update      ( void );
   void config      ( void );
   void updateLayout( void );
};
#endif

