//! \file us_font.h
#ifndef QK_TEMPS_H
#define QK_TEMPS_H

#include "qk_widgets.h"
#include "qk_config.h"

//!  A class to allow the user to select a customized font.
class QK_Temps : public QK_Widgets
{
  Q_OBJECT
  
  public:
    //*! \brief Construct the window for font selection
    QK_Temps();
    
    //! \brief A null destructor.
    ~QK_Temps() {};

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
