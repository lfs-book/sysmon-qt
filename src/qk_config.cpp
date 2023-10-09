//! \file qk_config.cpp
//#include <QApplication>

#include "qk_config.h"

QK_Config::QK_Config( QWidget* parent )
{
   setWindowTitle( "Qkrellm Configuration" );

   QString family  = settings.value( "fontFamily", "DejaVu Sans" ).toString();
   int     size    = settings.value( "fontSize"  , 12 ).toInt();
   QFont   oldfont = QFont( family, size, QFont::Normal );  

   // Directories
   
   QBoxLayout* topbox = new QVBoxLayout( this );
   topbox->setContentsMargins ( 2, 2, 2, 2 );
   topbox->setSpacing         ( 2 );

   version = qk_label( "Version: " QKVERSION );
   topbox->addWidget( version );

   int row = 0;
   QGridLayout* mainEntries = new QGridLayout();

   // time
   QString timeFormat  = settings.value( "timeFormat", "HH:mm:ss" ).toString();
   bool    timeChecked = settings.value( "useTime",    true       ).toBool();

   QGridLayout* CBTimeLayout = qk_checkbox( "time", CBtime, timeChecked );
                LEtime       = qk_lineedit( timeFormat );
                PBtime       = qk_pushbutton( tr( "Help" ) );

   CBtime->setFont( oldfont );
   LEtime->setFont( oldfont );
   PBtime->setFont( oldfont );

   connect( PBtime, SIGNAL( clicked() ), this, SLOT( time_help() ) );

   mainEntries->addLayout( CBTimeLayout, row,   0 );
   mainEntries->addWidget( LEtime,       row,   1 );
   mainEntries->addWidget( PBtime,       row++, 2 );

   // date
   QString dateFormat  = settings.value( "dateFormat", "ddd d MMM" ).toString();
   bool    dateChecked = settings.value( "useDate",    true        ).toBool();

   QGridLayout* CBDateLayout = qk_checkbox( "date", CBdate, dateChecked );
                LEdate       = qk_lineedit( dateFormat );
                PBdate       = qk_pushbutton( tr( "Help" ) );

   CBdate->setFont( oldfont );
   LEdate->setFont( oldfont );
   PBdate->setFont( oldfont );

   connect( PBdate, SIGNAL( clicked() ), this, SLOT( date_help() ) );

   mainEntries->addLayout( CBDateLayout, row,   0 );
   mainEntries->addWidget( LEdate,       row,   1 );
   mainEntries->addWidget( PBdate,       row++, 2 );

   // uptime
   bool uptimeChecked = settings.value( "useUptime", true ).toBool();

   QGridLayout* CBuptimeLayout = qk_checkbox( "uptime", CBuptime, uptimeChecked );
                PBuptime       = qk_pushbutton( tr( "Help" ) );

   CBuptime->setFont( oldfont );
   PBuptime->setFont( oldfont );

   connect( PBuptime, SIGNAL( clicked() ), this, SLOT( uptime_help() ) );

   mainEntries->addLayout( CBuptimeLayout, row,   0 );
   mainEntries->addWidget( PBuptime,       row++, 2 );

   // CPU Load
   bool CPUchecked = settings.value( "useCPU",    true ).toBool();
   bool CPUbar     = settings.value( "useCPUbar", true ).toBool();

   QGridLayout* CBcpuLayout    = qk_checkbox( "CPU Load", CBcpu,    CPUchecked );
   QGridLayout* CBcpuBarLayout = qk_checkbox( "CPU Bar",  CBcpuBar, CPUbar );
                PBcpu          = qk_pushbutton( tr( "Help" ) );

   CBcpu   ->setFont( oldfont );
   CBcpuBar->setFont( oldfont );
   PBcpu   ->setFont( oldfont );

   connect( PBcpu, SIGNAL( clicked() ), this, SLOT( cpu_help() ) );

   mainEntries->addLayout( CBcpuLayout,    row,   0 );
   mainEntries->addLayout( CBcpuBarLayout, row,   1 );
   mainEntries->addWidget( PBcpu,          row++, 2 );

   // Memory display
   bool memoryChecked = settings.value( "useMemory", true ).toBool();

   QGridLayout* CBmemoryLayout = qk_checkbox( "Memory Bar", CBmemory, memoryChecked );
                PBmemory       = qk_pushbutton( tr( "Help" ) );

   CBmemory->setFont( oldfont );
   PBmemory->setFont( oldfont );
   
   connect( PBmemory, SIGNAL( clicked() ), this, SLOT( memory_help() ) );

   mainEntries->addLayout( CBmemoryLayout, row,   0 );
   mainEntries->addWidget( PBmemory,       row++, 2 );

   topbox->addLayout( mainEntries );

   // Misc Settings
   row = 0;
   QGridLayout* otherSettings = new QGridLayout();

   // Font Preferences
   lbl_font = qk_label(      tr( "Font Preferences:" ) );
   pb_font  = qk_pushbutton( tr( "Change Font"       ) );

   otherSettings->addWidget( lbl_font, row,   0 );
   otherSettings->addWidget( pb_font,  row++, 1 );
   connect( pb_font, SIGNAL( clicked() ), this, SLOT( update_font() ) );

   // Color Preferences
   lbl_color = qk_label(      tr( "Color Preferences:" ) );
   pb_color  = qk_pushbutton( tr( "Change Colors"      ) );

   otherSettings->addWidget( lbl_color, row,   0 );
   otherSettings->addWidget( pb_color,  row++, 1 );
   connect( pb_color, SIGNAL( clicked() ), this, SLOT( update_colors() ) );

   // Temperature Preferences
   lbl_temps = qk_label(      tr( "Temperature Preferences:" ) );
   pb_temps  = qk_pushbutton( tr( "Change Temps"             ) );

   otherSettings->addWidget( lbl_temps, row,   0 );
   otherSettings->addWidget( pb_temps,  row++, 1 );
   connect( pb_temps, SIGNAL( clicked() ), this, SLOT( update_temps() ) );

   // Pushbuttons
   //pb_help = qk_pushbutton( tr( "Help" ) );
   //connect( pb_help, SIGNAL( clicked() ), this, SLOT( help() ) );
   
   pb_apply = qk_pushbutton( tr( "Apply" ) );
   connect( pb_apply, SIGNAL( clicked() ), this, SLOT( apply() ) );

   pb_exit = qk_pushbutton( tr( "Exit" ) );
   connect( pb_exit, SIGNAL( clicked() ), this, SLOT( close() ) );

   QBoxLayout* buttons = new QHBoxLayout();
   buttons->addWidget( pb_apply  );
//  buttons->addWidget( pb_help   );
   buttons->addWidget( pb_exit );

   topbox->addLayout( otherSettings );
   topbox->addLayout( buttons );
   setLayout( topbox);

   update_local();

   // Place the window to the left or right of the parent
   // depending on the position on the screen
   QRect parentGeom = parent->geometry();
   int   width      = this->width();
   int   newx;
   if ( parentGeom.left() - width - 100 < 0 )
      newx = parentGeom.x() + parentGeom.width() + 100;
   else
      newx = parentGeom.x() - width - 100;

   this->move( newx, parentGeom.y() );
}

