import cv2
import numpy as np
import sys
import matplotlib.pyplot as plt
import random
from PIL import Image, ImageDraw

if __name__ == '__main__':

	# 255(白)の点の座標のリスト
	list_address = []
	file_name = "../data/solve/小日向美穂_焼きなまし法_3000点"
	with open(file_name + ".csv") as f:
		for line in f:
			list_tmp = line.split(",")
			list_address.append((int(list_tmp[1]),int(list_tmp[0])))

	list_address = tuple(list_address)

	# 元画像をの解像度を取得
	original_img = cv2.imread("../data/graphics/小日向美穂.jpg")
	height = len(original_img)
	width = len(original_img[0])
	img = Image.new("RGB", (height,width), (0,0,0))
	draw = ImageDraw.Draw(img)

	draw.line(list_address, fill=(255,255,255), width=1)

	img.save(file_name+".png", quality=95)

	# 画像出力
	# cv2.imwrite(file_name+".png", np.array(choice_point))
