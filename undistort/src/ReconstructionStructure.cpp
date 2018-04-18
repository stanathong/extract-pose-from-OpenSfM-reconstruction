/**********************************************************
 * Project:			undistort
 * File:			ReconstructionStructure.cpp
 * Description:		Read reconstruction.json file to get R,t
 * Last updated:	1 April 2016
 * Author:			stanathong
 **********************************************************/

#include <stdio.h>				// To manage FILE
#include <stdlib.h>				// To manage FILE
#include <iostream>
#include "../external/rapidjson/document.h"
#include "../external/rapidjson/filereadstream.h"
#include "../include/ReconstructionStructure.h"

CReconstructionStructure::CReconstructionStructure()
{
}

CReconstructionStructure::CReconstructionStructure(const std::string &_json_file_name)
{
	doInitialize(_json_file_name);
}

bool CReconstructionStructure::doInitialize(const std::string &_json_file_name)
{
	// Get the path to store the resulting RT files
	getRTPath(_json_file_name);

	m_bJSON_VALID = readReconstruction(_json_file_name);

	return m_bJSON_VALID;
}

bool CReconstructionStructure::readReconstruction(const std::string &_json_file_name)
{
	static const char* kTypeNames[] = 
		{ "Null", "False", "True", "Object", "Array", "String", "Number" };

	// Declaration of buffer
	int BUFFER_SIZE = 65535;
	char * buffer;
	buffer = reinterpret_cast<char *> (malloc(sizeof(char) * (BUFFER_SIZE+1))); 
	if (!buffer)
	{
		m_bJSON_VALID = false;
		return false;
	}

	// Open the json file to read
	FILE * pfile = fopen(_json_file_name.c_str(), "r");
	rapidjson::FileReadStream frstream(pfile, buffer, BUFFER_SIZE);
	rapidjson::Document document;
	document.ParseStream(frstream);
	fclose(pfile);
	free(buffer);

	if (document.HasParseError())
	{
		std::cout << "Failed to parse the reconstruction file: " << _json_file_name << std::endl;		
		m_bJSON_VALID = false;
		return false;
	}

	if (document.IsArray())
	{
		rapidjson::Value::ConstValueIterator it;
		for (it = document.Begin(); it != document.End(); ++it)
		{
			VEC_CAMERA vec_cameras;		// Camera parameter the current partial reconstruction
			const rapidjson::Value& object = *it;
			
			rapidjson::Value::ConstMemberIterator itmember;
			for (itmember = object.MemberBegin(); itmember != object.MemberEnd(); ++itmember)
			{
				if (caseInsensitiveStringCompare("cameras", itmember->name.GetString()) && itmember->value.IsObject())
				{
					const rapidjson::Value& cameras = object["cameras"];

					// Iterate to each camera in the document "cameras"					
					for (rapidjson::Value::ConstMemberIterator it_camera = cameras.MemberBegin(); it_camera != cameras.MemberEnd(); ++it_camera)
					{
						const rapidjson::Value& camera = it_camera->value;

						std::string brand_model = it_camera->name.GetString();
						double focal_prior = camera["focal_prior"].GetDouble();
						double focal  = camera["focal"].GetDouble();
						double k1_prior = camera["k1_prior"].GetDouble();
						double k2_prior = camera["k2_prior"].GetDouble();
						double k1 = camera["k1"].GetDouble();
						double k2 = camera["k2"].GetDouble();
						int width = (camera["width"].GetInt() < 0)? IMAGE_WIDTH : camera["width"].GetInt();
						int height = (camera["height"].GetInt() < 0)? IMAGE_HEIGHT : camera["height"].GetInt();

						//m_cameras.push_back(CAMERA(brand_model, focal_prior, focal, k1_prior, k2_prior, k1, k2, width, height));
						vec_cameras.push_back(CAMERA(brand_model, focal_prior, focal, k1_prior, k2_prior, k1, k2, width, height));
					}
				}

				if (caseInsensitiveStringCompare("shots", itmember->name.GetString()) && itmember->value.IsObject())
				{
					const rapidjson::Value& shots = object["shots"];

					// Iterate to each frame in the document "shots"
					for (rapidjson::Value::ConstMemberIterator it_shot = shots.MemberBegin(); it_shot != shots.MemberEnd(); ++it_shot)
					{
						// Store the output file name
						std::string shot_name = it_shot->name.GetString();
						m_output_files.push_back(shot_name + ".txt");

						int i;

						const rapidjson::Value& shot = it_shot->value;

						int image_id = getImageID(it_shot->name.GetString());
						std::string camera_name = shot["camera"].GetString();
						//int camera_model = getIdxCameraBrandModel(camera_name);
						int camera_model = getIdxCameraBrandModel(camera_name, vec_cameras);

						double gps_dop = shot["gps_dop"].GetDouble();
						double rotation[3];
						i = 0;
						for (rapidjson::Value::ConstValueIterator itr = shot["rotation"].Begin(); itr != shot["rotation"].End(); ++itr, i++)
						{
							rotation[i] = itr->GetDouble();
						}
						
						double translation[3];
						i = 0;
						for (rapidjson::Value::ConstValueIterator itr = shot["translation"].Begin(); itr != shot["translation"].End(); ++itr, i++)
						{
							translation[i] = itr->GetDouble();
						}

						double gps_position[3];
						i = 0;
						for (rapidjson::Value::ConstValueIterator itr = shot["gps_position"].Begin(); itr != shot["gps_position"].End(); ++itr, i++)
						{
							gps_position[i] = itr->GetDouble();
						}

						m_shots.push_back(SHOT(camera_model, image_id, gps_dop, rotation, translation, gps_position));
					}
				}
			}
		}
	}

	return true;
}

