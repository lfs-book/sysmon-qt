//! \file qk_font.cpp
#include "Qkrellm.h"
#include "qk_temps.h"

QK_Temps::QK_Temps() 
{
   // Get current font; second parameter is default
   QString family  = settings.value( "fontFamily", "DejaVu Sans" ).toString();
   int     size    = settings.value( "fontSize"  , 12 ).toInt();

   QFont   font    = QFont( family, size, QFont::Normal );

   // Frame layout
   setWindowTitle( "Temperature Selection Dialog" );
 
   QVBoxLayout* topbox = new QVBoxLayout( this );
   topbox->setSpacing( 2 );

   get_temp_data();

   int row = 0;
   tempsLayout = new QGridLayout();

   QLabel* lblInterface = qk_banner( "Interface" );
   QLabel* lblDevice    = qk_banner( "Device" );
   QLabel* lblSelected  = qk_banner( "Selected" );
   QLabel* lblLabel     = qk_banner( "Label" );
   
   tempsLayout->addWidget( lblInterface, row,   0 );
   tempsLayout->addWidget( lblDevice,    row,   1 );
   tempsLayout->addWidget( lblSelected,  row,   2 );
   tempsLayout->addWidget( lblLabel,     row++, 3 );

   // Sort Core devices
   for ( int i = 0; i < sensors.size(); i++ )
   {
     QStringList items  = sensors[ i ].split( "," );
     QString     device = items[ 1 ];

     if ( device.contains( "Core" ) )
     {
       QStringList c = device.split( " " );
       if ( c[ 1 ].size() == 1 ) device = "Core  " + c[ 1 ];
       sensors[ i ] = items[ 0 ] + "," + device + "," + items[ 2 ];
     }
   }
   
   sensors.sort();

   // Add temperature data
   settings.beginGroup( "temperatures" );
   QStringList keys = settings.childKeys();
   QStringList saved_temps;
   for ( int i = 0; i < keys.count(); i++ )
      saved_temps.append( settings.value( keys[ i ] ).toString() );
   settings.endGroup();

   // For each sensor (but skip non-termerature sensors):
   for ( int i = 0; i < sensors.size(); i++ )
   {
      QString Sensor    = sensors[ i ];
      QStringList data  = Sensor.split( ',' );
      QString interface = data[ 0 ];
      QString sensor    = data[ 1 ];
      bool    isChecked = false;

      // Make the label the first parts of the interface and label 
      QString     label = data[ 2 ];
      QStringList lbl_1 = interface.split( "-" );
      QStringList lbl_2 = label    .split( "_" );
      label = lbl_1[ 0 ] + "-" + lbl_2[ 0 ];

      if ( ! label.contains( "temp" ) ) continue;

      // label needs to change depending on settings
      for ( int j = 0; j < keys.count(); j++ )
      {
         QString key = interface + "," + sensor;
         if ( saved_temps[ j ].contains( key ) )
         {
            QStringList selected = saved_temps[ j ].split( ',' );
            label = selected[ 3 ];
            isChecked = true;
            break;
         }
      }

      QCheckBox* cbBox = new QCheckBox();
      // Check with settings to see if this needs to be checked here
      cbBox->setChecked( isChecked );
      checkboxes.append( cbBox );

      lblInterface = qk_label( interface );
      lblInterface->setFont( font );

      lblDevice    = qk_label( sensor    );
      lblDevice->setFont( font );
      
      QLineEdit* le_label  = qk_lineedit( label, 0 );
      lblLabel->setFont( font );
      labels.append( le_label );

      tempsLayout->addWidget( lblInterface, row,   0 );
      tempsLayout->addWidget( lblDevice,    row,   1 );
      tempsLayout->addWidget( cbBox,        row,   2, Qt::AlignHCenter );
      tempsLayout->addWidget( le_label,     row++, 3 );
   }

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

   topbox->addLayout( tempsLayout );
   topbox->addLayout( buttons );
}

void QK_Temps::get_temp_data( void )
{  
   QProcess*       process  = new QProcess(this);
   QString         program  = "sensors";
   QStringList     args     = QStringList() << "-jA";
   
   process->start( program, args );
   
   process->waitForFinished( 3000 ); // Give it 3 secs
   QByteArray      output   = process->readAllStandardOutput();
   QJsonParseError err;   
   QJsonDocument   doc      = QJsonDocument::fromJson( output, &err );

   QJsonObject     root       = doc.object();
   int             count      = root.count();  // The number of interfaces
   QStringList     interfaces = root.keys();   // keys are interface names
   QRegExp         rx( ".*_input$" );
      
   for ( int i = 0; i < count; i++ )
   {  
      QJsonObject object       = root[ interfaces[i] ].toObject(); // iface obj
      QStringList sensor_names = object.keys();

      // count2 is the number of sensors for the interface
      int count2 = sensor_names.count();

      if ( count2 <  1 ) continue;  // Should not happen
      
      for ( int j = 0; j < count2; j++ )
      {
         QJsonObject sensor_object = object[ sensor_names[ j ] ].toObject();
         QStringList sensor_params = sensor_object.keys();

         // count3 is the number of parameters for the sensor
         int count3                = sensor_params.count();

         for ( int k = 0; k < count3; k++ )
         {
            QString param = sensor_params[ k ];
            
            // We only want temperature input parameters
            if ( ! param.contains( "temp" ) ) continue;
            
            int pos       = param.indexOf( rx );
            if ( pos < 0 ) continue;

            QString found = interfaces  [ i ] + "," +
                            sensor_names[ j ] + "," +
                            param;
            sensors << found;
            break;
         }
      }
   }
}

void QK_Temps::apply()
{
   QString group = QString( "temperatures" );
   settings.beginGroup( group );
   settings.remove( "" );                 // Removes the group, and all it keys
   settings.endGroup();

   settings.beginGroup( group ); // Start over

   int index = tempsLayout->rowCount() - 1 ;

   for ( int i = 0; i < index ; i++ )
   {
      QCheckBox* cb      = checkboxes[ i ];
      bool       checked = cb->isChecked();
      if ( ! checked ) continue;

      // Get line edit entry, but remove any commas
      QString    le      = labels[ i ]->text();
      le.remove( "," );

      // Set temperature entry in settings
      QString    key     = "temp" + QString::number( i );

      // Get entry from sensors + grid [ i, 3 ];
      settings.setValue( key, sensors[ i ] + "," + le );
   }

   settings.endGroup();
   settings.sync();
   emit updateTemps();
}

//void QK_Font::help()
//{
// US_Help* help = new US_Help();
// help->show_help( "manual/usfont.html" );
//}

