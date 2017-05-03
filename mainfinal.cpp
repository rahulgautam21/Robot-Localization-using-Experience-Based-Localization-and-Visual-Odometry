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

/*

4.125 7.125 180
3.375 3.375 

5.625 1.875 60

*/
static float d2r(float d) {
  return (d / 180.0) * ((float) M_PI);
}

float hx[6],hy[6];
float hdis[6];
std::ofstream xxx;
std::ifstream myfile;

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

	SensorData frame ;
	for(int i=0;i<6; ){
		char ch;
		std::cin>>ch;
		if(ch=='y'){
			frame = camera.takeImage();
			i++;
			cv::Mat rgb = frame.imageRaw();
   			std::string s = "/home/rahul/immono.jpg";
   			cv::imwrite( s, rgb );
   			cv::waitKey(1);
			cv::Mat depth = frame.depthRaw();
	   		s = "/home/rahul/imdepth.jpg";
			cv::imwrite( s, depth );
   			cv::waitKey(1);
			std::string filename = "/home/rahul/rtabmap_project/StartPoint.py";
			std::string command = "python ";
			command += filename;
			system(command.c_str());
			myfile.open("Start.txt");
			myfile>>hx[i-1]>>hy[i-1];
			std::cout<<hx[i-1]<<" "<<hy[i-1]<<"\n";
			myfile.close();
			
		}
	}
	
	for(int i=0;i<6;i++){
		hdis[i]=0;
		for(int j=0;j<6;j++){
			hdis[i] += (hx[i]-hx[j])*(hx[i]-hx[j])+(hy[i]-hy[j])*(hy[i]-hy[j]);
		}
	}
	float hmi=10000;
	int hind=0;
	for(int i=0;i<6;i++){
		//std::cout<<hdis[i]<<" ";
		if(hmi>hdis[i]){
			hmi=hdis[i];
			hind=i;
		}
	}
	xxx.open("Start.txt");
	xxx<<hx[hind]<<"_"<<hy[hind]<<"_0_.jpg";
	xxx.close();
  //      frame = camera.takeImage();
	cv::Mat rgb = frame.imageRaw();
   	std::string s = "/home/rahul/immono.png";
 	cv::imwrite( s, rgb );
   	cv::waitKey(1);
	cv::Mat depth = frame.depthRaw();
	s = "/home/rahul/imdepth.png";
	cv::imwrite( s, depth );
   	cv::waitKey(1);
	std::string filename = "/home/rahul/rtabmap_project/TestRum.py";
	std::string command = "python ";
	command += filename;
	system(command.c_str());
	myfile.open("Coord.txt");
	float inx,iny,ang;
	myfile>>inx>>iny>>ang;
	ang = ang;
	inx = inx/100;
	iny = iny/100;
	//ang = 90-ang;
	std::cout<<"To start final rec press s\n";
	char ch1;	
	std::cin>>ch1;
	if(ch1=='s')
		camera.setImageRate(4);
	frame = camera.takeImage();
	//std::cin>>inx>>iny>>ang;
	//ang = 90-ang;
	ang = -1*d2r(ang);
	std::cout<<inx<<" "<<iny<<" "<<ang<<"\n";
	myfile.close();
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
	//	posenew = rotation*pose;
		float x,y,z,roll,pitch,yaw,temp,tx,ty;
		pose.getTranslationAndEulerAngles(x, y, z, roll, pitch, yaw);
	//	temp = y;
	//	y = x;
	//	x = -1*temp;
		printf("%d Motion orig: x = %f y = %f z = %f roll=%f pitch = %f yaw = %f\n",x,y,z,roll,pitch,yaw);
		tx = x;
		ty = y;
	//	x = (tx*cos(ang))-(ty*sin(ang));
	//	y = (ty*cos(ang))+(tx*sin(ang));
	//	printf("rahul %d Motion: x = %f y = %f \n",k,inx+tx,iny+ty);
	//	std::cout<<x<<y<<"Hi";
	   }
	   previousFrame = frame;
	   k++;
	   frame = camera.takeImage();
	      
	  cv::Mat rgb = frame.imageRaw();
	  cv::Mat rgb1;
	//frame1 = &frame; 
	  //rgb = frame1->rgb;
	   //int id = frame;
	   std::string s = "/home/rahul/immono.png";
	   cv::imwrite( s, rgb );
	   //rgb1 = cv::imread("/home/rahul/im.jpg");
	   rgb.convertTo(rgb,CV_8UC3);
	    cv::imshow("Image",rgb);  
	    cv::waitKey(1);
	//   cv::waitKey(1);
	    cv::Mat depth = frame.depthRaw();
	    s = "/home/rahul/imdepth.png";
	    cv::imwrite( s, depth );
	std::string filename = "/home/rahul/rtabmap_project/TestRum.py";
	std::string command = "python ";
	command += filename;
	if(k%4==0)
		system(command.c_str());
	myfile.open("Coord.txt");
	float hinx,hiny,hang;
	myfile>>hinx>>hiny>>hang;
	std::cout<<hinx<<" "<<hiny<<" "<<hang<<"\n";
	myfile.close();
	 //  std::cout<<rgb1;
	
	}	
return 0;
}
