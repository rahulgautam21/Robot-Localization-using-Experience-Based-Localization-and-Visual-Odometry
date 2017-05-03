import numpy as np
import cv2
from matplotlib import pyplot as plt
from PIL import Image
import os
import shutil

neighbours = []

tilesize = [50,38] 

def CheckNeighbours(a,b,c):
    ele = [-2,-1,0,1,2]
    angle = [0,60,120,180,240,300]
    for x in ele:
        for y in ele:
            for theta in angle:
		z=theta
                if z < 0:
                    z = 360+z
                if z >= 360:
                    z=z-360
                
                if IsInValid(int(x+a),int(y+b),int(z)):
                    continue
                l=[x+a,y+b,z]
                neighbours.append(l)

def IsInValid(a,b,c):
    l = [a,b,c]
    if (a<1):
        return True
    if (b<1):
        return True
    if(c<0):
        return True


#dim depth image address
#im mono image address

def yohuu_FLANN(p,q,r,dim,im):
    global prev
    harsh=0;
    newarray = []
    fr = open('Start.txt','r')
    data = fr.readline()
    fr.close()
    xxx = data.split('_')
    for i in range(1):
        cur=0
        ss='a'
        harsh=harsh+1
        matchedTuple=str('Default')
        #print(neighbours)
        #Image.open(im).show()
        #i1='D:\\dataset6sem\\TEST1\\depth\\'+str(dim)
        #i2='D:\\dataset6sem\\TEST1\\mono\\'+str(im)
        #i1 = cv2.imread(im1,0)
        #i2 = cv2.imread(im2,0)
	#print(xxx)
	CheckNeighbours(int(xxx[0]),int(xxx[1]),int(xxx[2]))
        orb = cv2.ORB_create(nfeatures=p,scoreType=q)
        #print(neighbours)
	
        kp1, des1 = orb.detectAndCompute(dim,None)
        kp2, d2 = orb.detectAndCompute(im,None)
   
        if(des1 is None):
	    des1 = d2
        else:
	    des1 = np.concatenate( (des1,d2) , axis = 0)
	    #des1 = d2
	#print(des1)
	if (des1 is None):
		print ('des1 is None')
        for l in neighbours:
            #print('l is ')
            #print(l)
            s = '/home/rahul/dataset6sem/UpdatedClassroomDataset/Mono/'+ str(l[0]) + '_' + str(l[1]) + '_' + str(l[2]) + '_.png'
            #print(s)
            ds='/home/rahul/dataset6sem/UpdatedClassroomDataset/Depth/'+ str(l[0]) + '_' + str(l[1]) + '_' + str(l[2]) + '_.png'
            #print(im1)
            img2=cv2.imread(s,0)
            img1=cv2.imread(ds,0)
            #img1 = cv2.imread('E:\\Test Run\\Test Run 1\\KinectSnapshot-10-15-11.bmp',0)
            

            # find the keypoints and descriptors with SIFT
            kp1, des2 = orb.detectAndCompute(img1,None)
            kp2, d1 = orb.detectAndCompute(img2,None)
            
            #des2.append(d2)
            FLANN_INDEX_LSH = 6
            if(des2 is None):
                des2=d1
	    else :
		des2 = np.concatenate( (des2,d1) , axis = 0)
		#des2 = d1
	    #print(des2)
            if(des2 is None):
		continue
	    #print(des2)
	    #print(des1)
            FLANN_INDEX_LSH = 6
            index_params= dict(algorithm = FLANN_INDEX_LSH,
                   table_number = 6, # 12
                   key_size = 12,     # 20
                   multi_probe_level = 1) #2

            search_params = dict(checks = 50)
            flann = cv2.FlannBasedMatcher(index_params, search_params)

            matches = flann.knnMatch(des1,des2,k=2)
            
            #print(len(matches))
            good = []
            for m_n in matches:
              if len(m_n) != 2:
                continue
              (m,n) = m_n
              if m.distance < r*n.distance:
                good.append(m)
            #print(len(good))
            length=(len(good))
            if(length>cur):
                cur=length
                matchedTuple=l
                #print(cur)
        
        #Image.open(im1).show()
        #Image.open(ss).show()
        h=matchedTuple
	f = open('Start.txt','w')
	xxx  = str(h[0])+'_'+str(h[1])+'_'+str(h[2])+'_.png'
	f.write(xxx)
        newarray.append(matchedTuple)
        #CheckNeighbours(h[0],h[1],h[2])
	return matchedTuple

#yohuu_FLANN()
'''
                        
a=[500,1000,2000]
b=[cv2.ORB_FAST_SCORE,cv2.ORB_HARRIS_SCORE]
c=[0.5,0.6,0.7,0.8,0.9]
F=open('CLASSROOM_START_POINT.txt',"w")
for p in a:
    for q in b:
        for r in c:
            if(b==cv2.ORB_FAST_SCORE):
                j='FAST'
            else:
                j='HARRIS'
            X=StartPointFLANN(p,q,r)
            x = X.split('_')
            F.write(x[0])
            F.write(',')
            F.write(x[1])
            F.write(',')
            F.write(x[2])
            F.write('\n')
            s=str('TESTRUNTest1DepthPointFLANN')+str(int(r*10))+str(j)+str(p)+'.txt'
            F = open(s,"w")
            print(s)
            CheckNeighbours(StartPointCordinate)
            x = yohuu_FLANN(p,q,r)
            for i in x:
                sid = str(i[0]) + ' ' +str(i[1]) + ' '+str(i[2])
                F.write(sid)
                F.write('\n')
            F.write('\n')
            F.close()        
F.close() ```

'''
f = open('Start.txt','r')

l = f.readline()

l = str(l)
l = l.split('_')
xx = '/home/rahul/immono.png'
#print(s)
dxx='/home/rahul/imdepth.png'
x = yohuu_FLANN(500,cv2.ORB_HARRIS_SCORE,0.5,cv2.imread(dxx,0),cv2.imread(xx,0))
print(x)
s = str(x[0]*tilesize[1])+' '+ str(x[1]*tilesize[1])+' '+ str(x[2])+'\n'
f = open('Coord.txt','w')
f.write(s)
f.close()
