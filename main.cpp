#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "thinkgear.h"
#include <windows.h>

/**
 * Prompts and waits for the user to press ENTER.
 */
void
wait() {
    printf( "\n" );
    printf( "Press the ENTER key...\n" );
    fflush( stdout );
    getc( stdin );
}

/**
 * Program which prints ThinkGear EEG_POWERS values to stdout.
 */
int
main( void ) {
	int len;
	int prev_time;
	int current_time;
	int iterate;
	
	int raw_data_count;

	char *timeptr = NULL;
	char dateStr[9];
	char timeStr[9];
    char *comPortName = NULL;
    int   dllVersion = 0;
    int   connectionId = 0;
    int   packetsRead = 0;
    int   errCode = 0;
	
	time_t base;
	time_t now;
	time_t temp_prev_time;

	SYSTEMTIME timetime;
	FILE *file;
	FILE *file_attention;
	FILE *file_battery;
	FILE *file_poor_signal;
	FILE *file_meditation;
	FILE *file_delta;
	FILE *file_theta;
	FILE *file_alpha1;
	FILE *file_alpha2;
	FILE *file_beta1;
	FILE *file_beta2;
	FILE *file_gamma1;
	FILE *file_gamma2;
	FILE *file_blink_strength;

	file=fopen("raw_data.csv","w");
	file_attention=fopen("attention_data.csv","w");
	file_battery=fopen("battery_data.csv","w");
	file_poor_signal=fopen("poor_signal_data.csv","w");
	file_meditation=fopen("meditation_data.csv","w");
	file_delta=fopen("delta_data.csv","w");
	file_theta=fopen("theta_data.csv","w");
	file_alpha1=fopen("alpha1_data.csv","w");
	file_alpha2=fopen("alpha2_data.csv","w");
	file_beta1=fopen("beta1_data.csv","w");
	file_beta2=fopen("beta2_data.csv","w");
	file_gamma1=fopen("gamma1_data.csv","w");
	file_gamma2=fopen("gamma2_data.csv","w");
	file_blink_strength=fopen("blink_strength_data.csv","w");

    /* Print driver version number */
    dllVersion = TG_GetDriverVersion();
    printf( "ThinkGear DLL version: %d\n", dllVersion );

    /* Get a connection ID handle to ThinkGear */
    connectionId = TG_GetNewConnectionId();
    if( connectionId < 0 ) {
        fprintf( stderr, "ERROR: TG_GetNewConnectionId() returned %d.\n", 
                 connectionId );
        wait();
        exit( EXIT_FAILURE );
    }

    /* Set/open stream (raw bytes) log file for connection */
    errCode = TG_SetStreamLog( connectionId, "streamLog.csv" );
    if( errCode < 0 ) {
        fprintf( stderr, "ERROR: TG_SetStreamLog() returned %d.\n", errCode );
        wait();
        exit( EXIT_FAILURE );
    }

    /* Set/open data (ThinkGear values) log file for connection */
    errCode = TG_SetDataLog( connectionId, "dataLog.csv" );
    if( errCode < 0 ) {
        fprintf( stderr, "ERROR: TG_SetDataLog() returned %d.\n", errCode );
        wait();
        exit( EXIT_FAILURE );
    }

    /* Attempt to connect the connection ID handle to serial port "COM5" */
    comPortName = "\\\\.\\COM1";
    errCode = TG_Connect( connectionId, 
                          comPortName, 
                          TG_BAUD_57600, 
                          TG_STREAM_PACKETS );
    if( errCode < 0 ) {
        fprintf( stderr, "ERROR: TG_Connect() returned %d.\n", errCode );
        wait();
        exit( EXIT_FAILURE );
    }

    /* Read 10 ThinkGear Packets from the connection, 1 Packet at a time */
    packetsRead = 0;
	
	base=time(NULL);

	prev_time=difftime(time(&now),base);
	current_time=prev_time;

	//GetSystemTime(&timetime);
	//printf("%s",timetime.wMilliseconds);

	temp_prev_time=base;
	raw_data_count=0;
	iterate=0;
	
	while(1)
	{
		now=time(NULL);
		current_time =difftime(now,base) ;
		if(current_time!=prev_time)
		{
			iterate=1;
			prev_time=current_time;
			printf("current time  %f \t" ,difftime(time(&now),base ));
			printf("Raw  %d \t" ,(int)TG_GetValue(connectionId, TG_DATA_RAW) );
			printf("Signal  %f \n" ,TG_GetValue(connectionId, TG_DATA_POOR_SIGNAL) );

			fprintf( file,"\n%s\n","#");
			fprintf( file_attention,"\n%s\n","#");
			fprintf( file_battery,"\n%s\n","#");
			fprintf( file_poor_signal,"\n%s\n","#");
			fprintf( file_meditation,"\n%s\n","#");
			fprintf( file_delta,"\n%s\n","#");
			fprintf( file_theta,"\n%s\n","#");
			fprintf( file_alpha1,"\n%s\n","#");
			fprintf( file_alpha2,"\n%s\n","#");
			fprintf( file_beta1,"\n%s\n","#");
			fprintf( file_beta2,"\n%s\n","#");
			fprintf( file_gamma1,"\n%s\n","#");
			fprintf( file_gamma2,"\n%s\n","#");
			fprintf( file_blink_strength,"\n%s\n","#");

			raw_data_count=0;
		}
		
		if(raw_data_count<500 && iterate >0)
		{
			//temp_prev_time=now;

			/* Attempt to read a Packet of data from the connection */
			errCode = TG_ReadPackets( connectionId, 1 );

			/* If TG_ReadPackets() was able to read a complete Packet of data... */
			if( errCode == 1 ) {
				raw_data_count++;
				packetsRead++;

				/* If attention value has been updated by TG_ReadPackets()... */
				if( TG_GetValueStatus(connectionId, TG_DATA_RAW) != 0 ) {
					/* Get and print out the updated attention value */
					fprintf( file,"%d \n", (int)TG_GetValue(connectionId, TG_DATA_RAW) );
					fprintf( file_attention,"%f \n", TG_GetValue(connectionId, TG_DATA_ATTENTION) );
					fprintf( file_battery,"%f \n", TG_GetValue(connectionId, TG_DATA_BATTERY) );
					fprintf( file_poor_signal,"%f \n", TG_GetValue(connectionId, TG_DATA_POOR_SIGNAL) );
					fprintf( file_meditation,"%f \n", TG_GetValue(connectionId, TG_DATA_MEDITATION) );
					fprintf( file_delta,"%f \n", TG_GetValue(connectionId, TG_DATA_DELTA) );
					fprintf( file_theta,"%f \n", TG_GetValue(connectionId, TG_DATA_THETA) );
					fprintf( file_alpha1,"%f \n", TG_GetValue(connectionId, TG_DATA_ALPHA1) );
					fprintf( file_alpha2,"%f \n", TG_GetValue(connectionId, TG_DATA_ALPHA2) );
					fprintf( file_beta1,"%f \n", TG_GetValue(connectionId, TG_DATA_BETA1) );
					fprintf( file_beta2,"%f \n", TG_GetValue(connectionId, TG_DATA_BETA2) );
					fprintf( file_gamma1,"%f \n", TG_GetValue(connectionId, TG_DATA_GAMMA1) );
					fprintf( file_gamma2,"%f \n", TG_GetValue(connectionId, TG_DATA_GAMMA2) );
					fprintf( file_blink_strength,"%f \n", TG_GetValue(connectionId, TG_DATA_BLINK_STRENGTH) );

				} /* end "If attention value has been updated..." */

			 } /* end "If a Packet of data was read..." */
		}
    } /* end "Read 10 Packets of data from connection..." */
	fclose(file);
    /* Clean up */
    TG_FreeConnection( connectionId );

    /* End program */
    wait();
    return( EXIT_SUCCESS );
}