import random
import math

if __name__ == "__main__":
	NumOfTown = 10
	NumOfAgent = 20
	NumOfSolve = 500
	ro = 0.4
	random.seed(2)

	towns = [i for i in range(NumOfTown)]
	positions = [(random.randint(1,100), random.randint(1,100)) for i in range(NumOfTown)]

	roads = {}
	pheromone = {}
	for i in towns:
		for j in towns:
			if i is j:
				continue
			roads[(i,j)] = math.sqrt((positions[i][0] - positions[j][0])**2 +
							(positions[i][1] - positions[j][1])**2)
			pheromone[(i,j)] = random.random()

	# print("roads")
	# print(roads)
	print("pheromone")
	print(pheromone)
	print(pheromone[(0,1)])
