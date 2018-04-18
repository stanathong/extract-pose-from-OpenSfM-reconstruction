/**********************************************************
 * Project:			undistort
 * File:			ReconstructionStructure.h
 * Description:		Read reconstruction.json file to get R,t
 * Last updated:	1 April 2016
 * Author:			stanathong
 **********************************************************/

#ifndef _RECONSTRUCTION_STRUCTURE_H_
#define	_RECONSTRUCTION_STRUCTURE_H_

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>		// for standard I/O
#include <fstream>		// for std::ifstream
#include <cstddef>
#include <cstdlib>		// To use system
#include "Global.h"

class CReconstructionStructure
{
public:
	CReconstructionStructure();

	CReconstructionStructure(const std::string &_json_file_name);

	bool doInitialize(const std::string &_json_file_name);

	bool produceRT();

private:
	bool readReconstruction(const std::string &_json_file_name);

	// Get the path to the folder to store RT files
	void getRTPath(const std::string &_json_file_name);

	// Helper function
	// Get the camera brand model in the format: "brand model"
	std::string getCameraBrandModel(const std::string &input_string);

	// Get the image id from the image file name
	int getImageID(const std::string &image_file_name);

	// Get the camera brand model index in the m_cameras
	int getIdxCameraBrandModel(const std::string &camera_name);
	int getIdxCameraBrandModel(const std::string &camera_name, VEC_CAMERA &cameras);

public:
	VEC_CAMERA		m_cameras;
	VEC_SHOT		m_shots;
	VEC_STR			m_output_files;

private:
	bool			m_bJSON_VALID;			// A flag indicating whether the json file is valid		
	std::string		m_str_RT_path;			// Path to store RT files
};


#endif
