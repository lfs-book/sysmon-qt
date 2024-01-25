#include <QAction>
#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QHostInfo>
#include <QRegularExpression>
#include <QTime>
#include <QTimer>
// #include <QMessageBox>

#include "sysmon-qt.h"
#include "sm_config.h"

int main(int argc, char *argv[])
{  
   //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
           
   QApplication a(argc, argv);
   QCoreApplication::setOrganizationName("LinuxFromScratch");
   QCoreApplication::setApplicationName("sysmon-qt");
   QCoreApplication::setApplicationVersion(QT_VERSION_STR);

   sysmon_qt mainWin;
   mainWin.show();

   return a.exec();
}  

sysmon_qt::sysmon_qt()
{
   //mFrame = false;
   setFrame();

   // Intialize font
   font_family = settings.value( "fontFamily", "DejaVu Sans" ).toString();
   font_size   = settings.value( "fontSize"  , 12 ).toInt();
   font_normal = QFont( font_family, font_size );

   // Set up palettes
   set_palettes();

   // timer
   QTimer* timer = new QTimer(this);
   timer->setInterval(1000);
   connect(timer, &QTimer::timeout, this, QOverload<>::of(&sysmon_qt::update));
   timer->start();

   // actions 
   QAction* configAction = new QAction( tr("Configuration"), this );
   configAction->setShortcut( Qt::Key_F1 );
   connect( configAction, SIGNAL(triggered()), this, SLOT( config() ) );
   addAction( configAction );

   QAction* quitAction = new QAction(tr("E&xit"), this);
   quitAction->setShortcut(tr("Ctrl+Q"));
   connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
   addAction(quitAction);

   QAction* frameAction = new QAction( tr("F&rame"), this );
   frameAction->setShortcut( tr("Ctrl+F") );
   connect( frameAction, &QAction::triggered, this, &sysmon_qt::changeFrame );
   addAction( frameAction );

   // Start layout   
   layout = new QVBoxLayout;

   // hostname
   QString hostname = QHostInfo::localHostName();
   lbl_hostname = banner( hostname, 0, QFont::Bold );
   lbl_hostname->setPalette( banner_palette);
   layout->addWidget( lbl_hostname );

   // Set up all entries
   setup_all();

   // End of layout
   QWidget* window = new QWidget();
   window->setLayout( layout );

   setCentralWidget( window );

   // Restore window position
   if ( settings.contains( "positionX" ) )
   {
      int x = settings.value( "positionX" ).toInt();
      int y = settings.value( "positionY" ).toInt();
      position = QPoint( x, y );
      this->move( position );
   }

   setContextMenuPolicy(Qt::ActionsContextMenu);  // Add actions for right click
   setWindowTitle(tr("sysmon_qt"));
}

sysmon_qt::~sysmon_qt()
{
   position = this->pos();
   settings.setValue( "positionX", QString::number( position.x() ) );
   settings.setValue( "positionY", QString::number( position.y() ) );
   settings.sync();
}

void sysmon_qt::setup_all()
{
   tick = 0;
   setup_time   ();
   setup_date   ();
   setup_uptime ();
   setup_cpuLoad();
   setup_memory ();

   set_tempsLayout();
   if ( tempsLayout != nullptr )
   {
      layout->addLayout( tempsLayout );
      pauseTempUpdate = false;
      update_temps();
   }
}

void sysmon_qt::setup_time()
{
   lbl_time = nullptr;
   if ( ! settings.value( "useTime", true ).toBool() ) return;

   lbl_time = label( " " );
   layout->addWidget( lbl_time );  
   update_time();
}

void sysmon_qt::update_time( void )
{
   if ( lbl_time == nullptr ) return; 

   // Get the time format
   QString format = settings.value( "timeFormat", "HH:mm:ss" ).toString();

   QTime time = QTime::currentTime();
   lbl_time->setText( time.toString( format ) );
} 

void sysmon_qt::setup_date()
{
   lbl_date = nullptr;
   if ( ! settings.value( "useDate", true ).toBool() ) return;

   lbl_date = label( " " );
   layout->addWidget( lbl_date );  
   update_date();
}

void sysmon_qt::update_date( void )
{
   if ( lbl_date  == nullptr ) return;  
   if ( tick % 60 != 0       ) return;  // Update once a minute

   QDateTime dateTime = QDateTime::currentDateTime();

   // Get the date format
   QString format = settings.value( "dateFormat", "ddd d MMM" ).toString();

   QString date   = dateTime.date().toString( format );
   lbl_date->setText( date );
} 

