/****************************************************************
Copyright (C) 2019 Kevin Allen

This file is part of laser_stimulation

laser_stimulation is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

laser_stimulation is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with laser_stimulation.  If not, see <http://www.gnu.org/licenses/>.

laser_stim_4hz is a simplified laser_stimulation program to specifically targets 4 Hz stimulation.

date 14.09.2019
************************************************************************/
#include "main.h"
#define DEBUG_4HZ

void print_options();
int main(int argc, char *argv[])
{
  char * prog_name=PACKAGE_NAME; // defined in config.h
  char * version=PACKAGE_VERSION;
  char * copyright_notice=PACKAGE_COPYRIGHT;
  char * package_bugreport=PACKAGE_BUGREPORT;
  int non_opt_arguments; // given by user
  int num_arg_needed=2; // required by program
  // flag for each option
  int with_h_opt=0;
  int with_v_opt=0;
  int with_P_opt=0;
  int with_H_opt=0;
  int with_l_opt=0;
  int with_m_opt=0;
  int with_M_opt=0;
  int with_r_opt=0;
  int with_f_opt=0;
  int with_F_opt=0;
  int with_q_opt=0;

  int with_w_opt=0;
  int with_X_opt=0;
  int with_y_opt=0;
  int with_z_opt=0;
  
  int with_o_opt=0;
  int with_c_opt=0;
  int with_x_opt=0;
  int with_d_opt=0;
  int with_a_opt=0;

  // variables to store values passed with options
  double stimulation_phase=90;
  double pulse_length_ms=200;
  int minimum_pulse_length_ms=10;
  int maximum_pulse_length_ms=20;
  double refractory_ms=5;
  double minimum_filter_freq=2;
  double maximum_filter_freq=6;
  double stimulation_phase_deviation=30;
  double power_threshold=5;
  
  // variables to do the fft
  int fft_length=16384;
  int fft_data_length_in_fft=10000;
  double power_window_size_ms= 500;
  int power_window_size;
  int sampling_rate=20000;
  
  char * dat_file_name;
  int channels_in_dat_file=32;
  int offline_channel=0;
  
  int device_index_for_stimulation=DEVICE_INDEX_FOR_STIMULATION;  
  double ttl_amplitude_volt=3;// for ttl pulse

  // to get the options
  int opt; 
  int i;
  
  // get the options
  while (1)
    {
      static struct option long_options[] =
	{
	  {"help", no_argument,0,'h'},
	  {"version", no_argument,0,'v'},

	  {"power",required_argument,0,'P'},
	  {"phase",required_argument,0,'H'},
	  
	  {"pulse_length",required_argument,0,'l'},
	  {"minimum_pulse_length_ms", required_argument,0,'m'},
	  {"maximum_pulse_length_ms", required_argument,0,'M'},
	  {"refractory", required_argument,0,'r'},
	  {"minimum_filter_freq", required_argument,0,'f'},
	  {"maximum_filter_freq", required_argument,0,'F'},
	  {"stimulation_phase_deviation", required_argument,0,'q'},

	  {"window_fft_total_length", required_argument,0,'w'},
	  {"window_fft_data_length", required_argument,0,'X'},
	  {"window_fft_power_ms", required_argument,0,'y'},
	  {"sampling_rate", required_argument,0,'z'},
	  
	  
	  {"offline", required_argument,0,'o'},
	  {"channels_in_dat_file", required_argument,0,'c'},
	  {"offline_channel", required_argument,0,'x'},

	  {"output_device_for_stimulation", required_argument,0,'d'},
	  {"ttl_amplitude_volt", required_argument,0,'a'},
	  {0, 0, 0, 0}
	};
      int option_index = 0;
      opt = getopt_long (argc, argv, "hvP:H:l:m:M:r:f:F:q:w:X:y:z:o:c:x:d:a:",
			 long_options, &option_index);
      
      /* Detect the end of the options. */
      if (opt == -1)
	break;
      
      switch (opt)
	{
	case 0:
	  /* If this option set a flag, do nothing else now. */
	  if (long_options[option_index].flag != 0)
	    break;
	  printf ("option %s", long_options[option_index].name);
	  if (optarg)
	    printf (" with arg %s", optarg);
	  printf ("\n");
	  break;
	case 'h':
	  {
	    with_h_opt=1;
	    break;
	  }
	case 'v':
	  {
	    with_v_opt=1;
	    break;
	  }
	case  'P':
	  {
	    with_P_opt=1;
	    power_threshold=atof(optarg);
	    break;
	  }
	case 'H':
	  {
	    with_H_opt=1;
	    stimulation_phase=atof(optarg);
	    break;
	  }
	case 'l':
	  {
	    with_l_opt=1;
	    pulse_length_ms=atof(optarg);
	    break;
	  }
	case  'm':
	  {
	    with_m_opt=1;
	    minimum_pulse_length_ms=atoi(optarg);
	    break;
	  }	  
	case  'M':
	  {
	    with_M_opt=1;
    	    maximum_pulse_length_ms=atoi(optarg);
	    break;
	  }
	case  'r':
	  {
    	    with_r_opt=1;
	    refractory_ms=atof(optarg);
	    break;
	  }
	case  'f':
	  {
	    with_f_opt=1;
	    minimum_filter_freq=atof(optarg);
	    break;
	  }	  
	case  'F':
	  {
	    with_F_opt=1;
	    maximum_filter_freq=atof(optarg);
	    break;
	  }	  
	case  'q':
	  {
	    with_q_opt=1;
	    stimulation_phase_deviation=atof(optarg);
	    break;
	  }	  
	case 'w':
	  {
	    with_w_opt=1;
	    fft_length=atoi(optarg);
	    break;
	  }
	case 'X':
	  {
	    with_X_opt=1;
	    fft_data_length_in_fft=atoi(optarg);
	    break;
	  }
	case 'y':
	  {
	    with_y_opt=1;
	    power_window_size_ms=atof(optarg);
	    break;
	  }
	case 'z':
	  {
	    with_z_opt=1;
	    sampling_rate=atoi(optarg);
	    break;
	  }
	case  'o':
	  {
	    with_o_opt=1;
	    dat_file_name=optarg;
	    break;
	  }	  
	case  'c':
	  {
	    with_c_opt=1;
	    channels_in_dat_file=atoi(optarg);
	    break;
	  }	  
	case  'x':
	  {
	    with_x_opt=1;
	    offline_channel=atoi(optarg);
	    break;
	  }	  

	case  'd':
	  {
	    with_d_opt=1;
	    device_index_for_stimulation=atoi(optarg);
	    break;
	  }	  
	case  'a':
	  {
	    ttl_amplitude_volt=atof(optarg);
	    break;
	  }	  
	case '?':
	  /* getopt_long already printed an error message. */
	  //	  break;
	  
	default:
	  return 1;
	}
    }
  
  if (with_v_opt)
    {
      printf("%s %s\n",prog_name, version);
      printf("%s\n",copyright_notice);
      printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n");
      return 0;
    }
  if (with_h_opt)
    {
      printf("%s is a program to do laser brain stimulation\n",prog_name);
      printf("Usage for %s is \n", prog_name);
      printf("%s trial_duration_sec laser_intensity_volts\n",prog_name);
      print_options();
      printf("report bugs at %s\n",package_bugreport);
      return 0;
    }

  // no -v or -h, can continue
  non_opt_arguments=argc-optind; // number of non-option argument required
  if ((non_opt_arguments) !=  num_arg_needed)
    {
      fprintf(stderr,"Usage for %s is \n", prog_name);
      fprintf(stderr,"%s trial_duration_sec laser_intensity_volts\n",argv[0]);
      print_options();
      fprintf(stderr,"You need %d arguments but gave %d arguments: \n",num_arg_needed,non_opt_arguments);
      for (i = 1; i < argc; i++)
	{
	  fprintf(stderr,"%s\n", argv[i]);
	}
      return (1);
    }  
  

  // variables read from arguments
  double laser_intensity_volt=1; // for laser power
  double baseline_volt=0; // for ttl pulse


  // same as above variables, but in comedi value instead of volts
  lsampl_t comedi_intensity =0;
  lsampl_t comedi_baseline =0;
  lsampl_t comedi_pulse=0;


  double power_4hz;
  double max_phase_diff=5;
  double frequency_for_phase=(minimum_filter_freq+maximum_filter_freq)/2; // use mid point of band pass filter
  double degree_duration_ms=(1000/frequency_for_phase)/360;
  
  // structure with variables about the comedi device, see main.h for details
  struct comedi_interface comedi_inter;

  // structure with variables to do the filtering of signal
  struct fftw_interface_4hz fftw_inter;

  // structure with variables to do time keeping
  struct time_keeper tk;



  struct data_file_si data_file; // data_file_short_integer

  // list of channels to scan, bluff, we will sample the same channel many times
  int channel_list[NUMBER_SAMPLED_CHANNEL_DEVICE_0];
  long int last_sample_no=0;
  double current_phase=0;
  double phase_diff;

  power_window_size=(int)(power_window_size_ms*sampling_rate/1000);
  
  // parce the arguments from the command line
  tk.trial_duration_sec=atof(argv[optind]);
  laser_intensity_volt=atof(argv[optind+1]);


  tk.duration_sleep_when_no_new_data=set_timespec_from_ms(SLEEP_WHEN_NO_NEW_DATA_MS);
  tk.pulse_duration_ms=pulse_length_ms;
  tk.duration_pulse=set_timespec_from_ms(tk.pulse_duration_ms);

  
  // variables to work offline from a dat file
  int new_samples_per_read_operation=60; //  3 ms of data
  short int* data_from_file;
  short int* ref_from_file;
  long int trial_iteration=0;

  /*************************************************************
   block to check if the options and arguments make sense
  *************************************************************/
  if(with_P_opt==1&&with_H_opt==1)
    {
      fprintf(stderr,"%s: can't run with both -P and -H options, choose one\n",prog_name);
      return 1;
    }
  if(with_P_opt==0&&with_H_opt==0)
    {
      fprintf(stderr,"%s: you need to run with either -P or -H options\n",prog_name);
      return 1;
    }
  if(with_H_opt)
    {
      if(stimulation_phase<0|| stimulation_phase>=360)
	{
	  fprintf(stderr,"%s: stimulation_phase should be >= 0 and < than 360\n",prog_name);
	  return 1 ;
	}
    }
  if(with_l_opt&&with_m_opt)
    {
      fprintf(stderr,"%s: can't run with both -l and -m options, choose one\n",prog_name);
      return 1;
    }
  if(with_l_opt)
    if(pulse_length_ms<0)
      {
	fprintf(stderr,"%s: pulse_length_ms should be a positive number\n",prog_name);
	return 1;
      }

  if(with_m_opt!=with_M_opt)
    {
      fprintf(stderr,"%s: if you use -m, you need to use -M as well\n",prog_name);
      return 1;
    }
    
  if(minimum_pulse_length_ms<=0)
    {
      fprintf(stderr,"%s: minimum_pulse_length_ms should be larger than 0\n",prog_name);
      return 1;
    }
  if(maximum_pulse_length_ms<=0)
    {
      fprintf(stderr,"%s: minimum_pulse_length_ms should be larger than 0\n",prog_name);
      return 1;
    }
  if(minimum_pulse_length_ms>=maximum_pulse_length_ms)
    {
      fprintf(stderr,"%s: minimum_pulse_length_ms should be smaller than maximum_pulse_length_ms\n",prog_name);
      return 1;
    }
  if(with_r_opt)
    if(refractory_ms<0)
      {
	fprintf(stderr,"%s: refractory_ms should not be a negative value\n",prog_name);
	return 1;
      }
  if(with_f_opt!=with_F_opt)
    {
      fprintf(stderr,"%s: -f and -F should be used together, or not used\n",prog_name);
      return 1;
    }
  if(minimum_filter_freq<=0)
    {
      fprintf(stderr,"%s: minimum_filter_freq should be larger than 0\n",prog_name);
      return 1;
    }
  if(maximum_filter_freq<=0)
    {
      fprintf(stderr,"%s: maximum_filter_freq should be larger than 0\n",prog_name);
      return 1;
    }
  if(minimum_filter_freq>=maximum_filter_freq)
    {
      fprintf(stderr,"%s: minimum_filter_freq should be smaller than maximum_filter_freq\n",prog_name);
      return 1;
    }
  if(with_q_opt)
    if(stimulation_phase_deviation<=0){
      fprintf(stderr,"%s: stimulation_phase_deviation should be larger than 0\n",prog_name);
      return 1;
    }

  if (tk.trial_duration_sec<1 || tk.trial_duration_sec > 10000)
    {
      fprintf(stderr,"%s: trial duration should be between 1 and 10000 sec\nYou gave %lf\n",prog_name,tk.trial_duration_sec);
      return 1;
    }

  if(laser_intensity_volt<=0 || laser_intensity_volt>4)
    {
      fprintf(stderr,"%s: voltage to control laser power should be between 0 and 4 volt\nYou gave %lf\n",prog_name,laser_intensity_volt);
      return 1;
    }
  if(with_o_opt!=with_c_opt&&with_o_opt!=with_x_opt)
	{
	  fprintf(stderr,"%s: option -o requires the use of -c and -x.\n", prog_name);
	  return 1;
	}
  if(with_c_opt)
    {
      if(channels_in_dat_file < 0)
	{
	  fprintf(stderr,"%s: channels_in_dat_file should be larger than 0 but is %d.\n", prog_name,channels_in_dat_file);
	  return 1;
	}
    }
  if(with_x_opt)
    {
      if(offline_channel<0 || offline_channel>=channels_in_dat_file)
	{
	  fprintf(stderr,"%s: offline_channel should be from 0 to %d but is %d.\n", prog_name,channels_in_dat_file-1,offline_channel);
	  return 1;
	}
    }
  if(with_d_opt)
    {
      if(device_index_for_stimulation<0)
	{
	  fprintf(stderr,"%s: device_index_for_stimulation should not be negative but is %d.\n", prog_name,device_index_for_stimulation);
	  return 1;
	}
    }
  
  if(fft_length<100||fft_length> 2000000)
    {
      fprintf(stderr,"%s: fft_length is outside of possible range (256,131072): %d.\n", prog_name,fft_length);
      fprintf(stderr,"fft_length should be 2 to the power of ... \nYou gave %d\n",fft_length);
      fprintf(stderr,"try one of the following values\n");
      for (i = 8; i < 18; i++)
	{
	  fprintf(stderr,"%d\n",(int)pow((double)2,(double)i));
	}	
      return 1;
    }
  
  if (!(((fft_length | (fft_length - 1)) + 1) / 2 == fft_length))
    {
      fprintf(stderr,"fft_length should be 2 to the power of ... \nYou gave %d\n",fft_length);
      fprintf(stderr,"try one of the following values\n");
      for (i = 8; i < 18; i++)
	{
	  fprintf(stderr,"%d\n",(int)pow((double)2,(double)i));
	}	
      return 1; 
    }
  
  if(fft_data_length_in_fft>=fft_length || fft_data_length_in_fft<=10)
    {
      fprintf(stderr,"%s: fft_data_length_in_fft should be smaller or equal to %d and larger than 10\n",prog_name,fft_length);
      return 1;
    }
  
  if(power_window_size<10||power_window_size>fft_data_length_in_fft)
    {
      fprintf(stderr,"%s: power_window_size should be larger than 10 and smaller than fft_data_length_in_fft (%d) but was %d\n",prog_name,fft_data_length_in_fft,power_window_size);
      return 1;
    }
  
  if(sampling_rate<1000 || sampling_rate>50000)
    {
      fprintf(stderr,"%s: sampling_rate should be betwene 1000 and 50000 but is %d\n",prog_name,sampling_rate);
      return 1;

    }

  
  /***********************************************
      declare the program as a reat time program 
      the user will need to be root to do this
  *************************************************/
  if (with_o_opt==0) // don't do it if you work offline
    {
      struct sched_param param;
      param.sched_priority = MY_PRIORITY;
      if(sched_setscheduler(0, SCHED_FIFO, &param) == -1) 
	{
	  fprintf(stderr,"%s : sched_setscheduler failed\n",prog_name);
	  fprintf(stderr,"Do you have permission to run real-time applications? You might need to be root or use sudo\n");
	  return 1;
	}
      // Lock memory 
      if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) 
	{
	  fprintf(stderr,"%s: mlockall failed",prog_name);
	  return 1;
	}
      // Pre-fault our stack 
      unsigned char dummy[MAX_SAFE_STACK];
      memset(&dummy, 0, MAX_SAFE_STACK);
    }





  /*********************************************************
     code to initialize the AD card drivers and our interface
  **********************************************************/
  if (with_o_opt==0)
    { // only done when working online
      if (comedi_interface_init(&comedi_inter)==-1)
	{
	  fprintf(stderr,"%s could not initialize comedi_interface\n",prog_name);
	  return 1;
	}

      if(comedi_interface_set_sampling_rate(&comedi_inter,sampling_rate)==-1)
	{
	  fprintf(stderr,"%s could not set the sampling rate of comedi_interface\n",prog_name);
	  return 1;
	}
      // set the comedi_interface so it tries to get data every 2 ms
      comedi_interface_set_inter_acquisition_sleeping_time(&comedi_inter,COMEDI_INTERFACE_ACQUISITION_SLEEP_TIME_MS);
      // set the first comedi device so that is sample NUMBER_SAMPLED_CHANNEL_DEVICE_0 times channel BRAIN_CHANNEL_1
      // useful during stimulation based on brain activity
      // buffer on the card will fill up more quickly and we get access to it more frequently
      for(i=0;i<NUMBER_SAMPLED_CHANNEL_DEVICE_0;i++)
	{
	  channel_list[i]=BRAIN_CHANNEL_1;
	}
      comedi_interface_set_sampled_channels(&comedi_inter,0, NUMBER_SAMPLED_CHANNEL_DEVICE_0,channel_list);
      
      // make sure there is a valid analog output subdevice on the acquisition card
      if(comedi_inter.dev[device_index_for_stimulation].subdevice_analog_output==-1)
	{
	  fprintf(stderr,"%s: there is no valid analog output subdevice on device %d\n",prog_name,device_index_for_stimulation);
	  return 1;
	}
      // make sure we have at least 2 channels for analog output: intensity and pulse of laser
      if (comedi_inter.dev[device_index_for_stimulation].number_channels_analog_output<2)
	{
	  fprintf(stderr, "%s: subdevice_analog_output has less than 2 channels\n",prog_name);
	  return 1;
	}
      // check if laser intensity is outside of range for the analog output subdevice
      if (comedi_inter.dev[device_index_for_stimulation].voltage_max_output<laser_intensity_volt)
	{
	  fprintf(stderr, "%s: subdevice_analog_output max voltage is smaller than laser intensity\n",prog_name);
	  return 1;
	}
      // check if laser pulse is outside of range for the analog output subdevice
      if (comedi_inter.dev[device_index_for_stimulation].voltage_max_output<ttl_amplitude_volt)
	{
	  fprintf(stderr, "%s: subdevice_analog_output max voltage is smaller than ttl_amplitude_volt (%lf)\n",prog_name,ttl_amplitude_volt);
	  return 1;
	}
      if(ttl_amplitude_volt<0)
	{
	  fprintf(stderr, "%s: ttl_amplitude_volt should be at least 0 Volt but is %lf\n",prog_name,ttl_amplitude_volt);
	  return 1;
	}
      
      // get the value for the laser intensity and pulse
      comedi_baseline=comedi_from_phys(baseline_volt,
				       comedi_inter.dev[device_index_for_stimulation].range_output_array[comedi_inter.dev[device_index_for_stimulation].range_output],
				       comedi_inter.dev[device_index_for_stimulation].maxdata_output);
      comedi_intensity=comedi_from_phys(laser_intensity_volt,
					comedi_inter.dev[device_index_for_stimulation].range_output_array[comedi_inter.dev[device_index_for_stimulation].range_output],
					comedi_inter.dev[device_index_for_stimulation].maxdata_output);
      comedi_pulse=comedi_from_phys(ttl_amplitude_volt,
				    comedi_inter.dev[device_index_for_stimulation].range_output_array[comedi_inter.dev[device_index_for_stimulation].range_output],
				    comedi_inter.dev[device_index_for_stimulation].maxdata_output);
      
      // set the intensity channel
      comedi_data_write(comedi_inter.dev[device_index_for_stimulation].comedi_dev,
			comedi_inter.dev[device_index_for_stimulation].subdevice_analog_output, 
			CHANNEL_FOR_LASER_INTENSITY,
			comedi_inter.dev[device_index_for_stimulation].range_output,
			comedi_inter.dev[device_index_for_stimulation].aref,
			comedi_intensity);

      // set the pulse channel to baseline
      comedi_data_write(comedi_inter.dev[device_index_for_stimulation].comedi_dev,
			comedi_inter.dev[device_index_for_stimulation].subdevice_analog_output,
			CHANNEL_FOR_PULSE,
			comedi_inter.dev[device_index_for_stimulation].range_output,
			comedi_inter.dev[device_index_for_stimulation].aref,
			comedi_baseline);
#ifdef DEBUG_4HZ
      comedi_interface_print_info(&comedi_inter);
#endif

    }
  
  /*********************************************
     code to initialize the dat file if needed
  *********************************************/
  if(with_o_opt==1)
    {
      if((init_data_file_si(&data_file,dat_file_name,channels_in_dat_file))!=0)
	{
	  fprintf(stderr, "%s: problem in initialisation of dat file\n",prog_name);
	  return 1;
	}
    }
  



  
  /*************************************************
   section to do 4Hz stimulation based on power
  *************************************************/
  if(with_P_opt==1)
    {
      if(fftw_interface_4hz_init(&fftw_inter,
				 minimum_filter_freq,
				 maximum_filter_freq,
				 fft_length,
				 fft_data_length_in_fft,
				 power_window_size,
				 sampling_rate)==-1)
	{
	  fprintf(stderr,"%s could not initialize fftw_interface_4hz\n",prog_name);
	  return 1;
	}
#ifdef DEBUG_4HZ
      fftw_interface_4hz_print(&fftw_inter);
#endif
      
      if(with_o_opt==1)
	{ // if get data from dat file, allocate memory to store short integer from dat file
	  if((data_from_file=(short *) malloc(sizeof(short)*fftw_inter.real_data_to_fft_size))==NULL)
	    {
	      fprintf(stderr,"%s: problem allocating memory for data_from_file\n",prog_name);
	      return 1;
	    }
	}

      tk.duration_refractory_period=set_timespec_from_ms(refractory_ms);
      
      // start the acquisition thread, which will run in the background until comedi_inter.is_acquiring is set to 0
#ifdef DEBUG_4HZ
      fprintf(stderr,"starting acquisition\n");
#endif
      if(with_o_opt==0)
	if(comedi_interface_start_acquisition(&comedi_inter)==-1)
	  {
	    fprintf(stderr,"%s could not start comedi acquisition\n",prog_name);
	    return 1;
	  }
#ifdef DEBUG_4HZ
      // to check the intervals before getting new data.
      clock_gettime(CLOCK_REALTIME, &tk.time_previous_new_data);
      long int counter=0;
#endif
      
      // get time at beginning of trial
      clock_gettime(CLOCK_REALTIME, &tk.time_beginning_trial);
      clock_gettime(CLOCK_REALTIME, &tk.time_now);
      clock_gettime(CLOCK_REALTIME, &tk.time_last_stimulation);
      tk.elapsed_beginning_trial=diff(&tk.time_beginning_trial,&tk.time_now);
      
#ifdef DEBUG_4HZ
      fprintf(stderr,"start trial loop\n");
#endif


      
      
      /*************************************************************
       *************************************************************
       Main loop processing the data
       ************************************************************
       ************************************************************/
      
      while(tk.elapsed_beginning_trial.tv_sec < tk.trial_duration_sec) // loop until the trial is over
	{

#ifdef DEBUG_4HZ
	    fprintf(stderr,"time elapsed: %ld.%09ld sec\n",tk.elapsed_beginning_trial.tv_sec,tk.elapsed_beginning_trial.tv_nsec);
#endif
	  
 
	  if (with_o_opt==0) // get data from comedi acquisition device
	    {
	      /**************************************************
              get data from the comedi device
	      *************************************************/
	      // check if acquisition loop is still running, could have stop because of buffer overflow
	      if(comedi_inter.is_acquiring==0)
		{
		  fprintf(stderr,"comedi acquisition was stopped, theta stimulation not possible\n");
		  return 1;
		}

	      // if no new data is available or not enough data to do a fftw
	      while((comedi_inter.number_samples_read<=last_sample_no) || (comedi_inter.number_samples_read < fftw_inter.real_data_to_fft_size) )
		{
		  // sleep a bit and check for new data
		  nanosleep(&tk.duration_sleep_when_no_new_data,&tk.req);
		}
	      // copy the last data from the comedi_interface into the fftw_inter.signal_data
	      last_sample_no=comedi_interface_get_last_data_one_channel(&comedi_inter,BRAIN_CHANNEL_1,fftw_inter.real_data_to_fft_size,fftw_inter.signal_data,&tk.time_last_acquired_data);
	      if(last_sample_no==-1)
		{
		  fprintf(stderr,"error returned by comedi_interface_get_data_one_channe()\n");
		  return 1;
		}
#ifdef DEBUG_4HZ
	      clock_gettime(CLOCK_REALTIME, &tk.time_current_new_data);
	      tk.duration_previous_current_new_data=diff(&tk.time_previous_new_data,&tk.time_current_new_data);
	      //  fprintf(stderr,"%ld until sample %ld, last_sample_no: %ld with interval %lf(us)\n",counter++,comedi_inter.number_samples_read, last_sample_no, tk.duration_previous_current_new_data.tv_nsec/1000.0);
	      tk.time_previous_new_data=tk.time_current_new_data;
#endif
	    }
	  else
	    {
	      /**************************************************
              get data from a .dat file
	      *************************************************/
	      clock_gettime(CLOCK_REALTIME,&tk.time_last_acquired_data);
	      if(last_sample_no==0)
		{
		  // fill the buffer with the beginning of the file
		  if((data_file_si_get_data_one_channel(&data_file, offline_channel, data_from_file,0,fftw_inter.real_data_to_fft_size))!=0)
		    {
		      fprintf(stderr,"%s, problem with data_file_si_get_data_one_channel, first index: %d, last index: %d\n",prog_name,0,fftw_inter.real_data_to_fft_size);
		      return 1;
		    }
		  last_sample_no=fftw_inter.real_data_to_fft_size;
		}
	      else
		{
		  // get data further on in the dat file
		  if((data_file_si_get_data_one_channel(&data_file, offline_channel, data_from_file,last_sample_no+new_samples_per_read_operation-fftw_inter.real_data_to_fft_size,last_sample_no+new_samples_per_read_operation))!=0)
		    {
		      fprintf(stderr,"%s, problem with data_file_si_get_data_one_channel, first index: %ld, last index: %ld\n",prog_name,last_sample_no+new_samples_per_read_operation-fftw_inter.real_data_to_fft_size,last_sample_no+new_samples_per_read_operation);
		      return 1;
		    }
		  last_sample_no= last_sample_no+new_samples_per_read_operation;
		}

	      // copy the short int array to double array
	      for (i=0; i < fftw_inter.real_data_to_fft_size;i++)
		{
		  fftw_inter.signal_data[i]=data_from_file[i];
		} 
	    }

	  /**************************************************
            data are in memory, time to process the signal
	   *************************************************/
	  // 	  for (i=0; i < fftw_inter.real_data_to_fft_size;i++)
	  if(last_sample_no>=fftw_inter.real_data_to_fft_size)
	    {
	      
	      // filter the raw signal
	      fftw_interface_4hz_apply_filter(&fftw_inter);

	      
             // get the power of signal
	      power_4hz=fftw_interface_4hz_power(&fftw_inter);


             // get the phase of the signal
	      clock_gettime(CLOCK_REALTIME, &tk.time_now);
	      tk.elapsed_last_acquired_data=diff(&tk.time_last_acquired_data,&tk.time_now);
	      // get phase of filtered signal
	      current_phase=fftw_interface_4hz_get_phase(&fftw_inter, &tk.elapsed_last_acquired_data,frequency_for_phase); 
	      
	      // phase difference between wanted and what it is now, from -180 to 180
	      phase_diff=phase_difference(current_phase,stimulation_phase);
	      

	      // to check input data, filtered signal, power and phase with R script
	      //	      fprintf(stderr,"iter: %ld, phase: %lf \n",trial_iteration,current_phase);	      
	      //for (i=0; i < fftw_inter.real_data_to_fft_size;i++)
	      //	  printf("%ld %ld %lf %lf %lf %lf\n",
	      //	 trial_iteration,last_sample_no-fftw_inter.real_data_to_fft_size+i,fftw_inter.signal_data[i],fftw_inter.filtered_signal[i],current_phase,power_4hz);
	      
 
#ifdef DEBUG_4HZ
	      printf("Index %ld, power: %lf, phase: %f, phase_difference: %f, power_threshold: %f, refractory: %ld.%09ld\n",last_sample_no,power_4hz,current_phase,phase_diff,power_threshold,tk.duration_refractory_period.tv_sec ,tk.duration_refractory_period.tv_nsec );
#endif
	      if(with_H_opt==0) // we care only about power
		{
		  if(power_4hz>power_threshold)
		    {
		      clock_gettime(CLOCK_REALTIME, &tk.time_now);
		      tk.elapsed_last_acquired_data=diff(&tk.time_last_acquired_data,&tk.time_now);

		      // check if refractory period is over
		      clock_gettime(CLOCK_REALTIME,&tk.time_now);
		      tk.elapsed_last_stimulation=diff(&tk.time_last_stimulation,&tk.time_now);
		      
		      // if the laser refractory period is over
		      if((tk.elapsed_last_stimulation.tv_sec>tk.duration_refractory_period.tv_sec) ||
			(tk.elapsed_last_stimulation.tv_sec==tk.duration_refractory_period.tv_sec &&
			 tk.elapsed_last_stimulation.tv_nsec>tk.duration_refractory_period.tv_nsec))

			{
			  if(with_m_opt)
			    { // how long is the laser pulse
			      int laser_on_ms =  (rand() %  (maximum_pulse_length_ms - minimum_pulse_length_ms + 1)) + minimum_pulse_length_ms;
			      tk.pulse_duration_ms=(double)laser_on_ms;
			      tk.duration_pulse=set_timespec_from_ms(tk.pulse_duration_ms);
			    }

			  // stimulation time!!
			  clock_gettime(CLOCK_REALTIME,&tk.time_last_stimulation); 
			  
			  // start the pulse
			  comedi_data_write(comedi_inter.dev[device_index_for_stimulation].comedi_dev,
					    comedi_inter.dev[device_index_for_stimulation].subdevice_analog_output,
					    CHANNEL_FOR_PULSE,
					    0,
					    comedi_inter.dev[device_index_for_stimulation].aref,
					    comedi_pulse);
			  // wait
			  nanosleep(&tk.duration_pulse,&tk.req);
			  
			  // end of the pulse
			  comedi_data_write(comedi_inter.dev[device_index_for_stimulation].comedi_dev,
					    comedi_inter.dev[device_index_for_stimulation].subdevice_analog_output,
					    CHANNEL_FOR_PULSE,
					    0,
					    comedi_inter.dev[device_index_for_stimulation].aref,
					    comedi_baseline);
#ifdef DEBUG_4HZ
			  fprintf(stderr,"******Stimulation******* duration: %ld.%09ld sec ******\n",tk.duration_pulse.tv_sec,tk.duration_pulse.tv_nsec);
#endif
			  // get the time of last stimulation
			  clock_gettime(CLOCK_REALTIME, &tk.time_last_stimulation);

			}
		      else{
#ifdef DEBUG_4HZ
			fprintf(stderr,"time within refractory period: %ld.%09ld sec\n",tk.elapsed_last_stimulation.tv_sec,tk.elapsed_last_stimulation.tv_nsec);
#endif			   

		      }
		    }
		}
	      else //(with_H_opt==1)
		{ // we care about power and phase
		  if(power_4hz>power_threshold && sqrt(phase_diff*phase_diff) < max_phase_diff)
		    {
		      clock_gettime(CLOCK_REALTIME, &tk.time_now);
		      tk.elapsed_last_acquired_data=diff(&tk.time_last_acquired_data,&tk.time_now);
		      
		      // if we are just before the stimulation phase, we nanosleep to be bang on the correct phase
		      if(phase_diff<0)
			{
			  tk.duration_sleep_to_right_phase=set_timespec_from_ms((0-phase_diff)*degree_duration_ms);
			  nanosleep(&tk.duration_sleep_to_right_phase,&tk.req);
			}
		      clock_gettime(CLOCK_REALTIME,&tk.time_now);
		      tk.elapsed_last_stimulation=diff(&tk.time_last_stimulation,&tk.time_now);
			  
		      // if the laser refractory period is over
		      if((tk.elapsed_last_stimulation.tv_sec>tk.duration_refractory_period.tv_sec) ||
			(tk.elapsed_last_stimulation.tv_sec==tk.duration_refractory_period.tv_sec &&
			 tk.elapsed_last_stimulation.tv_nsec>tk.duration_refractory_period.tv_nsec))
			{

			  if(with_m_opt)
			    { // how long is the laser pulse
			      int laser_on_ms =  (rand() %  (maximum_pulse_length_ms - minimum_pulse_length_ms + 1)) + minimum_pulse_length_ms;
			      tk.pulse_duration_ms=(double)laser_on_ms;
			      tk.duration_pulse=set_timespec_from_ms(tk.pulse_duration_ms);
			    }
			  
			  // stimulation time!!
			  clock_gettime(CLOCK_REALTIME,&tk.time_last_stimulation); 
			  
			  // start the pulse
			  comedi_data_write(comedi_inter.dev[device_index_for_stimulation].comedi_dev,
					    comedi_inter.dev[device_index_for_stimulation].subdevice_analog_output,
					    CHANNEL_FOR_PULSE,
					    0,
					    comedi_inter.dev[device_index_for_stimulation].aref,
					    comedi_pulse);
			  // wait
			 
			  nanosleep(&tk.duration_pulse,&tk.req);
			  
			  // end of the pulse
			  comedi_data_write(comedi_inter.dev[device_index_for_stimulation].comedi_dev,
					    comedi_inter.dev[device_index_for_stimulation].subdevice_analog_output,
					    CHANNEL_FOR_PULSE,
					    0,
					    comedi_inter.dev[device_index_for_stimulation].aref,
					    comedi_baseline);
#ifdef DEBUG_4HZ
			  fprintf(stderr,"******Stimulation, phase: %f, duration: %ld.%09ld sec ******\n",stimulation_phase,tk.duration_pulse.tv_sec,tk.duration_pulse.tv_nsec);
#endif
			  // get the time of last stimulation
			  clock_gettime(CLOCK_REALTIME, &tk.time_last_stimulation);

			}
		      else
			{
#ifdef DEBUG_4HZ
			  fprintf(stderr,"time within refractory period: %ld.%09ld sec\n",tk.elapsed_last_stimulation.tv_sec,tk.elapsed_last_stimulation.tv_nsec);
#endif			   

			}
		    }
		}
	      
	      
	      clock_gettime(CLOCK_REALTIME, &tk.time_now);
	      tk.elapsed_last_acquired_data=diff(&tk.time_last_acquired_data,&tk.time_now);
	      
	    }
	  
	  clock_gettime(CLOCK_REALTIME, &tk.time_now);
	  tk.elapsed_beginning_trial=diff(&tk.time_beginning_trial,&tk.time_now);
	  trial_iteration+=1;
	}
      
      // this will stop the acquisition thread
      if(with_o_opt==0)
	{
#ifdef DEBUG_4HZ
	  fprintf(stderr,"stop acquisition thread\n");
#endif
	  if(comedi_interface_stop_acquisition(&comedi_inter)==-1)
	    {
           fprintf(stderr,"%s could not stop comedi acquisition\n",prog_name);
	   return 1;
	    }
	}
       // free the memory used by fftw_inter    
      fftw_interface_4hz_free(&fftw_inter);
    }
     






  




  /******************************
       code to free resources 
  ******************************/
  if (with_o_opt==0)
    {
      // free the memory used by comedi_inter
      comedi_interface_free(&comedi_inter);
    }
  // free the memory for dat file data, if running with offline data
  if(with_o_opt==1)
    {
      if((clean_data_file_si(&data_file))!=0)
	{
	  fprintf(stderr, "%s: problem with clean_data_file_si\n",prog_name);
	  return 1;
	}
      free(data_from_file);
    }
  return 0;
} 

