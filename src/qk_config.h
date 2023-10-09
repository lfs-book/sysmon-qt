//! \file qk_config.h

#ifndef QKCONFIG_H
#define QKCONFIG_H

#include "qk_widgets.h"
#include "qk_font.h"
#include "qk_color.h"
#include "qk_temps.h"
#include "QKrellm_version.h"

//! \brief Provide the top level window for user configuration

/*! \class QK_Config
           This class provides the top level window for user configuration.
           It allows customization of the user's Qkrellm display.
           It also provides pushbuttons for access to screens to configure 
           the user's fonts and widget colors.
*/
class QK_Config : public QK_Widgets
{
  Q_OBJECT

public:
    /*! \brief The constructor creates the various controls for the main
               window.  
    */
   QK_Config( QWidget* = nullptr );
   ~QK_Config() {};
  
   QPushButton* pb_apply;

private:
//  QPushButton*    pb_help;
   QLabel*      version;
   QLabel*      lbl_font;
   QLabel*      lbl_color;
   QLabel*      lbl_temps;

   QPushButton* pb_font;
   QPushButton* pb_color;
   QPushButton* pb_temps;
   QPushButton* pb_exit;
   
   // Help pushbuttons
   QPushButton* PBtime;
   QPushButton* PBdate;
   QPushButton* PBuptime;
   QPushButton* PBcpu;
   QPushButton* PBmemory;

   QCheckBox*   CBtime;
   QCheckBox*   CBdate;
   QCheckBox*   CBuptime;
   QCheckBox*   CBcpu;
   QCheckBox*   CBcpuBar;
   QCheckBox*   CBmemory;

   QLineEdit*   LEtime;
   QLineEdit*   LEdate;

   QSettings    settings;

   void         update_local( void );
   void         msg_box     ( QString*, int );

   QStringList  sensors;  // <Interface,Device>

private slots:

  void update_font  ( void );
  void update_colors( void );
  void update_temps ( void );
  void time_help    ( void );
  void date_help    ( void );
  void uptime_help  ( void );
  void cpu_help     ( void );
  void memory_help  ( void );

public slots:
   void sendFonts();
   void sendColors();
   void sendTemps();
   void apply();

signals:
   void updateFonts   ( void );
   void updateColors  ( void );
   void updateTemps   ( void );
   void updateEntries ( void );
};
#endif
