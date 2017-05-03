from sklearn import svm
from sklearn import datasets
import cv2
import os
import numpy as np
train_data = []

fr = open('training_data.txt','r')
data = fr.readlines()   
fr.close()

print(len(data))
z = []
for d in data:
	x = d.split('_')
	z = [float(x[0]),float(x[1]),float(x[2]),float(x[3]),float(x[4]),float(x[5]),float(x[6]),float(x[7]),float(x[8])]
	train_data.append(z)

#print(len(train_data))

fr = open('label_data.txt','r')
class_data = fr.readlines()   
fr.close()

'''
print(len(class_data))
print(class_data[1])
print(train_data[1])
'''
clf = svm.LinearSVC(C=1.0, class_weight=None, dual=True, fit_intercept=True,
     intercept_scaling=1, loss='squared_hinge', max_iter=1000,
     multi_class='ovr', penalty='l2', random_state=None, tol=0.0001,
     verbose=0)
clf.fit(train_data, class_data)

def HOG(im):
    im = np.float32(im) / 255.0
 
    # Calculate gradient 
    gx = cv2.Sobel(im, cv2.CV_32F, 1, 0, ksize=1)
    gy = cv2.Sobel(im, cv2.CV_32F, 0, 1, ksize=1)


    mag, angle = cv2.cartToPolar(gx, gy, angleInDegrees=True)

    hist = [0,0,0,0,0,0,0,0,0]

    for i in range (480):
        for j in range (640):
            an = angle[i][j]
            if an > 179.9999:
                an = an - 180
            fb = an/20
            fb = int(fb)
            sb = fb+1
            minA=20*fb
            maxA = 20*sb
            if sb == 9:
                minA = 160
                maxA = 180
                sb = 0
            d_fb = an - minA
            d_sb = maxA - an
            if d_sb < 0.0:
                print(an)
            hist[fb] = hist[fb] + mag[i][j]*(d_fb/20)
            hist[sb] = hist[sb] + mag[i][j]*(d_sb/20)

    return (hist)


for file1 in os.listdir('/home/rahul/dataset6sem/TEST2/mono'):
	z = '/home/rahul/dataset6sem/TEST2/mono/' + str(file1)
	#print(z)
	y = HOG(cv2.imread(z,0))
	#print(y)
	y = np.array(y).reshape(1,-1)
	x = clf.predict(y)
	print(x)