//void QK_Config::help( void )
//{
//  showhelp.show_help( "config.html" );
//}

// Relay response from update_font()
void QK_Config::sendFonts( void )
{
   // Update fonts here also 
   update_local();
   emit updateFonts();
}

// Display time help info
void QK_Config::time_help( void )
{
   QString* text = new QString(
         "Select how time is displayed.\n"
         "Format:\n"
         "h        The hour without a leading zero\n"
         "hh       The hour with a leading zero\n"
         "H        The hour without a leading zero\n"
         "HH       The hour with a leading zero\n"
         "m        The minute without a leading zero\n"
         "mm       The minute with a leading zero\n"
         "s        The whole second, without any leading zero\n"
         "ss       The whole second, with a leading zero\n"
         "AP or A  Use AM/PM display.\n"
         "ap or a  Use am/pm display.\n"
         "t        The timezone (for example 'CEST')" );

   msg_box( text, 60 );
}

// Display date help info
void QK_Config::date_help( void )
{
   QString* text = new QString(
         "Select how date is displayed.\n"
         "Format:\n"
         "d     The day as a number without a leading zero (1 to 31)\n"
         "dd    The day as a number with a leading zero (01 to 31)\n"
         "ddd   The abbreviated localized day name (e.g. 'Mon' to 'Sun')\n"
         "dddd  The long localized day name (e.g. 'Monday' to 'Sunday').\n" 
         "M     The month as a number without a leading zero (1 to 12)\n"
         "MM    The month as a number with a leading zero (01 to 12)\n"
         "MMM   The abbreviated month name (e.g. 'Jan' to 'Dec').\n"
         "MMMM  The long month name (e.g. 'January' to 'December').\n"
         "yy    The year as a two digit number (00 to 99)\n"
         "yyyy  The year as a four digit number.\n" );

   msg_box( text, 60 );
}

