//! \file qk_font.cpp
#include "Qkrellm.h"
#include "qk_font.h"

QK_Color::QK_Color() 
{
   // Get current font; second parameter is default
   QString family  = settings.value( "fontFamily", "DejaVu Sans" ).toString();
   int     size    = settings.value( "fontSize"  , 12 ).toInt();
   bool    checked = settings.value( "fontBold"  , false ).toBool();
   int     weight  = checked ? QFont::Bold : QFont::Normal;

   // Frame layout
   setWindowTitle( "Color Selection Dialog" );
   setAttribute( Qt::WA_DeleteOnClose );
   QBoxLayout* topbox = new QVBoxLayout( this );
   topbox->setSpacing( 2 );

   // Label color
   QGridLayout* labelLayout = new QGridLayout();

   pb_label_color = qk_pushbutton( tr( "Change\nLabel Color" ) );
   labelLayout->addWidget( pb_label_color, 0, 0 );
   connect( pb_label_color, SIGNAL( clicked() ), SLOT( label_color() ) );

   pb_label_background = qk_pushbutton( tr( "Change\nLabel Background" ) );
   labelLayout->addWidget( pb_label_background, 0 , 1 );
   connect( pb_label_background, SIGNAL( clicked() ), SLOT( label_background() ) );

   sample_label = qk_label( "Sample Label" );
   sample_label->setFont( QFont( family, size, weight ) );
   labelLayout->addWidget( sample_label, 0, 2 );

   // Data color
   pb_data_color = qk_pushbutton( tr( "Change\nData Color" ) );
   labelLayout->addWidget( pb_data_color, 1, 0 );
   connect( pb_data_color, SIGNAL( clicked() ), SLOT( data_color() ) );

   pb_data_background = qk_pushbutton( tr( "Change\nData Background" ) );
   labelLayout->addWidget( pb_data_background, 1, 1 );
   connect( pb_data_background, SIGNAL( clicked() ), SLOT( data_background() ) );

   sample_data = qk_label( "Sample Data" );
   sample_data->setFont( QFont( family, size, weight ) );
   labelLayout->addWidget( sample_data, 1, 2 );

   //topbox->addLayout( labelLayout );

   // ProgressBar color
   pb_progress_color = qk_pushbutton( tr( "Change\nProgress Color" ) );
   labelLayout->addWidget( pb_progress_color, 2, 0 );
   connect( pb_progress_color, SIGNAL( clicked() ), SLOT( progress_color() ) );

   pb_progress_background = qk_pushbutton( tr( "Change\nProgress Background" ) );
   labelLayout->addWidget( pb_progress_background, 2, 1 );
   connect( pb_progress_background, SIGNAL( clicked() ), SLOT( progress_background() ) );

   sample_progress = new QProgressBar();
   sample_progress->setRange( 0, 100 );
   sample_progress->setValue( 75 );
   sample_progress->setFont( QFont( family, size, weight ) );
   labelLayout->addWidget( sample_progress, 2, 2 );

   topbox->addLayout( labelLayout );

   // Default

   pb_default = qk_pushbutton( tr( "Set Default Colors" ) );
   connect( pb_default, SIGNAL( clicked() ), SLOT( setDefault() ) );
   topbox->addWidget( pb_default );
   
   // Buttons
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

   redraw();  // Set colors for examples
}

void QK_Color::setDefault( void )
{
   QPalette p = sample_label->palette();

   p.setColor( QPalette::Active,   QPalette::Window,     QColor( "#efefef" ) );
   p.setColor( QPalette::Active,   QPalette::WindowText, QColor( "#000000" ) );
   p.setColor( QPalette::Inactive, QPalette::Window,     QColor( "#efefef" ) );
   p.setColor( QPalette::Inactive, QPalette::WindowText, QColor( "#000000" ) );

   p.setColor( QPalette::Active,   QPalette::Highlight,  QColor( "#308cc6" ) );
   p.setColor( QPalette::Active,   QPalette::Base,       QColor( "#ffffff" ) );
   p.setColor( QPalette::Inactive, QPalette::Highlight,  QColor( "#308cc6" ) );
   p.setColor( QPalette::Inactive, QPalette::Base,       QColor( "#ffffff" ) );

   sample_label   ->setPalette( p );
   sample_data    ->setPalette( p );
   sample_progress->setPalette( p );
}

void QK_Color::label_color( void )
{
   QPalette p = sample_label->palette();
                                      // Background is QPalette::Window
   QColor oldColor  = p.color( QPalette::Active, QPalette::WindowText);
   QColor newColor  = QColorDialog::getColor(oldColor, this );

   if ( ! newColor.isValid() ) return;

   p.setColor( QPalette::Active,   QPalette::WindowText, newColor );
   p.setColor( QPalette::Inactive, QPalette::WindowText, newColor );
   sample_label->setPalette( p );
}

