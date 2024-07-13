import sys
import os
import cv2
import imutils
import numpy as np
import matplotlib.pyplot as plt
import common

def check_similarity(img_name1, img_name2):
    img1 = cv2.imread(img_name1)
    height, width, channel = img1.shape
    
    img2 = cv2.imread(img_name2)
    img2 = cv2.resize(img2, (width, height))
    
    # Take the absolute difference of the images
    res = cv2.absdiff(img1, img2)
    
    # Convert the result to integer type
    res = res.astype(np.uint8)
    
    # Find percentage similarity based on number of pixels that are not zero
    percentage = round(abs(((np.count_nonzero(res)*100)/res.size)-100), 1)
    
    return percentage

def compare(img_name1, img_name2, test_name):
    # Load the two images
    img1 = cv2.imread(img_name1)
    img2 = cv2.imread(img_name2)
    
    # Resize 2 image
    height, width, channels = img1.shape
    img2 = cv2.resize(img2, (width, height))

    similarity = check_similarity(img_name1, img_name2)
    
    img_height = img1.shape[0]
    
    # Grayscale
    gray1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    
    # Find the difference between the two images
    # Calculate absolute difference between two arrays 
    diff = cv2.absdiff(gray1, gray2)
    #cv2.imshow("diff(img1, img2)", diff)
    
    # Apply threshold. Apply both THRESH_BINARY and THRESH_OTSU
    thresh = cv2.threshold(diff, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)[1]
    #cv2.imshow("Threshold", thresh)
    
    # Dilation
    kernel = np.ones((5,5), np.uint8) 
    dilate = cv2.dilate(thresh, kernel, iterations=2) 
    #cv2.imshow("Dilate", dilate)
    
    # Calculate contours
    contours = cv2.findContours(dilate.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours = imutils.grab_contours(contours)
    
    for contour in contours:
        if cv2.contourArea(contour) > 100:
            # Calculate bounding box around contour
            x, y, w, h = cv2.boundingRect(contour)
            # Draw rectangle - bounding box on both images
            cv2.rectangle(img1, (x, y), (x+w, y+h), (0,0,255), 2)
            cv2.rectangle(img2, (x, y), (x+w, y+h), (0,0,255), 2)
    
    # Show images with rectangles on differences
    x = np.zeros((img_height,5,3), np.uint8)
    result = np.hstack((img1, x, img2))
    #cv2.imshow("Differences", result)
    
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    fig=plt.figure(figsize=(8, 2.5))
    #fig.suptitle('Similarity: '+str(similarity)+'%')
    plt.tick_params(left = False, right = False, labelleft = False, labelbottom = False, bottom = False)
    plt.title('Original - Similarity: '+str(similarity)+'% - Output')
    plt.imshow(cv2.cvtColor(result, cv2.COLOR_BGR2RGB), aspect=1)
    img_name = test_name
    fig.savefig(common.REPORT_DIR+img_name+'.png', dpi=300, pad_inches=0)
    #plt.show()
    os.remove(common.OUTPUT_DIR+sys.argv[1]+'.jpg')
    return similarity

print(compare(common.SAMPLES_DIR+sys.argv[1]+'.jpg', common.OUTPUT_DIR+sys.argv[1]+'.jpg', sys.argv[2]))