void sysmon_qt::setup_uptime()
{
   lbl_uptime = nullptr;
   if ( ! settings.value( "useUptime", true ).toBool() ) return;

   lbl_uptime = label( " " );
   layout->addWidget( lbl_uptime );  
   update_uptime();
}

void sysmon_qt::update_uptime( void )
{
   if ( lbl_uptime == nullptr ) return;
   if ( tick % 60  != 0       ) return;  // Update once a minute

   QFile file( "/proc/uptime" );
   file.open( QIODevice::ReadOnly | QIODevice::Text);
   QByteArray ba = file.readLine();
   file.close();

   QString lineString = QString( ba );
   QString upSecs = lineString.left( lineString.indexOf(".") );
   
   uint secs    = upSecs.toInt();
   uint minutes = (secs / 60  ) % 60;
   uint hours   = (secs / 3600) % 24;
   uint days    = secs / (3600 * 24);

   QString uptime = QStringLiteral( "%1d %2:%3" )
      .arg(days)
      .arg(hours,  2, 10, QChar('0') )
      .arg(minutes,2, 10, QChar('0') );

   lbl_uptime->setText( uptime );
} 

void sysmon_qt::setup_cpuLoad()
{
   bool useCPU    = settings.value( "useCPU",    true ).toBool();
   bool useCPUbar = settings.value( "useCPUbar", true ).toBool();
   
   lbl_cpu = nullptr;
   
   if ( ! useCPU && ! useCPUbar ) return;

   if ( useCPU )
   {
      lbl_loads = label( " " );
      layout->addWidget( lbl_loads );
   }

   if ( useCPUbar )
   {
      load = new QProgressBar();
      load->setRange( 0, 100 );
      load->setValue( 0 );

      // Set palette
      QPalette p = load->palette();

      QString color = settings.value( "progressColor", "#308cc6" ).toString();
      QString bg    = settings.value( "progressBg",    "#ffffff" ).toString();

      p.setColor( QPalette::Active,   QPalette::Highlight, QColor( color ) );
      p.setColor( QPalette::Active,   QPalette::Base,      QColor( bg    ) );
      p.setColor( QPalette::Inactive, QPalette::Highlight, QColor( color ) );
      p.setColor( QPalette::Inactive, QPalette::Base,      QColor( bg    ) );
      load->setPalette( p );

      layout->addWidget( load );
   }

   lbl_cpu = banner( "CPU Load", 0, QFont::Bold );
   layout->addWidget( lbl_cpu   );
   update_cpu();
}

void sysmon_qt::update_cpu( void )
{
   if ( tick % cpu_refresh != 0 ) return;

   if ( lbl_loads != nullptr )
   {
      QFile file( "/proc/loadavg" );
      file.open( QIODevice::ReadOnly | QIODevice::Text);
      QByteArray ba2 = file.readLine();
      file.close();

      QString lineString   = QString( ba2 );
      QStringList loadList = lineString.split( QLatin1Char(' ') );
      QString CPU_loads    = loadList[0] + " " +
                             loadList[1] + " " +
                             loadList[2];
      lbl_loads->setText( CPU_loads );
   }

   if ( load != nullptr )
      load->setValue( cpuPercentage );
} 

void sysmon_qt::setup_memory()
{
   lbl_memory = nullptr;
   if ( ! settings.value( "useMemory", true ).toBool() ) return;

   memory = new QProgressBar();
   memory->setRange( 0, 100 );

   // set palette
   QPalette p = memory->palette();

   QString color = settings.value( "progressColor", "#308cc6" ).toString();
   QString bg    = settings.value( "progressBg",    "#ffffff" ).toString();

   p.setColor( QPalette::Active,   QPalette::Highlight, QColor( color ) );
   p.setColor( QPalette::Active,   QPalette::Base,      QColor( bg    ) );
   p.setColor( QPalette::Inactive, QPalette::Highlight, QColor( color ) );
   p.setColor( QPalette::Inactive, QPalette::Base,      QColor( bg    ) );
   memory->setPalette( p );

   lbl_memory = banner( "Memory Use", 0, QFont::Bold );
   layout->addWidget( memory );
   layout->addWidget( lbl_memory );

   update_memory();
}

