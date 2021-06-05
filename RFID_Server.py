import time
import serial
import datetime
import cv2

# set up the serial line
ser = serial.Serial('COM6', 9600)
time.sleep(2)

# read and resize images
img1 = cv2.imread('hamidreza.jpg',1)
img1 = cv2.resize(img1,(250,250))

img2 = cv2.imread('abolfazl.jpg',1)
img2 = cv2.resize(img2,(250,250))

img3 = cv2.imread('gholamreza.jpg',1)
img3 = cv2.resize(img3,(250,250))

# database name, enter/exit , date, image, loaded from file
data = [["Hamidreza Radfar", False, "",img1,False],
        ["Abolfazl Omrani", False, "",img2,False],
        ["Gholamreza Dar", False, "",img3,False]]

# load initial data
with open("log.txt","r") as f:
    lines = f.readlines()
    for line in lines[::-1]:
        current_data = line.split()
        for user in data:
            if (" ".join(current_data[0:2]))==user[0] and user[-1]==False:
                user[-1] = True
                user[1] = True if (current_data[2]== "Enter") else False
# cv2.imshow("Profile",img3)
# empty list to store the data
while(True):
    
    # reading from Arduinos serial output
    b = ser.readline()  
    string_n = b.decode()
    name = string_n.rstrip()
    
    # reading current date and time
    now = datetime.datetime.now()

    # find user
    for user in data:
        if user[0] == name:
            user[1] = not user[1]
            user[2] = now
            log = name.ljust(20) +( " Enter".ljust(8) if user[1] else " Exit".ljust(8)) + " "+now.strftime("%Y-%m-%d %H:%M:%S")
            
            # printing log to console
            print(log)

            # show user profile picture
            cv2.imshow("Profile",user[3])
            cv2.waitKey(2000)
            time.sleep(1)
            # saving log to file
            with open("log.txt", "a") as f:
              f.write(log+"\n")
              
            break
            
    time.sleep(3)

ser.close()
