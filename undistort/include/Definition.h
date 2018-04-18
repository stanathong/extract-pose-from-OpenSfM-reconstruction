#ifndef _DEFINITION_H_
#define	_DEFINITION_H_

#pragma once
#pragma warning (disable:4786)

#include <vector>
#include <string>
#include <set>
#include <opencv2/core/core.hpp> 

typedef std::vector< std::string >		VEC_STR;		
typedef std::vector< unsigned int >		VEC_UINT;	
typedef std::vector< int >				VEC_INT;	
typedef std::set< unsigned int>			SET_UINT;
typedef SET_UINT::iterator				SET_UINT_ITER;

// Structure of CAMERA
struct CAMERA
{
	std::string brand_model;
	double focal_prior;
	double focal;
	double k1_prior;
	double k2_prior;
	double k1;
	double k2;
	int width;
	int height;

	CAMERA() 	
	{	
		brand_model = "";
		focal_prior = 0.0; 
		focal = 0.0; 
		k1_prior = 0.0; 
		k2_prior = 0.0;
		k1 = 0.0; 
		k2 = 0.0; 
		width = -1; 
		height = -1;
	}

	CAMERA(	std::string _brand_model, double _focal_prior, double _focal,
			double _k1_prior, double _k2_prior, double _k1, double _k2, int _width, int _height)
	{	
		brand_model = _brand_model; 
		focal_prior = _focal_prior; 
		focal = _focal; 
		k1_prior = _k1_prior; 
		k2_prior = _k2_prior;
		k1 = _k1; 
		k2 = _k2; 
		width = _width; 
		height = _height;
	}
};

typedef std::vector< CAMERA >	VEC_CAMERA;

// Structure of SHOT
struct SHOT
{
	int camera_model;
	int image_id;
	double gps_dop;
	double rotation[3];
	double translation[3];
	double gps_position[3];

	SHOT() 	
	{	
		camera_model = 0;
		image_id = -1; 
		gps_dop = 0.0; 
		rotation[0] = rotation[1] = rotation[2] = 0.0; 
		translation[0] = translation[1] = translation[2] = 0.0;
		gps_position[0] = gps_position[1] = gps_position[2] = 0.0;
	}

	SHOT(	int _camera_model, int _image_id, double _gps_dop,
			double * _rotation, double * _translation, double * _gps_position)
	{	
		camera_model = _camera_model;
		image_id = _image_id; 
		gps_dop = _gps_dop; 
		for (int i = 0; i < 3; i++)
		{
			rotation[i] = _rotation[i]; 
			translation[i] = _translation[i];
			gps_position[i] = _gps_position[i];		
		}
	}

	SHOT(	int _camera_model, int _image_id, double _gps_dop,
			double _rotation0, double _rotation1, double _rotation2,
			double _translation0, double _translation1, double _translation2,
			double _gps_position0, double _gps_position1, double _gps_position2)
	{	
		camera_model = _camera_model;
		image_id = _image_id; 
		gps_dop = _gps_dop; 
		rotation[0] = _rotation0;
		rotation[1] = _rotation1;
		rotation[2] = _rotation2; 
		translation[0] = _translation0;
		translation[1] = _translation1;
		translation[2] = _translation2;
		gps_position[0] = _gps_position0;
		gps_position[1] = _gps_position1;
		gps_position[2] = _gps_position2;
	}
};

typedef std::vector< SHOT >	VEC_SHOT;

#endif
