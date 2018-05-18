# extract-pose-from-OpenSfM-reconstruction
A handy tool to extract camera-pose in the form of projection matrix from Reconstruction.json, the output of OpenSfM

After running [OpenSfM](https://github.com/mapillary/OpenSfM) for camera pose estimation, we obtain Reconstruction.json as an output. This file contains 3 info: "cameras" for intrinsics parameters, "shots" for camera poses for each shot and "points".

This is just a small C++ program that reads the Reconstruction.json file and produces files storing projection matrix (3x4) for each shot/camera. 

The resulting file each name as _IMAGE_FILE_NAME.txt_, for example Camera100.jpg.txt storing the projection matrix for the image Camera100.jpg

The format of the resulting file is simply<br>
<br>
p11 p12 p13 p14<br>
p21 p22 p23 p24<br>
p31 p32 p33 p34<br>
<br>
gps_x pgs_y gps_z<br>

The code is developed in C++ using MS Visual Studio 2012 as IDE without referering to any MFC specific function. It uses [RapidJson](http://rapidjson.org/) for reading json file. However, there is no need to download any as all the necessary files are already included in the folder "external".

After building, it can be run on Windows by using command
`undistort PATH_TO_RECONSTRUCTION.json`<br>

I have included an example reconstruction file in the folder Data. <br>

An example to run this under cmd terminal on Windows is by <br>
`undistort D:\\extract-pose-from-OpenSfM-reconstruction\\undistort\\Data\\reconstruction_20151022_153411_CW_4.json`
or on Linux as
`undistort /home/user/source/extract-pose-from-OpenSfM-reconstruction/undistort/Data/reconstruction_20151022_153411_CW_4.json`

The code will create a new folder named `undistorted` under the main path, by using system command. As illustrate above, slash must be used according to the running operating system (that is `\\` for Windows and `/` for Linux). This is because excuting command `mkdir path` will fail on Windows if the forward slash `/` is used when the program creates a new folder. To avoid the hassle about slash, you may comment out the command to create the folder as shown in the code below:

```
bool CReconstructionStructure::produceRT()
{
	// Create the undistort folder
	std::string command = "mkdir " + m_str_RT_path;
	//system(command.c_str()); // Comment this out
	...
}
```  



