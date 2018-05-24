# extract-pose-from-OpenSfM-reconstruction
A handy tool to extract camera-pose in the form of projection matrix from `reconstruction.json`, the output of OpenSfM

After running [OpenSfM](https://github.com/mapillary/OpenSfM) for camera pose estimation, we obtain `reconstruction.json` as an output. This file contains 3 info: "cameras" for intrinsics parameters, "shots" for camera poses for each shot and "points".

This repository contains a small C++ program that reads the `reconstruction.json` file and outputs files containing projection matrix (3x4) for each shot/camera and the GPS priors. 

Each output file is named as _IMAGE_FILE_NAME.txt_, for example `Camera100.jpg.txt`, storing the projection matrix for the input image `Camera100.jpg`.

The format of the resulting file is simply<br>
<br>
```
p11 p12 p13 p14
p21 p22 p23 p24
p31 p32 p33 p34

gps_x pgs_y gps_z<br>

```
The code is developed in C++ using MS Visual Studio 2012 BUT there is no reference to any MFC specific functions. It uses [RapidJson](http://rapidjson.org/) to read the content from the json file. However, there is no need to download this library as all the necessary files are already included in this project under the folder "external".

After building, it can be run on Windows by using command
`undistort PATH_TO_RECONSTRUCTION.json`<br>

I have included an example reconstruction file in the folder "Data". <br>

An example to run this under cmd terminal on Windows is by <br>
`undistort D:\\extract-pose-from-OpenSfM-reconstruction\\undistort\\Data\\reconstruction_20151022_153411_CW_4.json`<br>
or on Linux as<br>
`undistort /home/user/source/extract-pose-from-OpenSfM-reconstruction/undistort/Data/reconstruction_20151022_153411_CW_4.json`

The code will create a new folder named `undistorted` under the main path, by using system command. As illustrated above, slash must be used according to the running operating system (that is `\\` for Windows and `/` for Linux). This is because excuting command `mkdir path` will fail on Windows if the forward slash `/` is used when the program is trying to create a new folder. To avoid the hassle about slash, you may comment out the command to create the folder as shown in the code below and create yourself the folder named `undistorted` in the folder where your json file is located.

```
bool CReconstructionStructure::produceRT()
{
	// Create the undistort folder
	std::string command = "mkdir " + m_str_RT_path;
	//system(command.c_str()); // Comment this out
	...
}
```  



