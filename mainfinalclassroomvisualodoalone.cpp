#include <rtabmap/core/RegistrationVis.h>
#include <rtabmap/core/CameraRGBD.h>
#include <rtabmap/core/util3d_transforms.h>
#include <rtabmap/utilite/ULogger.h>
#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace rtabmap;

static float d2r(float d) {
  return (d / 180.0) * ((float) M_PI);
}

int main(int argc, char * argv[])
{
	CameraFreenect camera; // Assume Kinect for XBOX 360
	//CameraRGBDImages camera("/home/rahul/dataset6sem/classroom_testrun2/mono/", "/home/rahul/dataset6sem/classroom_testrun2/depth/");
	Transform opticalRotation(0,0,1,0, -1,0,0,0, 0,-1,0,0);
	camera.setLocalTransform(opticalRotation);
	if(!camera.init(".", "calibration"))
	{
   		printf("Failed to initialize the camera!\n");
   		return -1;
	}
	
	ParametersMap parameters;
	parameters.insert(ParametersPair(Parameters::kVisMaxFeatures(), "2000"));
	parameters.insert(ParametersPair(Parameters::kGFTTQualityLevel(), "0.0001"));
	RegistrationVis registration(parameters);

	SensorData frame = camera.takeImage();
  	float inx,iny,ang;
	std::cin>>inx>>iny>>ang;
	ang = ang;
	inx = inx/100;
	iny = iny/100;
	//ang = 90-ang;
	ang = -1*d2r(ang);
	std::cout<<inx<<" "<<iny<<" "<<ang<<"\n";
	SensorData previousFrame = frame;

	int k = 1;

	Transform pose(inx,iny,ang);
	//Transform rotation(0,0,0,0,0,(float)M_PI/2);
	Transform posenew ;
	//pose = posenew;
/*	pose.data()[3] = inx;
	pose.data()[7] = iny;
	pose.data()[0] = cos(ang);
	pose.data()[1] = -1*sin(ang);
	pose.data()[2] = 0;
	pose.data()[4] = sin(ang);
	pose.data()[5] = cos(ang);
	pose.data()[6] = 0;
	pose.data()[8] = 0;
	pose.data()[9] = 0;
	pose.data()[10] = 1;
*/
	while(frame.isValid())
	{
	   Transform t = registration.computeTransformation(previousFrame, frame, Transform()); 
	   if(t.isNull())
	   {
	      printf("Could not compute motion\n");
	   }
	   else
	   {
	        pose *= t;
		float x,y,z,roll,pitch,yaw,temp,tx,ty;
		pose.getTranslationAndEulerAngles(x, y, z, roll, pitch, yaw);
		printf("%d Motion orig: x = %f y = %f z = %f roll=%f pitch = %f yaw = %f\n",x,y,z,roll,pitch,yaw);		
		tx = x;
		ty = y;
	   }
	   previousFrame = frame;
	   k++;
	   char ch;
	   std::cin>>ch;
	   if(ch=='y')
	   	frame = camera.takeImage();
	}	
return 0;
}
