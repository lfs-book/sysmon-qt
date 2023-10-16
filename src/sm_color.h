//! \file sm_font.h
#ifndef SM_COLOR_H
#define SM_COLOR_H

#include <QColor>
#include <QColorDialog>

#include "sm_widgets.h"
#include "sm_config.h"

//! Define the height of widgets in pixels.
#define BUTTON_H 26

//!  A class to allow the user to select a customized font.
class SM_Color : public SM_Widgets
{
  Q_OBJECT
  
  public:
    //*! \brief Construct the window for font selection
    SM_Color();
    
    //! \brief A null destructor.
    ~SM_Color() {};

    QPushButton* pb_apply;

  signals:  
    void updateColors( void );

  private:
    QSettings    settings;

    QComboBox*   cb_family;
    QSpinBox*    sb_size;

    QCheckBox*   ckbox_bold;
    QGridLayout* ckbox_grid;
    QHBoxLayout* stats_row;

    QColor*      lbl_color;
    QColor*      lbl_background;

    QLabel*      small;
    QLabel*      regular;
    QLabel*      regularBold;
    QLabel*      large;
    QLabel*      largeBold;
    QLabel*      title;
    QLabel*      lbl_family;
    QLabel*      lbl_size;
    QLabel*      sample_label;
    QLabel*      sample_data;

    QProgressBar* sample_progress;

    QPushButton* pb_label_color;
    QPushButton* pb_label_background;
    QPushButton* pb_data_color;
    QPushButton* pb_data_background;
    QPushButton* pb_progress_color;
    QPushButton* pb_progress_background;

    //QPushButton* pb_font;
    QPushButton* pb_default;
    //QPushButton* pb_help;
    QPushButton* pb_exit;

    void redraw( void );

  private slots:
  
    void label_color        ( void );
    void label_background   ( void );
    void data_color         ( void );
    void data_background    ( void );
    void progress_color     ( void );
    void progress_background( void );

    void apply     ( void );
    //void help      ( void );
    void setDefault( void );
    void update    ( int  );
};
#endif
