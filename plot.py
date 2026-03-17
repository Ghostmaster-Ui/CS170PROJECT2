import matplotlib.pyplot as plt

x = [1, 2, 3, 4]
y = [88.0, 95.3, 95.3, 96.0]

plt.plot(x, y, marker='o')
plt.xlabel("Number of Features")
plt.ylabel("Accuracy (%)")
plt.title("Forward Selection on Iris Dataset")
plt.xticks(x)
plt.grid()

plt.show()
