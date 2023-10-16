//! \file us_font.h
#ifndef SM_TEMPS_H
#define SM_TEMPS_H

#include "sm_widgets.h"
#include "sm_config.h"

//!  A class to allow the user to select a customized font.
class SM_Temps : public SM_Widgets
{
  Q_OBJECT
  
  public:
    //*! \brief Construct the window for font selection
    SM_Temps();
    
    //! \brief A null destructor.
    ~SM_Temps() {};

    QPushButton* pb_apply;

  signals:  
    void updateTemps( void );

  private:
    QSettings           settings;

    QGridLayout*        tempsLayout;

    QList< QCheckBox* > checkboxes;
    QList< QLineEdit* > labels;

    QStringList         sensors;  // <Interface,Device>

    QPushButton*        pb_exit;
    //QPushButton*        pb_help;

    void get_temp_data( void );

  private slots:
  
    void apply     ( void );
    //void help      ( void );
};
#endif
