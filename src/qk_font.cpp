//! \file qk_font.cpp
#include "Qkrellm.h"
#include "qk_font.h"

QK_Font::QK_Font() 
{
   // Get current font; second parameter is default
   QString family  = settings.value( "fontFamily", "DejaVu Sans" ).toString();
   int     size    = settings.value( "fontSize"  , 12 ).toInt();
   bool    checked = settings.value( "fontBold"  , false ).toBool();

//   setPalette( US_GuiSettings::frameColor() );

   // Frame layout
   setWindowTitle( "Font Selection Dialog" );
   setAttribute( Qt::WA_DeleteOnClose );
 
   QBoxLayout* topbox = new QVBoxLayout( this );
   topbox->setSpacing( 2 );

   // Body
   pb_font = qk_pushbutton( tr( "Select Base Font" ) );
   topbox->addWidget( pb_font );
   connect( pb_font, SIGNAL( clicked() ), SLOT( selectFont() ) );

   lbl_family = qk_label( tr( "Current Family:" ), -1 );
   lbl_family->setFixedHeight( BUTTON_H );
   
   QGridLayout* lineGrid = new QGridLayout();

   QFontDatabase fontLib;; 
   cb_family = qk_comboBox();
   cb_family->addItems( fontLib.families() );
   cb_family->setCurrentText( family );

   connect( cb_family, SIGNAL( currentIndexChanged( int ) ),
            this,      SLOT  ( update             ( int ) ) ); 
   
   int row = 0;

   lineGrid->addWidget( lbl_family, row  , 0 );
   lineGrid->addWidget( cb_family , row++, 1 );

   lbl_size = qk_label( tr( "Point Size:" ), -1 );
   lbl_size->setFixedHeight( BUTTON_H );
  
   sb_size = qk_spinBox();
   sb_size->setRange( 9, 24 );
   sb_size->setValue( size );    

   connect( sb_size, SIGNAL( valueChanged ( int ) ),
            this,    SLOT  ( update       ( int ) ) ); 

   ckbox_grid = qk_checkbox( "Bold", ckbox_bold, checked );

   stats_row = new QHBoxLayout();
   stats_row->addWidget( lbl_size );
   stats_row->addWidget( sb_size );
   stats_row->addLayout( ckbox_grid );

   lineGrid->addLayout( stats_row, row++, 0 );

   samples = qk_banner( tr( "Selected Font Samples:" ) );
   lineGrid->addWidget( samples, row++, 0, 1, 2 );

   small = qk_label( tr( "Small Font Sample" ), -1 );
   lineGrid->addWidget( small, row++, 0, 1, 2 );

   regular = qk_label( tr( "Regular Font Sample" ) );
   lineGrid->addWidget( regular, row++, 0, 1, 2 );

   regularBold = qk_label( tr( "Regular Font Sample, Bold" ), 0, QFont::Bold );
   lineGrid->addWidget( regularBold, row++, 0, 1, 2 );

   large = qk_label( tr( "Large Font Sample" ), +1 );
   lineGrid->addWidget( large, row++, 0, 1, 2 );

   largeBold = qk_label( tr( "Large Font Sample, Bold" ), +1, QFont::Bold );
   lineGrid->addWidget( largeBold, row++, 0, 1, 2 );

   title = qk_label( tr( "Title Font Sample" ), +2, QFont::Bold );
   lineGrid->addWidget( title, row++, 0, 1, 2 );

   topbox->addLayout( lineGrid );

   pb_default = qk_pushbutton( tr( "Select Default" ) );
   connect( pb_default, SIGNAL( clicked() ), SLOT( setDefault() ) );
   topbox->addWidget( pb_default );

   pb_apply = qk_pushbutton( tr( "Apply" ) );
   connect( pb_apply, SIGNAL( clicked() ), SLOT( apply() ) );

   //pb_help = qk_pushbutton( tr( "Help" ) );
   //connect( pb_help, SIGNAL( clicked() ), SLOT( help() ) );

   pb_exit = qk_pushbutton( tr( "Exit" ) );
   connect( pb_exit, SIGNAL( clicked() ), SLOT( close() ) );

   QBoxLayout* buttons = new QHBoxLayout();

   buttons->addWidget( pb_apply  );
   //buttons->addWidget( pb_help   );
   buttons->addWidget( pb_exit );

  topbox->addLayout( buttons );

  redraw();  // Set fonts fot all local widgets
}

void QK_Font::setDefault( void )
{
   cb_family->setCurrentText( "DejaVu Sans" );
   sb_size  ->setValue      ( 12 );
}

void QK_Font::update( int index )
{
   index++; // So the comiler does not complain about unused parameter
   redraw();
}

void QK_Font::apply()
{
   // Set font vales in settings
   settings.setValue( "fontFamily", cb_family->currentText() );
   settings.setValue( "fontSize"  , sb_size->value()         );
   settings.setValue( "fontBold"  , ckbox_bold->isChecked()  );
   settings.sync();

   //QMessageBox::information( this,
   //  tr( "Settings Saved" ),
   //  tr( "The settings were successfully saved.\n"
   //      "Changes have been applied." ) );

   emit updateFonts();
}

//void QK_Font::help()
//{
// US_Help* help = new US_Help();
// help->show_help( "manual/usfont.html" );
//}

void QK_Font::selectFont()
{
   bool    ok        = false;
   QString family    = cb_family->currentText();
   int     pointSize = sb_size->value();

   QFont oldFont = QFont( family, pointSize );
   QFont newFont = QFontDialog::getFont( &ok, oldFont, this );

   if ( ! ok ) return;

//qDebug() << newFont.family() << " " << newFont.pointSize();

   cb_family ->setCurrentText ( newFont.family() );
   sb_size   ->setValue       ( newFont.pointSize() );
}

void QK_Font::redraw( void )
{
   QString family    = cb_family->currentText();
   int     pointSize = sb_size->value();

//qDebug() << "redraw: " << family << " " << pointSize;

   lbl_family  ->setFont( QFont( family, pointSize                  ) );
   cb_family   ->setFont( QFont( family, pointSize                  ) );
   lbl_size    ->setFont( QFont( family, pointSize                  ) );
   sb_size     ->setFont( QFont( family, pointSize                  ) );
   ckbox_bold  ->setFont( QFont( family, pointSize                  ) );
   samples     ->setFont( QFont( family, pointSize                  ) );

   small       ->setFont( QFont( family, pointSize - 1              ) );
   regular     ->setFont( QFont( family, pointSize                  ) );
   regularBold ->setFont( QFont( family, pointSize,     QFont::Bold ) );
   large       ->setFont( QFont( family, pointSize + 1              ) );
   largeBold   ->setFont( QFont( family, pointSize + 1, QFont::Bold ) );
   title       ->setFont( QFont( family, pointSize + 2, QFont::Bold ) );
   pb_default  ->setFont( QFont( family, pointSize + 1 ) );
   pb_apply    ->setFont( QFont( family, pointSize + 1 ) );
   pb_font     ->setFont( QFont( family, pointSize + 1 ) );
   //pb_help     ->setFont( QFont( family, pointSize + 1 ) );
   pb_exit     ->setFont( QFont( family, pointSize + 1 ) );

   this->repaint();
}

