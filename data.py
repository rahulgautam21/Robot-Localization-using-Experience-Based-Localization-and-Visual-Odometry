import os
import shutil

for fi in os.listdir('/home/rahul/dataset6sem/Office/Depth'):
	fi1 = fi.split('_')
	source = '//home//rahul//dataset6sem//Office//Depth//'+str(fi)
	x = fi1[0]
	y = fi1[1]
	z = fi1[2]
	s = '//home//rahul//dataset6sem//FinalOfficeDepth//' + str(x)+ '_' + str(y) +'_'+ str(z) +'_.png'
	shutil.copy(source,s)
