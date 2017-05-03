#include <rtabmap/core/RegistrationVis.h>
#include <rtabmap/core/CameraRGBD.h>
#include <rtabmap/core/util3d_transforms.h>
#include <rtabmap/utilite/ULogger.h>
#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
using namespace rtabmap;

static float d2r(float d) {
  return (d / 180.0) * ((float) M_PI);
}


int main(int argc, char * argv[])
{
//ULogger::setType(ULogger::kTypeConsole);
//ULogger::setLevel(ULogger::kDebug);

//CameraFreenect camera; // Assume Kinect for XBOX 360
CameraRGBDImages camera("/home/rahul/dataset6sem/TEST1/mono/", "/home/rahul/dataset6sem/TEST1/depth/");
camera.setImageRate(4); // capture at 1 Hz
Transform opticalRotation(0,0,1,0, -1,0,0,0, 0,-1,0,0);
camera.setLocalTransform(opticalRotation);

float inx,iny,ang;
scanf("%f%f%f",&inx,&iny,&ang);
ang = d2r(ang);

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
SensorData previousFrame = frame;

int k = 1;

Transform pose = Transform::getIdentity();

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
	temp = y;
//	y = x;
//	x = -1*temp;
//	printf("%d Motion orig: x = %f y = %f ang = %f\n",x,y,ang);
	tx = x;
	ty = y;
	x = (tx*cos(ang))+(ty*sin(ang));
	y = (ty*cos(ang))-(tx*sin(ang));
	printf("%d Motion: x = %f y = %f \n",k,x+inx,y+iny);
//	std::cout<<x<<y<<"Hi";
   }
   previousFrame = frame;
   frame = camera.takeImage();
   k++;   
  cv::Mat rgb = frame.imageRaw();
  cv::Mat rgb1;
//frame1 = &frame; 
  //rgb = frame1->rgb;
   //int id = frame;
   std::string s = "/home/rahul/im.jpg";
   cv::imwrite( s, rgb );
   //rgb1 = cv::imread("/home/rahul/im.jpg");
   rgb.convertTo(rgb,CV_8UC3);
    cv::imshow("Image",rgb);  
    cv::waitKey(1);
 //  std::cout<<rgb1;
}


return 0;
}