void sysmon_qt::update_memory( void )
{
   if ( lbl_memory            == nullptr ) return;
   if ( tick % memory_refresh != 0       ) return;

   QFile file( "/proc/meminfo" );
   file.open( QIODevice::ReadOnly | QIODevice::Text);
   QTextStream textStream( &file );

   QString MemTotal;
   QString MemAvailable;

   while ( true )
   {
     QString line = textStream.readLine();
     if ( line.contains( "MemTotal"     ) ) MemTotal = line;
     if ( line.contains( "MemAvailable" ) ) 
     {
        MemAvailable = line;
        break;
     }
   }
   file.close();
    
   QRegularExpression re( "\\d+" );
   QRegularExpressionMatch match = re.match( MemTotal );
   int mem_total = match.captured(0).toInt();

   match = re.match( MemAvailable );
   int mem_avail = match.captured(0).toInt();
      
   int percent = ( (mem_total - mem_avail ) * 100 ) / mem_total;
  
   memory->setValue( percent );
}

QGridLayout* sysmon_qt::set_tempsLayout( void)
{
   settings.beginGroup( "temperatures" );
   QStringList keys = settings.childKeys();
   tempsCount       = keys.count();
   tempsLayout      = nullptr;;
   tempDesc.clear();
   degrees.clear();

   // Set temperature entries
   if ( tempsCount > 0 )
   {  
      tempsLayout = new QGridLayout();

      for ( int i = 0; i < tempsCount; i++ )
      {
         QString     t       = keys[ i ]; // temperature key
         QString     v       = settings.value( t ).toString();
         QStringList v_split = v.split( ',' );
         QString     t_label = v_split[ 3 ];
         // Extra spaces were added for sorting.  Remove them here.
         QString     sensor  = v_split[ 1 ].replace( "  ", " " );
         tempDesc.append( v_split[ 0 ] + "," + v_split[ 1 ] );
         
         QLabel* t_lbl_temp = textlabel( t_label );
         //QLabel* degC       = new QLabel( "??°C" );
         QLabel* degC       = textlabel( "??°C" );
         degrees.append( degC );

         tempsLayout->addWidget( t_lbl_temp, i, 0 );
         tempsLayout->addWidget( degC,       i, 1 );
      }
   }

   settings.endGroup();
   return tempsLayout;
}

void sysmon_qt::setFrame()
{
   if ( mFrame )
     setWindowFlags( Qt::Window );  // Turn on frame
   else
     setWindowFlags( Qt::Window | Qt::FramelessWindowHint ); //Frame off
}

void sysmon_qt::changeFrame()
{
   mFrame = ! mFrame;
   setFrame();
   show();
}

QLabel* sysmon_qt::label( const QString& labelString, int fontAdjust, int weight )
{
   QLabel* newLabel = new QLabel( labelString, this );
  
   newLabel->setFrameStyle( QFrame::StyledPanel | QFrame::Raised );
   newLabel->setAlignment ( Qt::AlignVCenter | Qt::AlignCenter );
   newLabel->setMargin    ( 2 );
   newLabel->setAutoFillBackground( true );

   bool bold   = settings.value( "fontBold", false ).toBool();
        weight = bold ? QFont::Bold : QFont::Normal;

   QFont labelFont = QFont( font_family, font_size + fontAdjust,  weight );

   newLabel->setFont   ( labelFont    );
   newLabel->setPalette( data_palette );

   return newLabel;
}

// textlabel ( defaults to smaller font and changes text colors )
QLabel* sysmon_qt::textlabel( const QString& labelString, int fontAdjust, int weight )
{
  QLabel* newLabel = label( labelString, fontAdjust, weight );

  newLabel->setPalette( data_palette );

  return newLabel;
}

// banner ( defaults to Bold and changes text colors )
QLabel* sysmon_qt::banner( const QString& labelString, int fontAdjust, int weight )
{
  QLabel* newLabel = label( labelString, fontAdjust, weight );

  newLabel->setAlignment ( Qt::AlignCenter );
  newLabel->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
  newLabel->setMidLineWidth( 2 );

  // Set banner colors
  newLabel->setPalette( banner_palette );

  return newLabel;
}

