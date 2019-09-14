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
  int with_o_opt=0;
  int with_c_opt=0;
  int with_x_opt=0;
  int with_d_opt=0;
  int with_a_opt=0;

  // variables to store values passed with options
  double stimulation_phase=90;
  double pulse_length_ms=10;
  double minimum_pulse_length_ms=10;
  double maximum_pulse_length_ms=20;
  double refractory_ms=5;
  double minimum_filter_freq=2;
  double maximum_filter_freq=6;
  double stimulation_phase_deviation=30;
  
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

	  {"power",no_argument,0,'P'},
	  {"phase",required_argument,0,'H'},
	  
	  {"pulse_length",required_argument,0,'l'},
	  {"minimum_pulse_length_ms", required_argument,0,'m'},
	  {"maximum_pulse_length_ms", required_argument,0,'M'},
	  {"refractory", required_argument,0,'r'},
	  {"minimum_filter_freq", required_argument,0,'f'},
	  {"maximum_filter_freq", required_argument,0,'F'},
	  {"stimulation_phase_deviation", required_argument,0,'q'},
	  	  
	  {"offline", required_argument,0,'o'},
	  {"channels_in_dat_file", required_argument,0,'c'},
	  {"offline_channel", required_argument,0,'x'},

	  {"output_device_for_stimulation", required_argument,0,'d'},
	  {"ttl_amplitude_volt", required_argument,0,'a'},
	  {0, 0, 0, 0}
	};
      int option_index = 0;
      opt = getopt_long (argc, argv, "hvPH:l:m:M:r:f:F:q:o:c:x:d:a:",
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
	    minimum_pulse_length_ms=atof(optarg);
	    break;
	  }	  
	case  'M':
	  {
	    with_M_opt=1;
    	    maximum_pulse_length_ms=atof(optarg);
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
  double laser_intensity_volt; // for laser power
  double baseline_volt=0; // for ttl pulse


  // same as above variables, but in comedi value instead of volts
  lsampl_t comedi_intensity =0;
  lsampl_t comedi_baseline =0;
  lsampl_t comedi_pulse=0;

  //  double theta_frequency=(MIN_FREQUENCY_THETA+MAX_FREQUENCY_THETA)/2;
  //double theta_degree_duration_ms=(1000/theta_frequency)/360;
  //double theta_delta_ratio;


  // structure with variables about the comedi device, see main.h for details
  struct comedi_interface comedi_inter;

  // structure with variables to do the filtering of signal
  struct fftw_interface_theta fftw_inter;

  // structure with variables to do time keeping
  struct time_keeper tk;



  struct data_file_si data_file; // data_file_short_integer

  // list of channels to scan, bluff, we will sample the same channel many times
  int channel_list[NUMBER_SAMPLED_CHANNEL_DEVICE_0];
  long int last_sample_no=0;
  double current_phase=0;
  double phase_diff;

  
  // parce the arguments from the command line
  tk.trial_duration_sec=atof(argv[optind]);
  laser_intensity_volt=atof(argv[optind+1]);


  tk.duration_sleep_when_no_new_data=set_timespec_from_ms(SLEEP_WHEN_NO_NEW_DATA_MS);
  //  tk.duration_pulse=set_timespec_from_ms(tk.pulse_duration_ms);
  
  // variables to work offline from a dat file
  int new_samples_per_read_operation=60; //  3 ms of data
  short int* data_from_file;
  short int* ref_from_file;


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

  if (tk.trial_duration_sec<=0 || tk.trial_duration_sec > 10000)
    {
      fprintf(stderr,"%s: trial duration should be between 0 and 10000 sec\nYou gave %lf\n",prog_name,tk.trial_duration_sec);
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

  printf("--offline <dat_file_name> or -o\t\t\t: use a .dat file as input data. You also need option -c, together with either -s or -t, to work with -o\n");
  printf("--channels_in_dat_file <number> or -c\t\t: give the number of channels in the dat file. Use only when working offline from a dat file (-o or --offline)\n");
  printf("--offline_channel <number> or -x\t\t: give the channel on which detection is done when working offline from a dat file (-o)\n");

  printf("--output_device_for_stimulation <number> or -d\t: give the comedi device to use\n");
  printf("--ttl_amplitude_volt <V> or -a\t\t\t: give the amplitude of the ttl pulse in volt\n");
  return;
}


