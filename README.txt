Authors: Florence Townend and Jack James
Date Created: 21/02/2018 to 22/03/2018

Description of Code:

This code performs a series of functions on an INFILE provided by the user. These include hexdump (can be used on any file type), P2toP5 (can be used on P2 files), P5toP2 (can be used on P5 files), rotate (maximum image size 512x512), rotate90 and negative - which can all be used with P5 or P2 files.
Corrupted files are detected by this code in the P2toP5 and the P5toP2 functionalities. 
Incorrect usage is also detected - this includes incorrect number of arguments, incorrect spelling of arguments, unopenable infiles and incorrect infile types (for P2toP5 and P5toP2).
These incorrect usages and corrupted files return error messages indicating the users' errors.


How to compile the code:

	Type "make" into the terminal.
	Type "./pnmdump.exe --usage" into the terminal and note the message that is outputted showing how to use the code. 

	Usage:
	  	./pnmdump.exe --version
  		./pnmdump.exe --usage
  		./pnmdump.exe --hexdump [FILE]

	Other Usages not included in the message:
		./pnmdump.exe  --P2toP5   INFILE  OUTFILE
	    ./pnmdump.exe  --P5toP2   INFILE  OUTFILE
	    ./pnmdump.exe  --rotate   INFILE  OUTFILE
	    ./pnmdump.exe  --rotate90   INFILE  OUTFILE
	    ./pnmdump.exe  --negative  INFILE OUTFILE

	 --version will print the version of the code.
	 --usage will print instructions for use as before.
	 --hexdump will print the characters in a file to hexidecimal and ascii. The file can be from stdin or linked in the terminal in the   [FILE] placeholder.
	 --P2toP5 takes a P2 INFILE and converts it into a P5 file, writing to the OUTFILE provided.
	 --P5toP2 takes a P5 INFILE and converts it into a P2 file, writing to the OUTFILE provided.
	 --rotate reflects/rotates the INFILE (P2 or P5) and writes it to the OUTFILE.
	 --rotate90 rotates the INFILE (P2 or P5) 90 degrees clockwise and writes it to the OUTFILE.
	 --negative takes an INFILE (P2 or P5) and applies a negative effect (darkest pixels --> lightest pixels and vice versa).


Changelog:

20/03/2018: Improving look of code by adding comments and spaces when required.
18/03/2018: code can now detect corrupted p5 files by checking if array isn't fully filled or if there is more to be read
18/03/2018: reversed middle arguments of fread function so it returns the correct value.
16/03/2018: negative function work with P5 files
16/03/2018: Corrupted files P2 1,2,3,4 now give error codes
10/03/2018: Created an additional function "--negative" which takes P2 pgm files and makes them have the 'negative' effect
10/03/2018: Created functions for writing the Image Properties, Rotating arrays, Rotating arrays by 90 degrees, P2 to P5 and P5 to P2.
09/03/2018: added if statements to --rotate and --rotate90 to change reading/writing method according to file type (P2 or P5)
09/03/2018: used fread and fwrite to read/write binary files for P2toP5 and rotate/rotate90 
06/03/2018: Rotate/Reflecting array
01/03/2018: Reading rest of file into a 2D array
25/02/2018: added if loop to check if --P2toP5 receives P2 and vice versa
24/02/2018: Changed ReadFourLines from a void to a ImageProps to return a struct.
24/02/2018: ReadFourLines function made. Using sscanf, reading first four lines and assigning respective variables to struct instances.
21/02/2018: improved beginning if else statements to disallow loopholes when entering arguments and not receiving bad arguments messages when they should
21/02/2018: made function called Iteration within for(;;) loop to condense code
20/02/2018: read from stdin using fread(array, 1, 8, stdin) when detecting no file input (no argv[2])
17/02/2018: adding a line break after printing of hexdump no matter how many leftover bytes there are (e.g. 0 bytes left still prints \n)
16/02/2018: check that ascii version is between 32 and 126, print "." otherwise
16/02/2018: changing error messages to match the testfile
16/02/2018: using strcmp with argv[] to detect when argument asks for version number
16/02/2018: correct number of spaces between characters put in
16/02/2018: for loop for iteration of reading through infile for hexdump functionality

