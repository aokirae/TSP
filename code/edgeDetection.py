import cv2
import numpy as np
import sys
import matplotlib.pyplot as plt
import random

if __name__ == '__main__':
	img = cv2.imread("../data/graphics/小日向美穂.jpg")
	edges = cv2.Canny(img, 150, 200)
	# cv2.imwrite("../data/edges/小日向美穂_エッジ検出.png", edges)

	# 選択点数
	points = 3000
	# 選択点数のリスト
	choice_point = [[0 for i in range(len(edges[j]))] for j in range(len(edges))]

	# 255(白)の点の座標のリスト
	list_address = []
	for i in range(len(edges)):
		for j in range(len(edges[i])):
			if edges[i,j] == 255:
				list_address.append([i,j])
	# pointsの数だけ点を選択
	list_choice_address = random.sample(list_address, points)
	list_choice_address.sort()
	for i in list_choice_address:
		choice_point[i[0]][i[1]] = 255
	# 画像出力
	cv2.imwrite("../data/edges/小日向美穂_エッジ選択_"+str(points)+"点.png", np.array(choice_point))
	# csv出力
	with open("../data/edges/小日向美穂_エッジ選択_"+str(points)+"点.csv", mode="w") as f:
		for i in list_choice_address:
			f.write(str(i[0]))
			f.write(",")
			f.write(str(i[1]))
			f.write("\n")
