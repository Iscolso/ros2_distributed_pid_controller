import cv2

cam = cv2.VideoCapture("mandarina.mp4")

if not cam.isOpened():
	print("Error: OpenCV couldn't open the cam file")
else:
	ret,frame = cam.read()
	if ret:
		height,width,channels = frame.shape
		print(f"Camera is connected! image of {height}x{width} pixels")
	else:
		print("camera couldn't connect")
cam.release()
