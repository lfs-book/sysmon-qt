//! \file sm_widgets.h
#ifndef SM_WIDGETS_H
#define SM_WIDGETS_H

#include <QtCore>
#include <QtWidgets>
#include <QSettings>

/*! \brief Set up widgets our way.
 
    This class is designed to be the parent class to almost all 
    windows.  It allows easy creation of widgets and applies the appropriate
    palette and font acording to the user's settings (or the default).
    Depending on the widget, other parameters may be set.
*/
class SM_Widgets : public QFrame
{
  public: 
    //! The constructor connects to global memory and moves the screen to 
    //! the point saved there. 
    SM_Widgets( void );
    ~SM_Widgets( void ){};

    //! \param labelString - contents of label
    //! \param fontAdjust  - adjustment to default point size 
    //! \param weight      - QFont::{Light,Normal,DemiBold,Bold,Black}\n
    //! * Alignment is set to Qt::AlignVCenter | Qt::AlignLeft\n
    //! * Margin is set to 5\n
    //! * Palette is set to US_GuiSettings::labelColor()
    QLabel*      sm_label     ( const QString&, int = 0,  QFont::Weight = QFont::Bold );

    //! Same as us_label except:\n
    //! * different default font size (-1 point)\n
    //! * color set to US_GuiSettings::editColor()
    QLabel*      sm_textlabel ( const QString&, int = -1, QFont::Weight = QFont::Bold );

    //! Same as us_label except:\n
    //! * color set to US_GuiSettings::frameColor()\n
    //! * alignment set to Qt::AlignCenter\n
    //! * style set to QFrame::WinPanel | QFrame::Raised\n
    //! * MidLineWidth set to 2 for frame border
    QLabel*      sm_banner    ( const QString&, int = 0,  QFont::Weight = QFont::Bold );

    //! \param labelString - text in pushbutton
    //! \param enabled
    //! \param fontAdjust -  adjustment to default font size\n
    //! * Standard Font family and size\n
    //! * color set to US_GuiSettings::pushbColor()\n
    //! * button is enabled\n
    //! * AutoDefault is set false
    QPushButton* sm_pushbutton( const QString&, bool = true, int = 0 ); 

    //! \param text - starting text in line edit box
    //! \param fontAdjust -  adjustment to default point size\n
    //! \param readonly -  flag if read-only, default uses editColor()
    QLineEdit* sm_lineedit( const QString& = 0, int = -1, bool = false );

    //! \param text - Text to set with checkbox
    //! \param cb - reference to a pointer of the checkbox\n
    //! \param state - Initial check state\n
    //! * Font weight is bold\n
    //! * Color is set to US_GuiSettings::normalColor()
    QGridLayout* sm_checkbox  ( const QString&, QCheckBox*&, bool = false );

    QComboBox*   sm_comboBox( void );
    QSpinBox*    sm_spinBox ( int = 0 );

  private:
      QSettings settings;
      QString   fontFamily;
      QString   fontSize; 
      QFont     font;
};
#endif

