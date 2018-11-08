/* pnmdumpmain.c */
#include <stdio.h>
#include <string.h>

// defining a struct for the properties for the .pgm file
struct ImageProperties
{
	char ImageType[2];  //  p2 or p5
	int Height;
	int Width;
	int MaxValue;
} typedef ImageProps;

int BadArguments();
ImageProps ReadFourLines(FILE* infile);
int Iteration(int NumBytes, char array[], int n);
int WriteFourLines(FILE* outfile, ImageProps file);
int WriteFourLinesRotate(FILE* outfile, ImageProps file);
int WriteRotateArray(FILE* infile, FILE* outfile, ImageProps file);
int WriteRotate90Array(FILE* infile, FILE* outfile, ImageProps file);
int P2toP5Func(FILE* infile, FILE* outfile, ImageProps file);
int P5toP2Func(FILE* infile, FILE* outfile, ImageProps file);
int negativeFunc(FILE* infile, FILE* outfile, ImageProps file);
int main(int argc, char *argv[])
{
	ImageProps file; // defining instance of struct
	FILE* infile = NULL;
	FILE* outfile = NULL;
	char ver[] = "1.0";

 	if(argc < 2) // error when having no arguments
 	{
 		BadArguments();
 		return 1;
 	}
 	
  	if(argc == 2 && !strcmp(argv[1], "--version")) // printing version
  	{
  		fprintf(stdout, "%s\n", ver);
  		return 0;
  	}

	else if(argc == 2 && !strcmp(argv[1], "--usage")) // printing usage
  	{
  		fprintf(stdout, "Usage:\n");
  		fprintf(stdout, "./pnmdump.exe --version\n");
  		fprintf(stdout, "./pnmdump.exe --usage\n");
  		fprintf(stdout, "./pnmdump.exe --hexdump [FILE]\n");
  		return 0;
  	}

  	if(!strcmp(argv[1], "--hexdump")) // for hexdump argument
  	{
  		char BytesArray[8];
		int nBytes;	
		int x = 0;
  		if(argc == 3) // checking that there are 3 arguments (./pnmdump.exe --hexdump [FILE])
  		{
  			char* infilename = argv[2];
  			infile = fopen(infilename, "rb"); // read
  			if(infile == NULL)  // if unable to open the file
  			{
    			fprintf(stderr, "No such file: \"%s\"\n", infilename);
    			return 1;
  			}
  		}
  		if(argc > 3 || argc < 2) // without the correct number of arguments
  		{
  			BadArguments();
  			return 1;
  		}
		for(;;)
		{
			int n = 0; // indicates position in array
			if(argc == 3)
			{
				nBytes = fread(BytesArray, 1, 8, infile); // returns number of bytes read from infile
			}
			else if(argc == 2)
			{
				nBytes = fread(BytesArray, 1, 8, stdin); // returns number of bytes read from stdin
			} 
			printf("%07x", x);
			Iteration(nBytes, BytesArray, n);
			if(nBytes == 0) // end of file
			{
				printf("\n");
				break;
			}
			if(nBytes < 8) // if bytes_read<8, the end of file has been reached
			{
				printf("%07x\n", x + nBytes);  // outputting the final number of bytes read
				break;
			}
			x = x + nBytes;
		}
		if(argc == 3)  // closing file
		{
			fclose(infile);
		}	
  		return 0;
  	}

  	if((!strcmp(argv[1], "--P2toP5") && argc == 4)) // checking correct number of arguments with P2toP5
	{	
		char* infilename = argv[2]; // opening input file
  		infile = fopen(infilename, "rb");
  		char* outfilename = argv[3]; // opening output file
  		outfile = fopen(outfilename, "wb");
  		if(infile == NULL)
  		{
    		fprintf(stderr, "No such file: \"%s\"\n", infilename);
    		return 1;
  		}
		file = ReadFourLines(infile); // reading the first four lines, returns a struct
		if(file.Width == 0 || file.Height == 0) // input file not in correct format
		{
			fprintf(stderr, "Corrupted input file\n");
			return 1;
		}
  		if(strcmp(file.ImageType, "P2")) // checking infile is the correct type (P2)
  		{
  			fprintf(stderr, "Input is not in P2 format\n");
  			return 1;
  		}
  		WriteFourLines(outfile, file); // outputting the first four lines
  		int check = P2toP5Func(infile, outfile, file);
  		if(check == 1)  // function returned 1
		{
			fprintf(stderr, "Corrupted input file\n");
			return 1;
		}
		fclose(infile);
		fclose(outfile);
		return 0;
	}
	
	if(!strcmp(argv[1], "--P5toP2") && argc == 4) // correcting number of arguments with P5toP2
	{	
		char* infilename = argv[2];
  		infile = fopen(infilename, "rb"); // opening infile
  		char* outfilename = argv[3];
  		outfile = fopen(outfilename, "wb"); // opening outfile
  		if(infile == NULL)
		{
			fprintf(stderr, "No such file: \"%s\"\n", infilename);
			return 1;
		}
		file = ReadFourLines(infile); // reading first four lines, returning struct
		if(strcmp(file.ImageType, "P5")) // checking file type is P5
  		{
  			fprintf(stderr, "Input is not in P5 format\n");
  			return 1;
  		}
  		WriteFourLines(outfile, file); // outputting first four lines
  		int check = P5toP2Func(infile, outfile, file);
  		if(check == 1)
  		{
  			fprintf(stderr, "Corrupted input file\n");
			return 1;
  		}
  		fclose(infile);
		fclose(outfile);
		return 0;
	}

	if(!strcmp(argv[1], "--rotate"))
	{
		char* infilename = argv[2];
  		infile = fopen(infilename, "rb"); // opening infile
  		char* outfilename = argv[3];
  		outfile = fopen(outfilename, "wb"); // opening outfile
		file = ReadFourLines(infile); // reading first four lines
		WriteFourLinesRotate(outfile, file);
		int check = WriteRotateArray(infile, outfile, file);
		if(check == 1)
		{
			fprintf(stderr, "Corrupted input file\n");
			return 1;
		}
		fclose(infile);
		fclose(outfile);
		return 0;
	}

	if(!strcmp(argv[1], "--rotate90"))
	{
		char* infilename = argv[2];
  		infile = fopen(infilename, "rb"); // opening infile
  		char* outfilename = argv[3];
  		outfile = fopen(outfilename, "wb"); // opening outfile
		file = ReadFourLines(infile);// reading first four lines
		WriteFourLinesRotate(outfile, file);
		int check = WriteRotate90Array(infile, outfile, file);
		if(check == 1)
		{
			fprintf(stderr, "Corrupted input file\n");
			return 1;
		} 
		fclose(infile);
		fclose(outfile);
		return 0;
	}

	if(!strcmp(argv[1], "--negative"))
	{
		char* infilename = argv[2];
  		infile = fopen(infilename, "rb"); // opening infile
  		char* outfilename = argv[3];
  		outfile = fopen(outfilename, "wb");
		file = ReadFourLines(infile);
		negativeFunc(infile, outfile, file);
		fclose(infile);
		fclose(outfile);
		return 0;
	}

	if(strcmp(argv[1], "--P2toP5") || strcmp(argv[1], "--P5toP2") || strcmp(argv[1], "--rotate") 
		|| strcmp(argv[1], "--rotate90") || strcmp(argv[1], "--negative"))
	{
		if(argc != 4)
		{
			BadArguments();
			return 1;
		}
	}

	if(strcmp(argv[1], "--P2toP5") || strcmp(argv[1], "--P5toP2") || strcmp(argv[1], "--rotate") 
		|| strcmp(argv[1], "--rotate90") || strcmp(argv[1], "--hexdump") || strcmp(argv[1], "--usage")
		 || strcmp(argv[1], "--version") || strcmp(argv[1], "--negative"))
	{
		BadArguments();
		return 1;
	}
}