void sysmon_qt::config( void )
{
   SM_Config* w = new SM_Config( this );
   w->show();
   connect( w, SIGNAL( updateFonts  () ), this, SLOT( updateFont()   ) );
   connect( w, SIGNAL( updateColors () ), this, SLOT( updateColor()  ) );
   connect( w, SIGNAL( updateTemps  () ), this, SLOT( updateTemp()   ) );
   connect( w, SIGNAL( updateEntries() ), this, SLOT( updateLayout() ) );
}

void sysmon_qt::updateFont( void )
{
   font_family = settings.value( "fontFamily", "DejaVu Sans" ).toString();
   font_size   = settings.value( "fontSize"  , 12 ).toInt();
   font_normal = QFont( font_family, font_size );

   bool bold   = settings.value( "fontBold"  , false ).toBool();
   QFont::Weight weight = bold ? QFont::Bold : QFont::Normal;
   font_normal.setWeight( weight );

   QFont fontBold = QFont( font_family, font_size, QFont::Bold );

   lbl_hostname->setFont( fontBold );
   delete_all();
   setup_all();
}

void sysmon_qt::set_palettes( void )
{
   QPalette p;

   QString lblColor = settings.value( "labelColor", "#000000" ).toString();
   QString lblBg    = settings.value( "labelBg",    "#efefef" ).toString();

   p.setColor( QPalette::Active,   QPalette::WindowText, QColor( lblColor ) );
   p.setColor( QPalette::Active,   QPalette::Window,     QColor( lblBg    ) );
   p.setColor( QPalette::Inactive, QPalette::WindowText, QColor( lblColor ) );
   p.setColor( QPalette::Inactive, QPalette::Window,     QColor( lblBg    ) );

   banner_palette = p;

   QString dataColor = settings.value( "dataColor", "#000000" ).toString();
   QString dataBg    = settings.value( "dataBg",    "#efefef" ).toString();

   p.setColor( QPalette::Active,   QPalette::WindowText, QColor( dataColor ) );
   p.setColor( QPalette::Active,   QPalette::Window,     QColor( dataBg    ) );
   p.setColor( QPalette::Inactive, QPalette::WindowText, QColor( dataColor ) );
   p.setColor( QPalette::Inactive, QPalette::Window,     QColor( dataBg    ) );

   data_palette = p;
}

void sysmon_qt::updateColor( void )
{
   set_palettes();
   lbl_hostname->setPalette( banner_palette );
   delete_all();
   setup_all();
}

void sysmon_qt::updateTemp( void )
{
   // Delete the current temp layout
   delete_temps();

   // Create the new layout
   set_tempsLayout();

   if ( tempsLayout != nullptr ) layout->addLayout( tempsLayout );

   // Restart update
   pauseTempUpdate = false;
   update_temps();
}

void sysmon_qt::delete_temps( void )
{
   // First stop updating temperatures
   pauseTempUpdate = true;

   // Delete existing tempsLayout if it exists
   if ( tempsLayout != nullptr ) 
   {
      QLayoutItem* item;
      while ( ( item = tempsLayout->takeAt( 0 ) ) != nullptr )
      {
         delete item->widget();
         delete item;
      }
      delete tempsLayout;
   }

   tempsLayout = nullptr;
}

void sysmon_qt::updateLayout( void )
{
   pauseUpdate = true;
   delete_all();

   // Now rebuild and restart
   setup_all();
   pauseUpdate = false;
}

void sysmon_qt::delete_all( void )
{
  // First delete temps
  delete_temps(); 

  // Now delete all the other entries except the first (hostname)
  QLayoutItem* item;

  while ( ( item = layout->takeAt( 1 ) ) != nullptr )
  {
     delete item->widget();
     delete item;
  }

  //lbl_hostname = nullptr;  We do not delete this 
  lbl_time     = nullptr;
  lbl_date     = nullptr;
  lbl_uptime   = nullptr;
  lbl_cpu      = nullptr;     
  load         = nullptr; // cpu QProgressBar 
  lbl_loads    = nullptr;
  lbl_memory   = nullptr; 
}

void sysmon_qt::update( void )
{
   update_cpu_percentage();
   if ( pauseUpdate ) return;
   update_time();
   update_cpu();
   update_memory();
   update_uptime();
   update_date();
   update_temps();

   tick++;  // Update time every second 
}

