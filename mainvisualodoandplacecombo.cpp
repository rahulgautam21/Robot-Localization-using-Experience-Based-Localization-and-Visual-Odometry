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
	//CameraFreenect camera; // Assume Kinect for XBOX 360
	CameraRGBDImages camera("/home/rahul/dataset6sem/classroomcombotest/mono/", "/home/rahul/dataset6sem/classroomcombotest/depth/");
	CameraRGBDImages camera1("/home/rahul/rtabmap_project/mono/", "/home/rahul/rtabmap_project/depth/");
	Transform opticalRotation(0,0,1,0, -1,0,0,0, 0,-1,0,0);
	camera.setLocalTransform(opticalRotation);
	camera1.setLocalTransform(opticalRotation);
	if(!camera.init(".", "calibration"))
	{
   		printf("Failed to initialize the camera!\n");
   		return -1;
	}
	if(!camera1.init(".", "calibration"))
	{
   		printf("Failed to initialize the camera1!\n");
   		return -1;
	}
	ParametersMap parameters;
	parameters.insert(ParametersPair(Parameters::kVisMaxFeatures(), "2000"));
	parameters.insert(ParametersPair(Parameters::kGFTTQualityLevel(), "0.0001"));
	RegistrationVis registration(parameters);

	SensorData frame ;
	/*for(int i=0;i<6; ){
		char ch;
		std::cin>>ch;
		if(ch=='y'){
			frame = camera.takeImage();
			i++;
			cv::Mat rgb = frame.imageRaw();
   			std::string s = "/home/rahul/immono.png";
   			cv::imwrite( s, rgb );
   			cv::waitKey(1);
			cv::Mat depth = frame.depthRaw();
	   		s = "/home/rahul/imdepth.png";
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
	*/
	frame = camera.takeImage();
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
	/*std::cout<<"To start final rec press s\n";
	char ch1;	
	std::cin>>ch1;
	*/frame = camera.takeImage();
	ang = -1*d2r(ang);
	std::cout<<inx<<" "<<iny<<" "<<ang<<"\n";
	myfile.close();
	SensorData previousFrame = frame;

	int k = 1;

	Transform pose(inx,iny,ang);
	float hinx=inx,hiny=iny,hang=ang;

	while(frame.isValid())
	{
	   frame = camera.takeImage();
	   Transform t = registration.computeTransformation(frame, previousFrame); 
	   if(t.isNull())
	   {
	      printf("Could not compute motion\n");
	   }
	   else
	   {
		Transform posenew(hinx/100,hiny/100,hang);
	        posenew *= t;
		float x,y,z,roll,pitch,yaw,temp,tx,ty;
		posenew.getTranslationAndEulerAngles(x, y, z, roll, pitch, yaw);
		printf("Motion orig: x = %f y = %f z = %f roll=%f pitch = %f yaw = %f\n",x,y,z,roll,pitch,yaw);
		tx = x;
		ty = y;
	   }
	  cv::Mat rgb = frame.imageRaw();
	  cv::Mat rgb1;
	   std::string s = "/home/rahul/immono.png";
	   cv::imwrite( s, rgb );
	   rgb.convertTo(rgb,CV_8UC3);
	    //cv::imshow("Image",rgb);  
	    cv::waitKey(1);
	//   cv::waitKey(1);
	    cv::Mat depth = frame.depthRaw();
	    s = "/home/rahul/imdepth.png";
	    cv::imwrite( s, depth );
		std::string filename = "/home/rahul/rtabmap_project/TestRum.py";
		std::string command = "python ";
		command += filename;
		system(command.c_str());
		myfile.open("Coord.txt");
		myfile>>hinx>>hiny>>hang;
		std::cout<<hinx<<" "<<hiny<<" "<<hang<<"\n";
		myfile.close();
		std::string s1;
		myfile.open("Start.txt");
		myfile>>s1;
		myfile.close();
		std::string s2 = "/home/rahul/dataset6sem/UpdatedClassroomDataset/Mono/" + s1;
		std::string s3 = "/home/rahul/dataset6sem/UpdatedClassroomDataset/Depth/" + s1;
		std::cout<<"\n#"<<s2<<"#";
		rgb = cv::imread(s2);
		//cv::imshow("Image",rgb);  
	    	cv::waitKey(1);
		depth = cv::imread(s3);
		cv::waitKey(1);
		std::stringstream rgbs,ds;		
		s2 = "/home/rahul/rtabmap_project/mono/" ;
		s3 = "/home/rahul/rtabmap_project/depth/" ;
		rgbs<<s2<<k<<".png";
		ds<<s3<<k<<".png";
		std::cout<<"\n*"<<rgbs.str()<<"\n";
		cv::imwrite(rgbs.str(),rgb);
		cv::waitKey(1);
		cv::imwrite(ds.str(),depth);
		cv::waitKey(1);
	   	previousFrame = camera1.takeImage();;
	   	k++;
	}	
return 0;
}