// Display cpu help info
void QK_Config::cpu_help( void )
{
   QString* text = new QString(
         "Select whether CPU Load and/or CPU Progress bar is displayed." );

   msg_box( text, 45 );
}

// Display uptime help info
void QK_Config::uptime_help( void )
{
   QString* text = new QString(
         "Select whether uptime is displayed." );

   msg_box( text, 45 );
}

// Display memory usage help info
void QK_Config::memory_help( void )
{
   QString* text = new QString(
         "Select whether memory usage is displayed." );

   msg_box( text, 45 );
}

// Present text in a QMessageBox with specified width in characters
void QK_Config::msg_box( QString* text, int width )
{
   // Trying to get the message box wider than about 60 characters 
   // does not seem to work
   QFont        font     = QFontDatabase::systemFont(QFontDatabase::FixedFont);
   int          boxWidth = QFontMetrics( font ).averageCharWidth() * width;
   QMessageBox  MB;
   QSpacerItem* spacer   = new QSpacerItem( boxWidth, 0, QSizePolicy::Minimum, 
                                            QSizePolicy::Expanding );
   
   MB.setText( *text );
   MB.setFont( font );
   QGridLayout* MBlayout = (QGridLayout*)MB.layout();
   MBlayout->addItem( spacer, MBlayout->rowCount(), 0, 1, MBlayout->columnCount() );
   MB.exec();
}

// Relay response from update_colors()
void QK_Config::sendColors( void )
{
   //QMessageBox::information(this, "Test", "qk_config received update colors" );
   emit updateColors();
}

// Relay response from update_Temps()
void QK_Config::sendTemps( void )
{
   //QMessageBox::information(this, "Test", "qk_config received update temps" );
   emit updateTemps();
}

void QK_Config::update_font( void )
{
   QK_Font* font = new QK_Font();  
   font->show();
   connect( font, SIGNAL( updateFonts() ), this, SLOT( sendFonts() ) );
}

void QK_Config::update_colors( void )
{
   QK_Color* colors = new QK_Color();  
   //colors->setModality( Qt::WindowModal );  
   colors->show();
   connect( colors, SIGNAL( updateColors() ), this, SLOT( sendColors() ) );
}

void QK_Config::update_temps( void )
{
   QK_Temps* temps = new QK_Temps();  
   temps->show();
   connect( temps, SIGNAL( updateTemps() ), this, SLOT( sendTemps() ) );
}

void QK_Config::update_local( void )
{
   // Update local widgets

   QFont font = QFont( settings.value( "fontFamily", "DejaVu Sans" ).toString(),
                       settings.value( "fontSize"  , 12 ).toInt() );
   
   version ->setFont( font );
   pb_font ->setFont( font );
   pb_color->setFont( font );
   pb_temps->setFont( font );
   pb_apply->setFont( font );
   pb_exit ->setFont( font );

   CBtime  ->setFont( font );
   CBdate  ->setFont( font );
   CBuptime->setFont( font );
   CBcpu   ->setFont( font );
   CBcpuBar->setFont( font );
   CBmemory->setFont( font );

   PBtime  ->setFont( font );
   PBdate  ->setFont( font );
   PBuptime->setFont( font );
   PBcpu   ->setFont( font );
   PBmemory->setFont( font );

   LEtime  ->setFont( font );
   LEdate  ->setFont( font );

   lbl_font ->setFont( font );
   lbl_color->setFont( font );
   lbl_temps->setFont( font );
}

void QK_Config::apply( void )
{
   settings.setValue( "useTime",   CBtime->isChecked()   );
   settings.setValue( "useDate",   CBdate->isChecked()   );
   settings.setValue( "useUptime", CBuptime->isChecked() );
   settings.setValue( "useCPU",    CBcpu->isChecked()    );
   settings.setValue( "useCPUbar", CBcpuBar->isChecked() );
   settings.setValue( "useMemory", CBmemory->isChecked() );


   settings.setValue( "timeFormat", LEtime->text() );
   settings.setValue( "dateFormat", LEdate->text() );
   settings.sync();

   emit updateEntries();
}



