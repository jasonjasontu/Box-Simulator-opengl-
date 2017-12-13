// File for switching between project and tutorial code.  

#include <Project1.hpp>
#include <SessionI_Part1.hpp>
#include <SessionI_Part2.hpp>
// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


int main()
{
	// Doing this the easy way, just comment out what you want to run here. 

	// OpenGL Session I Part 1
	// return SessionI_Part1::SessionI_Part1();

	// OpenGL Session I Part 2
	// return SessionI_Part2::SessionI_Part2();


	// Project 1 starter code
	return Project1::Project1();

}




