//! \file sm_config.h

#ifndef SMCONFIG_H
#define SMCONFIG_H

#include "sm_widgets.h"
#include "sm_font.h"
#include "sm_color.h"
#include "sm_temps.h"
#include "version.h"

//! \brief Provide the top level window for user configuration

/*! \class SM_Config
           This class provides the top level window for user configuration.
           It allows customization of the user's Qkrellm display.
           It also provides pushbuttons for access to screens to configure 
           the user's fonts and widget colors.
*/
class SM_Config : public SM_Widgets
{
  Q_OBJECT

public:
    /*! \brief The constructor creates the various controls for the main
               window.  
    */
   SM_Config( QWidget* = nullptr );
   ~SM_Config() {};
  
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
