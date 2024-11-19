BMP Image Viewer

A C++ program that opens and displays binary BMP images with 2 colors (black and white) using ASCII art.

Overview

This project utilizes the C++ Standard Template Library (STL) and Object-Oriented Programming (OOP) principles to create a simple BMP image viewer. The program reads a BMP file, checks its validity, and then outputs the image using ASCII characters.

Features

  Supports opening of small to medium-sized BMP files
  Displays images with only 2 colors (black and white)
  Uses ASCII art to render the image
  Implemented using OOP principles with a BMPImage class
  Utilizes STL containers (e.g. std::vector) and file input/output streams (std::fstream)
  Handles exceptions using std::exception

Usage

  Compile the program using a C++ compiler (e.g. g++)
  Run the program, providing the path to a valid BMP file as a command-line argument
  The program will display the image using ASCII characters

Requirements

  C++ compiler (e.g. g++)
  BMP file with 2 colors (black and white)
  
Notes

  The program assumes that the input BMP file is not too large, as it reads the entire file into memory.
  The output image is rendered using ASCII characters, which may not be suitable for large or complex images.
  
Example Use Case

  Compile the program:

  g++ -o bmp_viewer bmp_viewer.cpp
		
Run the program with a sample BMP file:

  ./bmp_viewer sample.bmp
		
This will display the image using ASCII characters.
