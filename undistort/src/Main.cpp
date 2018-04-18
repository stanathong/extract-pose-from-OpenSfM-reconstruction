/**********************************************************
 * Project:			undistort
 * File:			Main.cpp
 * Description:		Read reconstruction.json file to get R,t
 * Last updated:	1 April 2016
 * Author:			stanathong
 **********************************************************/

#include <cstdlib>
#include <chrono>			// To measure time

#include "../include/ReconstructionStructure.h"

void ShowUsage();

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		ShowUsage();
		return -1;
	}

	CReconstructionStructure obj(argv[1]);
	obj.produceRT();

	return 0;
}

/*
 * Print out the help message.
 */
void ShowUsage()
{
	std::cout << "Undistort\n";
	std::cout << "     Desc:      This program writes an RT matrix for each image frame into each separate file.\n";
	std::cout << "				  The resulting files will be stored under the folder \"data/undistorted\". In case that\n";
	std::cout << "				  no folder \"data\" in the passed-in path for reconstruction.json, files will be stored\n";
	std::cout << "				  in the folder \"undistorted\" folder under the same folder as the json file";
	std::cout << "     Usage:     undistort <full-path to reconstruction.json>\n";
	std::cout << "     Example:   undistort /home/user/source/OpenSfM/data/Carriageway/reconstruction.json";
	std::cout << std::endl;
}