void QK_Color::label_background( void )
{
   QPalette p = sample_label->palette();
                                      // Background is QPalette::Window
   QColor oldColor  = p.color( QPalette::Active, QPalette::Window);
   QColor newColor  = QColorDialog::getColor(oldColor, this );

   if ( ! newColor.isValid() ) return;

   p.setColor( QPalette::Active,   QPalette::Window, newColor );
   p.setColor( QPalette::Inactive, QPalette::Window, newColor );
   sample_label->setPalette( p );
}

void QK_Color::data_color( void )
{
   QPalette p = sample_data->palette();
                                      // Background is QPalette::Window
   QColor oldColor  = p.color( QPalette::Active, QPalette::WindowText);
   QColor newColor  = QColorDialog::getColor(oldColor, this );

   if ( ! newColor.isValid() ) return;

   p.setColor( QPalette::Active,   QPalette::WindowText, newColor );
   p.setColor( QPalette::Inactive, QPalette::WindowText, newColor );
   sample_data->setPalette( p );
}

void QK_Color::data_background( void )
{
   QPalette p = sample_data->palette();
                                      // Background is QPalette::Window
   QColor oldColor  = p.color( QPalette::Active, QPalette::Window);
   QColor newColor  = QColorDialog::getColor(oldColor, this );

   if ( ! newColor.isValid() ) return;

   p.setColor( QPalette::Active,   QPalette::Window, newColor );
   p.setColor( QPalette::Inactive, QPalette::Window, newColor );
   sample_data->setPalette( p );
}

void QK_Color::progress_color( void )
{
   QPalette p = sample_progress->palette();
                                      // Background is QPalette::Window
   QColor oldColor  = p.color( QPalette::Active, QPalette::WindowText);
   QColor newColor  = QColorDialog::getColor(oldColor, this );

   if ( ! newColor.isValid() ) return;

   p.setColor( QPalette::Active,   QPalette::Highlight, newColor );//12
   p.setColor( QPalette::Inactive, QPalette::Highlight, newColor );
   sample_progress->setPalette( p );
}

void QK_Color::progress_background( void )
{
   QPalette p = sample_progress->palette();
                                      // Background is QPalette::Window
   QColor oldColor  = p.color( QPalette::Active, QPalette::Window);
   QColor newColor  = QColorDialog::getColor(oldColor, this );

   if ( ! newColor.isValid() ) return;

   p.setColor( QPalette::Active,   QPalette::Base, newColor ); //9
   p.setColor( QPalette::Inactive, QPalette::Base, newColor );
   sample_progress->setPalette( p );
}

void QK_Color::update( int index )
{
   index++; // So the comiler does not complain about unused parameter
   redraw();
}

void QK_Color::apply()
{
   QPalette p          = sample_label->palette();
   QColor   text       = p.color( QPalette::Active, QPalette::WindowText );
   QColor   background = p.color( QPalette::Active, QPalette::Window );

   // Set color values in settings
   settings.setValue( "labelColor", text      .name() );
   settings.setValue( "labelBg"   , background.name() );

   p          = sample_data->palette();
   text       = p.color( QPalette::Active, QPalette::WindowText );
   background = p.color( QPalette::Active, QPalette::Window );

   settings.setValue( "dataColor", text.name()      );
   settings.setValue( "dataBg"   , background.name() );

   p          = sample_progress->palette();
   text       = p.color( QPalette::Active, QPalette::Highlight );
   background = p.color( QPalette::Active, QPalette::Base );

   settings.setValue( "progressColor", text.name()      );
   settings.setValue( "progressBg"   , background.name() );

   settings.sync();
   emit updateColors();
}

//void QK_Font::help()
//{
// US_Help* help = new US_Help();
// help->show_help( "manual/usfont.html" );
//}

void QK_Color::redraw( void )
{
   QString lblFg      = settings.value( "labelColor",    "#000000").toString();
   QString lblBg      = settings.value( "labelBg",       "#ffffff").toString();

   QString dataFg     = settings.value( "dataColor",     "#efefef").toString();
   QString dataBg     = settings.value( "dataBg",        "#000000").toString();

   QString progressFg = settings.value( "progressColor", "#efefef").toString();
   QString progressBg = settings.value( "progressBg",    "#000000").toString();

   QPalette p;
   p.setColor( QPalette::Active, QPalette::WindowText, QColor( lblFg ) );
   p.setColor( QPalette::Active, QPalette::Window,     QColor( lblBg ) );

   sample_label->setPalette( p );

   p.setColor( QPalette::Active, QPalette::WindowText, QColor( dataFg ) );
   p.setColor( QPalette::Active, QPalette::Window,     QColor( dataBg ) );

   sample_data->setPalette( p );

   p.setColor( QPalette::Active, QPalette::Highlight, QColor( progressFg ) );
   p.setColor( QPalette::Active, QPalette::Base,      QColor( progressBg ) );

   sample_progress->setPalette( p );

   this->repaint();
}
