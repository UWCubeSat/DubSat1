f = open("links.txt")
content = f.readlines()
content = [x.strip() for x in content]
content = [x.split(",") for x in content]
content.pop()
content = [[int(x[0]), x[1], x[2]] for x in content]