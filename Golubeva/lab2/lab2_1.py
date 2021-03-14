
path = input().split(' ') #list

vertice = input()
dictionary = {}
while (vertice):
    current = vertice.split(' ')
    if current[0] in dictionary:
        dictionary[current[0]].append(dict.fromkeys([current[1]], float(current[2])))#if current source vertice alredy in dictionary
    else:
        dictionary[current[0]]=[dict.fromkeys([current[1]], float(current[2]))]#if current source vertice not in dictionary
    try:    
        vertice = input()
    except:
        break
print("Our incoming pathes")    
print(dictionary)

current_path = [path[0]]#store a path of vertices
size = 1#current path size

while current_path[size-1]!=path[1]:
    print ("\nCurrent path")
    print(current_path)
    #trying to make path from the last vertice in current path
    try:
        find = dictionary[current_path[size-1]]#list of dictionaries
        print("Trying find greedy path from "+current_path[size-1])
    except:
        print("No path from "+ current_path[size-1]+", delete this vertice from current_path")
        current_path.pop()
        size-=1
        continue

    min_size = 1000
    for i in find:# i - dict
        keys = i.keys()# get a keys for current source vertice

        for j in keys:
            if i[j] < min_size:
                min_size = i[j]
                min_key = j
                item = i
    find.remove(item)#remove vertice from dictionary
    print("Find greedy path from "+min_key)
    current_path.append(min_key) 
    size+=1       
        
str_result = "\nResult path: "        
for i in current_path:
    str_result+=str(i)
print(str_result)