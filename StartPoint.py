import numpy as np
import cv2
from matplotlib import pyplot as plt
from PIL import Image
import os
import shutil


def CheckNeighbours(a,b,c):
    neighbours.clear()
    ele = [-2,-1,0,1,2]
    angle = [0,60,120,180,240,300]
    for x in ele:
        for y in ele:
            for theta in angle:
                if z <= 0:
                    z = 360+z
                if z >= 360:
                    z=z-360
                
                if IsInValid((x+a),(y+b),z):
                    continue
                l=[x+a,y+b,z]
                neighbours.append(l)

def IsInValid(a,b,c):
    l = [a,b,c]
    if (a<1):
        return True
    if (b<1):
        return True
    if(c<=0):
        return True

def StartPointFLANN(p,q,r,dimg,mimg):
    length=0
    i=0
    matchedTuple='DefAULT'
    orb = cv2.ORB_create(nfeatures=p,scoreType=q)	
    kp1, des1 = orb.detectAndCompute(dimg,None)
    kp2, d2 = orb.detectAndCompute(mimg,None)
    if(des1 is None):
        des1 = d2
    else:
        des1 = np.concatenate((des1,d2),axis =0)
    for l in os.listdir('/home/rahul/dataset6sem/UpdatedClassroomDataset/Mono'):
        #print('l is ')
        #print(l)
        i=i+1
        #print(i)
        s = '/home/rahul/dataset6sem/UpdatedClassroomDataset/Mono/'+str(l)
        ds = '/home/rahul/dataset6sem/UpdatedClassroomDataset/Depth/'+str(l)
        #print(s)
        img2=cv2.imread(s,0)
        img1=cv2.imread(ds,0)
        orb = cv2.ORB_create(nfeatures=int(p),scoreType = q)

        # find the keypoints and descriptors with SIFT
        kp1, d1 = orb.detectAndCompute(img2,None)
        kp2, des2 = orb.detectAndCompute(img1,None)
        if(des2 is None):
            des2=d1
        else:
            des2 = np.concatenate((des2, d1), axis=0)
            
        if(des2 is None):
            continue
        FLANN_INDEX_LSH = 6

        index_params= dict(algorithm = FLANN_INDEX_LSH,
                   table_number = 6, # 12
                   key_size = 12,     # 20
                   multi_probe_level = 1) #2

        search_params = dict(checks = 50)
        flann = cv2.FlannBasedMatcher(index_params, search_params)

        matches = flann.knnMatch(des1,des2,k=2)

        good = []
        for m_n in matches:
          if len(m_n) != 2:
            continue
          (m,n) = m_n
          if m.distance < r*n.distance:
            good.append(m)
        #print(len(good))
        if(length<len(good)):
            length=len(good)
            matchedTuple=l
            #print(l)
            #print(length)
    print('starting image is')
    print(matchedTuple)
    print(length)
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



l = 'img.png'

xx  = '/home/rahul/immono.png'
dxx = '/home/rahul/imdepth.png'

x = StartPointFLANN(500,cv2.ORB_HARRIS_SCORE,0.5,cv2.imread(dxx,0),cv2.imread(xx,0))
f = open('Start.txt','w')
z=x.split('_')
f.write(z[0])
f.write(' ')
f.write(z[1])
f.write('\n')
f.close()
print('next')