void print_options()
{
  printf("possible options:\n");
  
  printf("--help or -h\t\t\t\t\t: will print this text\n");
  printf("--version or -v\t\t\t\t\t: print the program version\n");

  printf("--power or -P\t\t\t\t\t: stimulate when a power threshold is reached\n");
  printf("--phase <phase_in_deg> or -H\t\t\t: stimulation at a given phase when a power threshold is reached\n");

  printf("--pulse_length <length_ms> or -l\t\t: length of a stimulation in ms, if you want a fixed stimulation length\n");
  printf("--minimum_pulse_length_ms <length_ms> or -m\t: minimum length of a stimulation in ms, will be chosen randomly between minimum and maximum length\n");
  printf("--maximum_pulse_length_ms <length_ms> or -M\t: maximum length of a stimulation in ms, will be chosen randomly between minimum and maximum length\n");
  printf("--refractory <refractory_ms> or -r\t\t: refractory periods between the two stimulations\n");
  printf("--minimum_filter_freq <freq_hz> or -f\t\t: lowest frequency of the band pass filter\n");
  printf("--maximum_filter_freq <freq_hz> or -F\t\t: highest frequency of the band pass filter\n");
  printf("--stimulation_phase_deviation <phase_deg> or -q\t: largest difference between stimulation phase and current phase that will trigger a stimulation\n");


  printf("--window_fft_total_length <samples> or -w\t: length of segment to fft (integer), faster with pow(2,x)\n");
  printf("--window_fft_data_length <samples> or -X\t: number of data samples in fft (integer), the rest is padded with 0 for better phase cal\n");
  printf("--window_fft_power_ms <ms> or -y\t\t: lenght of segment in ms to calculate the power\n");
  printf("--sampling_rate <samples per seconds> or -z\t: sampling rate of the data\n");


  
  printf("--offline <dat_file_name> or -o\t\t\t: use a .dat file as input data. You also need to set option -c.\n");
  printf("--channels_in_dat_file <number> or -c\t\t: give the number of channels in the dat file. Use only when working offline from a dat file (-o or --offline)\n");
  printf("--offline_channel <number> or -x\t\t: give the channel on which detection is done when working offline from a dat file (-o)\n");

  printf("--output_device_for_stimulation <number> or -d\t: give the comedi device to use\n");
  printf("--ttl_amplitude_volt <V> or -a\t\t\t: give the amplitude of the ttl pulse in volt\n");
  return;
}
