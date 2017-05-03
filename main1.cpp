#include <rtabmap/core/RegistrationVis.h>
#include <rtabmap/core/CameraRGBD.h>
#include <rtabmap/core/util3d_transforms.h>
#include <iostream>
using namespace rtabmap;

int main(int argc, char * argv[])
{

//CameraFreenect camera; // Assume Kinect for XBOX 360
//camera.setImageRate(1); // capture at 1 Hz

CameraRGBDImages camera("/home/rahul/uimages/rgb/", "/home/rahul/uimages/depth/");
//camera.init(".", "calibration");

Transform opticalRotation(0,0,1,0, -1,0,0,0, 0,-1,0,0);
camera.setLocalTransform(opticalRotation); // from camera frame to rtabmap/ros frame

if(!camera.init(".", "calibration")) // use "calibration.yaml" in the current directory
{
	std::cout<<"Failed to initialize Camera";  
}

ParametersMap parameters;
parameters.insert(ParametersPair(Parameters::kVisMaxFeatures(), "2000"));
parameters.insert(ParametersPair(Parameters::kGFTTQualityLevel(), "0.0001"));
RegistrationVis registration(parameters);
//RegistrationVis registration;

SensorData frame = camera.takeImage();
SensorData previousFrame = frame;

int k = 1;

Transform pose = Transform::getIdentity();
while(frame.isValid())
{
   Transform t = registration.computeTransformation(previousFrame, frame); 
   
   if(t.isNull())
   {
      printf("Could not compute motion\n");
   }
   else
   {
        pose *= t;
	printf("%d Motion: %s\n", k, pose.prettyPrint().c_str());
//        previousFrame = frame;
   }
   previousFrame = frame;
   frame = camera.takeImage();
   k++;   
}


return 0;
}

