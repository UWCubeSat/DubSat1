import csv
import pprint
beforedict={}
with open("before.csv") as csvfile:
	reader = csv.DictReader(csvfile,delimiter="\t")
	for row in reader:
		try:
			# print(row["name"] + ", " +str(int(row["=calculated_id"])+2**31))
			beforedict[row["name"]] = int(row["=calculated_id"])
		except:
			print(row["name"])
# pprint.pprint(beforedict)
afterdict={}
with open("after.csv") as csvfile:
	reader = csv.DictReader(csvfile)
	for row in reader:
		try:
			# print(row["name"] + ", " +str(int(row["=calculated_id"])+2**31))
			afterdict[row["name"]] = int(row["=calculated_id"])
		except:
			print(row["name"])
# pprint.pprint(afterdict)
print("===========================")
replacementDict = []
for i in beforedict:
	if i in afterdict:
		# print(str(beforedict[i])+","+str(afterdict[i]))
		if beforedict[i] != afterdict[i]:
			replacementDict.append([beforedict[i]+2**31,afterdict[i]+2**31])
pprint.pprint(replacementDict)