/*!
   This is a little tricky.  The first line in /proc/stat is
   the composite of all CPUs and contains jiffies as appropriate
   for each kernel tick (many times per second).  The values are:

      user   normal processes executing in user mode
      nice   niced processes executing in user mode
      system processes executing in kernel mode
      idle   not doing anything
 
   Just calculate:
      work  = user + nice + system;
      total = work + idle;
   and get the delta from the previous iteration.  The load 
   percentage then is just ((work-oldwork)/(total-oldtotal)) * 100.
*/
void sysmon_qt::update_cpu_percentage( void )
{
   // We do this every second for now.
   // Could possibly do it every cpu_refresh time (3 seconds for now)
   static qint64      previousJiffiesWork;
   static qint64      previousJiffiesTotal;
   static QFile*      file;
   
   QString cpu;  // Not really used -- always "cpu"
   qint64  user, // jiffies
           nice, 
           system, 
           idle;

   // First time through only
   if ( previousJiffiesWork == 0 )
   {
      file = new QFile( "/proc/stat" );
      file->open( QIODevice::ReadOnly | QIODevice::Text);
      QTextStream stream( file );
      stream >> cpu >> user >> nice >> system >> idle;
      previousJiffiesWork  = user + nice + system;
      previousJiffiesTotal = previousJiffiesWork + idle;
      return;
   }

   QTextStream stream( file );
   stream.seek( 0 );  // Just read the first line each time
   stream >> cpu >> user >> nice >> system >> idle;

   qint64 work  = user + nice + system;
   qint64 total = work + idle;

   double loadFraction = (double)( work  - previousJiffiesWork  ) /
                                 ( total - previousJiffiesTotal );
 
   cpuPercentage = qRound( loadFraction * 100.0 );

   previousJiffiesWork  = work;
   previousJiffiesTotal = total;
}

void sysmon_qt::update_temps( void )
{
   if ( pauseTempUpdate ) return;

   QProcess*       process  = new QProcess(this);
   QString         program  = "sensors";
   QStringList     args     = QStringList() << "-jA";
   sensor_data.clear();
   
   process->start( program, args );
   
   process->waitForFinished( 3000 ); // Give it 3 secs
   // Need an error message here if fails
   QByteArray      output   = process->readAllStandardOutput();
   delete          process;

   QJsonParseError err;   
   QJsonDocument   doc      = QJsonDocument::fromJson( output, &err );
   // Need an error message here if fails
   
   QJsonObject root       = doc.object();
   int         count      = root.count();  // The number of interfaces
   QStringList interfaces = root.keys();   // keys are interface names
   QRegularExpression rx  = QRegularExpression( ".*_input$" );              
   
   for ( int i = 0; i < count; i++ )
   {  
      QJsonObject object       = root[ interfaces[i] ].toObject(); // iface obj
      QStringList sensor_names = object.keys();

      // count2 is the number of sensors for the interface
      int count2 = sensor_names.count();

      if ( count2 <  1 ) continue;  // Should not happen
                                    //
      for ( int j = 0; j < count2; j++ )
      {
         QJsonObject sensor_object = object[ sensor_names[ j ] ].toObject();
         QStringList sensor_params = sensor_object.keys();

         // count3 is the number of parameters for the sensor
         int         count3        = sensor_params.count();

         for ( int k = 0; k < count3; k++ )  // For each parameter
         {
            QString param = sensor_params[ k ];
            int pos = param.indexOf( rx ); 
            if ( pos < 0 ) continue;      
            // Also ignore non-temp input
            if ( ! param.contains( "temp" ) ) continue;
                                          
            QString full_sensor = interfaces   [ i ] + "," +
                                  sensor_names [ j ];
            QString found       = full_sensor + "," + param;

            const QJsonValue value = sensor_object[ param ];

            // Now save the data
            sensor_data.insert( found, value.toDouble() );
            break;
         }
      }
   }

   // Update user selected labels here
   int         dataSize     = sensor_data.size();
   QStringList keys         = sensor_data.keys();

   for ( int i = 0; i < dataSize; i++ ) 
   {
      for ( int j = 0; j < tempDesc.size(); j++ )
      {
         if ( keys[ i ].contains( tempDesc[ j ] ) )
         {
            int     temperature = sensor_data[ keys[ i ] ];
            QString newText     = QString::number( temperature ) + "°C"; 
            QLabel* lbl         = degrees[ j ];
            lbl->setText( newText );
         }
      }
   }
}

