//! \file us_widgets.cpp
//#include <QtSvg> 

#include "qk_widgets.h"

QK_Widgets::QK_Widgets()
{

   font = QFont( settings.value( "fontFamily", "DejaVu Sans" ).toString(), 
                 settings.value( "fontSize"  , 12 ).toInt() );
}

// label
QLabel* QK_Widgets::qk_label( const QString& labelString, int fontAdjust, int weight )
{
   QLabel* newLabel = new QLabel( labelString, this );

   newLabel->setFrameStyle( QFrame::StyledPanel | QFrame::Raised );
   newLabel->setAlignment ( Qt::AlignVCenter | Qt::AlignLeft );
   newLabel->setMargin    ( 2 );
   newLabel->setAutoFillBackground( true );

   QFont thisFont = font;
   thisFont.setPointSize( font.pointSize() + fontAdjust );
   thisFont.setWeight   ( weight );

   newLabel->setFont( thisFont );

   return newLabel;
}

// textlabel ( fontAdjust defaults to smaller font (-1) if not specified )
QLabel* QK_Widgets::qk_textlabel( const QString& labelString, int fontAdjust, int weight )
{
   return qk_label( labelString, fontAdjust, weight );
}

// banner ( defaults to Bold if weight not specified )
QLabel* QK_Widgets::qk_banner( const QString& labelString, int fontAdjust, int weight )
{
   QLabel* newLabel = qk_label( labelString, fontAdjust, weight );

   newLabel->setAlignment ( Qt::AlignCenter );
   newLabel->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
   newLabel->setMidLineWidth( 2 );
   newLabel->setFont( font );

   return newLabel;
}

// pushbutton
QPushButton* QK_Widgets::qk_pushbutton( const QString& labelString, bool enabled,
                                        int fontAdjust )
{
   QPushButton* button = new QPushButton( tr( labelString.toLatin1() ), this );

   QFont thisFont = font;
   thisFont.setPointSize( font.pointSize() + fontAdjust );
   button->setFont( thisFont );

   QString style = "border: 4px outset gray; border-radius: 10px;";
   button->setStyleSheet( style );

   button->setAutoDefault( false );
   button->setEnabled( enabled );

   return button;
}

// SpinBox
QSpinBox* QK_Widgets::qk_spinBox( const int fontAdjust )
{
   QSpinBox* sbox = new QSpinBox( this );

   QFont thisFont = font;
   thisFont.setPointSize( font.weight() + fontAdjust );

   sbox->setAutoFillBackground( true );
   sbox->setFont( thisFont );

   return sbox;
}

// Combo Box
QComboBox* QK_Widgets::qk_comboBox( void )
{
  QComboBox* cb = new QComboBox( this );

  cb->setAutoFillBackground( true );
  cb->setFont( font );
                     
  return cb;
}

// checkbox
QGridLayout* QK_Widgets::qk_checkbox( 
      const QString& text, QCheckBox*& cb, bool state )
{
   QFont thisFont = font;
   thisFont.setWeight( QFont::Bold );

   QFontMetrics fm( font );

   QLabel* lb_spacer = new QLabel;

   // Space as wide as a 'w'
   lb_spacer->setFixedWidth        ( fm.horizontalAdvance( "w" ) ); 
   lb_spacer->setAutoFillBackground( true );

   cb = new QCheckBox( text.toLatin1(), this );
   cb->setFont              ( thisFont  );
   cb->setChecked           ( state );
   cb->setAutoFillBackground( true  );

   QGridLayout* layout = new QGridLayout;
   layout->setContentsMargins( 0, 0, 0, 0 );
   layout->setSpacing        ( 0 );

   layout->addWidget( lb_spacer, 0, 0 );
   layout->addWidget( cb       , 0, 1 );

   return layout;
}

// lineedit
QLineEdit* QK_Widgets::qk_lineedit( const QString& text, int fontAdjust,
      bool readonly )
{
   QLineEdit* le = new QLineEdit( this );

   if ( readonly ) fontAdjust++;  

   QFont thisFont = font;
   thisFont.setPointSize( font.pointSize() + fontAdjust );
  
   le->setFont              ( thisFont );
   le->insert               ( text );
   le->setAutoFillBackground( true );
   le->show();

   return le;
}