// function definitions
int BadArguments() // outputting bad arguments
{
	fprintf(stderr, "pnmdump: bad arguments\n");
	fprintf(stderr, "Usage:\n");
    fprintf(stderr, "./pnmdump.exe --version\n");
    fprintf(stderr, "./pnmdump.exe --usage\n");
    fprintf(stderr, "./pnmdump.exe --hexdump [FILE]\n");
    return 0;
}
// iteration for hexdump functionality
int Iteration(int NumBytes, char array[], int n)
{
	for(n = 0; n < NumBytes; n++)
	{
		if(n == NumBytes - 1)
		{	
			printf("  %02X ", (unsigned char)array[n]);			
			if(array[n] >= 32 && array[n] <= 126)
			{	
				printf("%c\n", array[n]);
			}
			else
			{
				printf(".\n");
			}	
		}
		else
		{
			printf("  %02X ", (unsigned char)array[n]);
			if(array[n] >= 32 && array[n] <= 126)
			{	
				printf("%c", array[n]);
			}
			else
			{
				printf(".");
			}
		}
	}
	return 0;
}
// reading first four lines of P2 or P5 file
ImageProps ReadFourLines(FILE* infile)
{
	ImageProps file; // initialising struct
  	int LineLength = 250; // maximum size of line
  	char LineRead[LineLength];
 	for(int counter = 0; counter < 4; counter++)  // iterating through first four lines
 	{
 		fgets(LineRead, sizeof(LineRead), infile); 
  		if(counter == 0)
  		{
  			sscanf(LineRead, "%s", file.ImageType);
  		}
  		else if(counter == 2)
  		{
  			sscanf(LineRead, "%d %d", &file.Width, &file.Height);
  		}
  		else if(counter==3)
  		{
  			sscanf(LineRead, "%d", &file.MaxValue);
  		}
  	}
  	return file;
}	
// writing first four lines to outfile for rotate functionality
int WriteFourLinesRotate(FILE* outfile, ImageProps file)
{
	fprintf(outfile, "%s\n", file.ImageType);
	fprintf(outfile, "# Generated by pnmdump.exe\n");
	fprintf(outfile, "%d %d\n", file.Height, file.Width); // reversing height and width
	fprintf(outfile, "%d\n", file.MaxValue);
	return 0;
}
// writing first four lines to outfile 
int WriteFourLines(FILE* outfile, ImageProps file)
{	
	if(!strcmp(file.ImageType, "P2")) // changing format of picture
	{
		fprintf(outfile, "P5\n");
	}
	else if(!strcmp(file.ImageType, "P5"))
	{
		fprintf(outfile, "P2\n");
	}
	fprintf(outfile, "# Generated by pnmdump.exe\n");
	fprintf(outfile, "%d %d\n", file.Width, file.Height);
	fprintf(outfile, "%d\n", file.MaxValue);
	return 0;
}
// converting P2 to P5
int P2toP5Func(FILE* infile, FILE* outfile, ImageProps file)
{		
	int nRows = file.Height;
	int nCols = file.Width;
	int PicArray[nRows][nCols];
	unsigned char CharPicArray[nRows][nCols];
	for(int i = 0; i < nRows; i++)
	{
		for(int j = 0; j < nCols; j++)
		{
			fscanf(infile, "%d", &(PicArray[i][j]));	
			if(PicArray[i][j] > file.MaxValue)
			{
				return 1;
			}
			CharPicArray[i][j] = (unsigned char) PicArray[i][j];
		}
	}
	fwrite(&CharPicArray, 1, sizeof(CharPicArray), outfile);
  	return 0;
}
// converting P5 to P2
int P5toP2Func(FILE* infile, FILE* outfile, ImageProps file)
{
	int nRows = file.Height;
	int nCols = file.Width;
	unsigned char PicArray[nRows][nCols];
	int nBytes = fread(&PicArray, 1, sizeof(PicArray), infile);
	if(nBytes != (nRows * nCols)) // checking that the array was filled
	{
		return 1;
	}
	unsigned char BytesOverlap[nRows][nCols];
	int nBytesOverlap = fread(&BytesOverlap, 1, sizeof(BytesOverlap), infile);
	if(nBytesOverlap > 0)  // checking if there are bytes still in the file to be read, there shouldn't be
	{
		return 1;
	}
	for(int i = 0; i < nRows; i++)
	{
		for(int j = 0; j < nCols; j++)
		{	
			if(j == (nCols - 1))
			{
				fprintf(outfile, "%u\n", (unsigned) PicArray[i][j]);
			}
			else
			{
				fprintf(outfile, "%u ", (unsigned) PicArray[i][j]);
			}
		}
	}
	return 0;
}
// Rotating an array
int WriteRotateArray(FILE* infile, FILE* outfile, ImageProps file)
{
	
	int nRows = file.Height;
	int nCols = file.Width;
	int PicArray[nRows][nCols]; 
	unsigned char RotatedArray[nCols][nRows];
	unsigned char CharPicArray[nRows][nCols];
	if(!strcmp(file.ImageType, "P2"))
	{
		for(int i = 0;i < nRows; i++)
		{
			for(int j = 0; j < nCols; j++)
			{
				fscanf(infile, "%d", &(PicArray[i][j]));
				CharPicArray[i][j] = (unsigned char) PicArray[i][j];
				unsigned char element = CharPicArray[i][j];
				RotatedArray[j][i] = element; // reversing [i][j] to [j][i]
			}
		}
		for(int i = 0; i < nCols; i++)
		{
			for(int j = 0; j < nRows; j++)
			{
				if(j == (nRows - 1))
				{
					fprintf(outfile, "%u\n", (unsigned) RotatedArray[i][j]);
				}
				else
				{
					fprintf(outfile, "%u ", (unsigned) RotatedArray[i][j]);
				}
			}
		}
	}
	else if(!strcmp(file.ImageType, "P5"))
	{
		fread(&CharPicArray, 1, sizeof(CharPicArray), infile);
		for(int i = 0; i < nRows; i++)
		{
			for(int j = 0; j < nCols; j++)
			{ 
				unsigned char element = CharPicArray[i][j];
				RotatedArray[j][i] = element; // reversing [i][j] to [j][i]
			}
		}
		fwrite(&RotatedArray, 1, sizeof(RotatedArray), outfile);
	}
	return 0;
}
// rotating an array by 90 degrees
int WriteRotate90Array(FILE* infile, FILE* outfile, ImageProps file)
{
	int nRows = file.Height;
	int nCols = file.Width;
	int PicArray[nRows][nCols]; 
	unsigned char RotatedArray[nCols][nRows];
	unsigned char CharPicArray[nRows][nCols];
	if(!strcmp(file.ImageType, "P2"))
	{
		for(int i = 0; i < nRows; i++)
		{
			for(int j = 0; j < nCols; j++)
			{
				fscanf(infile, "%d", &(PicArray[i][j]));// scanning image into array
				if(PicArray[i][j] > file.MaxValue)
				{
					return 1;
				} 
				CharPicArray[i][j] = (unsigned char) PicArray[i][j];
			}
		}
		for(int i = 0; i < nCols; i++)
		{
	   		for(int j = 0; j < nRows; j++) 
	   		{
	      		RotatedArray[i][j] = CharPicArray[nRows - 1 - j][i]; // rotating 90 degrees clockwise
	      		if(j == (nRows - 1))
				{
					fprintf(outfile, "%u\n", (unsigned) RotatedArray[i][j]);
				}
				else
				{
					fprintf(outfile, "%u ", (unsigned) RotatedArray[i][j]);
				}
			}
	    }
	}
	else if(!strcmp(file.ImageType, "P5"))
	{
		fread(&CharPicArray, 1, sizeof(CharPicArray), infile);
		for(int i = 0; i < nCols; i++)
		{
			for(int j = 0; j < nRows; j++)
			{ 
				RotatedArray[i][j] = CharPicArray[nRows - 1 - j][i];
			}
		}
	  	fwrite(&RotatedArray, 1, sizeof(RotatedArray), outfile);
	}
	return 0;
}
// making a picture negative
int negativeFunc(FILE* infile, FILE* outfile, ImageProps file)
{	
	fprintf(outfile, "%s\n", file.ImageType); // outputting first four lines with no changes
	fprintf(outfile, "# Generated by pnmdump.exe\n");
	fprintf(outfile, "%d %d\n", file.Width,file.Height);
	fprintf(outfile, "%d\n", file.MaxValue);
	if(!strcmp(file.ImageType, "P2"))
	{
		int nRows = file.Height;
		int nCols = file.Width;
		int PicArray[nRows][nCols];
		for(int i = 0; i < nRows; i++)
		{
			for(int j = 0; j < nCols; j++)
			{
				fscanf(infile, "%d", &(PicArray[i][j]));
				int element = PicArray[i][j];
				int NegativeElement = 255 - element; // file.MaxValue - character 
				if(j == (nCols - 1))
				{ 
					fprintf(outfile, "%d\n", NegativeElement);
				}
				else
				{
					fprintf(outfile, "%d ", NegativeElement);
				}
			}
		}
	}
	else if(!strcmp(file.ImageType, "P5"))
	{
		int nRows = file.Height;
		int nCols = file.Width;
		unsigned char PicArray[nRows][nCols];
		unsigned char NegativeArray[nRows][nCols];
		fread(&PicArray, 1, sizeof(PicArray), infile);
		unsigned char element;
		unsigned char NegativeElement;
		for(int i = 0; i < nRows; i++)
		{
			for(int j = 0; j < nCols; j++)
			{
				element = PicArray[i][j];
				NegativeElement = 255 - element;
				NegativeArray[i][j] = NegativeElement;
			}
		}
		fwrite(&NegativeArray, 1, sizeof(NegativeArray), outfile);
	}
	return 0;
}
