#include "sane/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/fcntl.h>

#include "sane/sane.h"
#include "sane/sanei.h"
#include "sane/sanei_wire.h"
#include "sane/sanei_codec_ascii.h"
#include "sane/sanei_codec_bin.h"

static Wire w;

static SANE_Word dpi_word_list[] =
  {
    4,				/* # of elements */
    3, 30, 300, -600
  };

static SANE_String_Const mode_list[] =
  {
    "Lineart", "Grayscale", "Color", 0
  };

static char *program_name;
static char *default_codec = "bin";
static char *default_outfile = "test_wire.out";

static int
usage (int code)
{
  if (code == 0)
    {
      printf ("Usage: %s [OPTION]...\n\
\n\
Test the SANE wire manipulation library.\n\
\n\
    --codec=CODEC        set the codec [default=%s]\n\
    --help               display this message and exit\n\
-o, --output=FILE        set the output file [default=%s]\n\
    --readonly           do not create FILE, just read it\n\
    --version            print version information\n\
\n\
Valid CODECs are: `ascii' `bin'\n",
	      program_name, default_codec, default_outfile);
    }
  else
    {
      fprintf (stderr, "Type ``%s --help'' for more information.\n",
	       program_name);
    }
  exit (code);
}


int
main (int argc, char ** argv)
{
  SANE_Option_Descriptor desc[2], *desc_ptr;
  SANE_Word len;
  char *codec = default_codec;
  char *outfile = default_outfile;
  int readonly = 0;

  program_name = argv[0];
  argv ++;
  while (*argv != 0)
    {
      if (!strcmp (*argv, "--codec"))
	{
	  if (argv[1] == 0)
	    {
	      fprintf (stderr, "%s: option `%s' requires an argument\n",
		       program_name, *argv);
	      usage (1);
	    }

	  argv ++;
	  codec = *argv;
	}
      else if (!strncmp (*argv, "--codec=", 8))
	{
	  codec = *argv + 8;
	}
      else if (!strcmp (*argv, "--help"))
	{
	  usage (0);
	}
      else if (!strcmp (*argv, "-o") || !strcmp (*argv, "--output"))
	{
	  if (argv[1] == 0)
	    {
	      fprintf (stderr, "%s: option `%s' requires an argument\n",
		       program_name, *argv);
	      usage (1);
	    }

	  argv ++;
	  outfile = *argv;
	}
      else if (!strncmp (*argv, "--output=", 9))
	{
	  outfile = *argv + 9;
	}
      else if (!strcmp (*argv, "--readonly"))
	{
	  readonly = 1;
	}
      else if (!strcmp (*argv, "--version"))
	{
	  printf ("test_wire (%s) %s\n", PACKAGE, VERSION);
	  exit (0);
	}
      else if (**argv == '-')
	{
	  fprintf (stderr, "%s: unrecognized option `%s'\n",
		   program_name, *argv);
	  usage (1);
	}
      else
	{
	  fprintf (stderr, "%s: too many arguments\n", program_name);
	}

      argv ++;
    }


  if (!strcmp (codec, "bin"))
    sanei_w_init (&w, sanei_codec_bin_init);
  else if (!strcmp (codec, "ascii"))
    sanei_w_init (&w, sanei_codec_ascii_init);
  else
    {
      fprintf (stderr, "%s: unknown codec type `%s'\n", program_name, codec);
      usage (1);
    }

  desc[0].name = "resolution";
  desc[0].title = 0;
  desc[0].desc = "Determines scan resolution in dots/inch (\"DPI\").";
  desc[0].type = SANE_TYPE_FIXED;
  desc[0].unit = SANE_UNIT_DPI;
  desc[0].size = sizeof (SANE_Word);
  desc[0].cap = SANE_CAP_SOFT_SELECT | SANE_CAP_SOFT_DETECT;
  desc[0].constraint_type = SANE_CONSTRAINT_WORD_LIST;
  desc[0].constraint.word_list = dpi_word_list;

  desc[1].name = "mode";
  desc[1].title ="Scan Mode";
  desc[1].desc = "Determines scan mode.";
  desc[1].type = SANE_TYPE_STRING;
  desc[1].unit = SANE_UNIT_NONE;
  desc[1].size = 10;
  desc[1].cap = (SANE_CAP_SOFT_SELECT | SANE_CAP_SOFT_DETECT
		 | SANE_CAP_AUTOMATIC);
  desc[1].constraint_type = SANE_CONSTRAINT_STRING_LIST;
  desc[1].constraint.string_list = mode_list;

  {
    int flags;
    if (readonly)
      {
	printf ("reading %s output from %s\n", codec, outfile);
	flags = O_RDONLY;
      }
    else
      {
	printf ("creating %s\n", outfile);
	flags = O_RDWR | O_CREAT | O_TRUNC;
      }

    w.io.fd = open (outfile, flags, 0666);

    if (w.io.fd < 0)
      {
	perror (outfile);
	return -1;
      }
  }
  w.io.read  = read;
  w.io.write = readonly ? 0 : write;

  if (!readonly)
    {
      sanei_w_set_dir (&w, WIRE_ENCODE);
      w.status = 0;

      len = NELEMS(desc);
      desc_ptr = desc;
      sanei_w_array (&w, &len, (void **) &desc_ptr,
		     (WireCodecFunc) sanei_w_option_descriptor,
		     sizeof (desc[0]));

      if (w.status == 0)
	printf ("%s encode successful\n", codec);
      else
	fprintf (stderr, "%s: %s encode error %d: %s\n",
		 codec, program_name, w.status, strerror (w.status));

      printf ("%s output written to %s\n", codec, outfile);
    }

  sanei_w_set_dir (&w, WIRE_DECODE);
  w.status = 0;

  if (!readonly)
    {
      printf ("reading %s output from %s\n", codec, outfile);
      lseek (w.io.fd, 0, SEEK_SET);
    }

  sanei_w_array (&w, &len, (void **) &desc_ptr,
		 (WireCodecFunc) sanei_w_option_descriptor, sizeof (desc[0]));

  if (w.status == 0)
    printf ("%s decode successful\n", codec);
  else
    fprintf (stderr, "%s: %s decode error %d: %s\n",
	     program_name, codec, w.status, strerror (w.status));

  sanei_w_set_dir (&w, WIRE_FREE);
  w.status = 0;
  sanei_w_array (&w, &len, (void **) &desc_ptr,
		 (WireCodecFunc) sanei_w_option_descriptor, sizeof (desc[0]));

  if (w.status == 0)
    printf ("free successful\n");
  else
    fprintf (stderr, "%s: free error %d: %s\n",
	     program_name, w.status, strerror (w.status));

  close (w.io.fd);

  return 0;
}
