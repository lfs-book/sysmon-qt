//! \file us_font.h
#ifndef QK_FONT_H
#define QK_FONT_H

#include "qk_widgets.h"
#include "qk_config.h"

//! Define the height of widgets in pixels.
#define BUTTON_H 26

//!  A class to allow the user to select a customized font.
class QK_Font : public QK_Widgets
{
  Q_OBJECT
  
  public:
    //*! \brief Construct the window for font selection
    QK_Font();
    
    //! \brief A null destructor.
    ~QK_Font() {};

    QPushButton* pb_apply;

  signals:  
    void updateFonts( void );

  private:
    QSettings    settings;

    QComboBox*   cb_family;
    QSpinBox*    sb_size;

    QCheckBox*   ckbox_bold;
    QGridLayout* ckbox_grid;
    QHBoxLayout* stats_row;

    QLabel*      small;
    QLabel*      regular;
    QLabel*      regularBold;
    QLabel*      large;
    QLabel*      largeBold;
    QLabel*      title;
    QLabel*      lbl_family;
    QLabel*      lbl_size;
    QLabel*      samples;

    QPushButton* pb_font;
    QPushButton* pb_default;
    //QPushButton* pb_help;
    QPushButton* pb_exit;

    void redraw( void );

  private slots:
  
    void apply     ( void );
    //void help      ( void );
    void selectFont( void );
    void setDefault( void );
    void update    ( int  );

  //private signals:
  //  void do_update( void );
};

#endif
