mapping = {
    "Iris-setosa": 1,
    "Iris-versicolor": 2,
    "Iris-virginica": 3
}

with open("iris.data") as f, open("iris_formatted.txt", "w") as out:
    for line in f:
        if line.strip() == "":
            continue
        parts = line.strip().split(",")
        features = parts[:-1]
        label = mapping[parts[-1]]
        out.write(str(label) + " " + " ".join(features) + "\n")