// Produce RT matrix files for each image frame
bool CReconstructionStructure::produceRT()
{
	// Create the undistort folder
	std::string command = "mkdir " + m_str_RT_path;
	system(command.c_str());

	for (int i = 0; i < m_output_files.size(); i++)
	{
		cv::Mat R, RT(3, 4, CV_64FC1);

		// Create R from rotation vector
		cv::Mat rvec(3, 1, CV_64FC1);
		rvec.at<double>(0,0) = m_shots[i].rotation[0];
		rvec.at<double>(1,0) = m_shots[i].rotation[1];
		rvec.at<double>(2,0) = m_shots[i].rotation[2];
		cv::Rodrigues(rvec, R);

		// Create RT
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				RT.at<double>(y,x) = R.at<double>(y,x);
		RT.at<double>(0,3) = m_shots[i].translation[0];
		RT.at<double>(1,3) = m_shots[i].translation[1];
		RT.at<double>(2,3) = m_shots[i].translation[2];

		// Store the result to the output file
		std::string filename = m_str_RT_path + m_output_files[i]; 
		std::ofstream fileRT(filename.c_str(), std::ofstream::out | std::ofstream::app);
		if (!fileRT.is_open())
			continue;

		// 1. RT matrix
		for (int r = 0; r < 3; r++)
		{
			std::string line =	to_string_with_precision(RT.at<double>(r,0)) + " " + 
								to_string_with_precision(RT.at<double>(r,1)) + " " +
								to_string_with_precision(RT.at<double>(r,2)) + " " +
								to_string_with_precision(RT.at<double>(r,3));
			fileRT << line << "\n";
		}
		// 2. GPS position
		std::string line =	to_string_with_precision(m_shots[i].gps_position[0]) + " " +
							to_string_with_precision(m_shots[i].gps_position[1]) + " " +
							to_string_with_precision(m_shots[i].gps_position[2]);
		
		fileRT << "\n" << line << "\n";
		fileRT.close();
	}

	return true;
}

// Get the camera brand model in the format: "brand model"
// Example: "v2 pointgrey zebra2cropped-carriageway -1 -1 perspective 0.9375"
// The function returns "pointgrey zebra2cropped-carriageway"
std::string CReconstructionStructure::getCameraBrandModel(const std::string &input_string)
{
	std::string brand, model, brand_model;
	std::size_t pos1, pos2, pos3;

	pos1 = input_string.find(" ", 0)+1; // The first space or the start position of brand
	pos2 = input_string.find(" ", pos1); // The second space
	pos3 = input_string.find(" ", pos2+1); // The third space or the last position of model
	brand_model = input_string.substr(pos1, pos3-pos1);

	return brand_model;
}

// Get the image id from the image file name
// Example: "Carriageway_28.jpg", the function returns 28
int CReconstructionStructure::getImageID(const std::string &image_file_name)
{
	std::size_t pos1, pos2;
	pos2 = image_file_name.find_last_of(".");
	pos1 = image_file_name.find_last_of("_")+1;
	std::string str_index = image_file_name.substr(pos1, pos2-pos1);
	
	return std::stoi(str_index);
}

// Get the camera brand model index in the m_cameras
int CReconstructionStructure::getIdxCameraBrandModel(const std::string &camera_name)
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		if (caseInsensitiveStringCompare(camera_name, m_cameras[i].brand_model))
			return i;
	}
	
	return -1; // Not found
}

int CReconstructionStructure::getIdxCameraBrandModel(const std::string &camera_name, VEC_CAMERA &cameras)
{
	for (int i = 0; i < cameras.size(); i++)
	{
		if (caseInsensitiveStringCompare(camera_name, cameras[i].brand_model))
			return i;
	}
	
	return -1; // Not found
}

// Get the path to the folder to store RT files
// Example: _json_file_name = OPENSFM_PATH/data/DATA_NAME/xxx.json
// RTPath is in the format of OPENSFM_PATH/data/undistorted/
// In case, "data" not exist, RT path will be OPENSFM_PATH/data/DATA_NAME/undistorted/
void CReconstructionStructure::getRTPath(const std::string &_json_file_name)
{
	// Extract the path until 'data'
	std::string str_data = "data";
	std::size_t pos = _json_file_name.rfind(str_data);	// Position of 'data' in the path
	std::string str_data_path;
	if (pos == std::string::npos) // Not found - store under the same folder as the json file
	{
		pos = _json_file_name.find_last_of("\\/");	
		str_data_path = _json_file_name.substr(0, pos);
	}
	else // Found
		str_data_path = _json_file_name.substr(0, pos) + "data";

	if (isDSBackSlash(_json_file_name))
		m_str_RT_path = str_data_path + "\\undistorted\\";
	else 
		m_str_RT_path = str_data_path + "/undistorted/";
}
