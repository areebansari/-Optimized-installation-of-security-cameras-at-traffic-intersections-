import re

def numbers_check(string):
        for numbers in re.findall("[(][^)]*[)]", string):
            if len(re.findall("[-+]?\d+[\.]?\d*[eE]?[-+]?\d*", numbers)) == 2:
                continue
            else:
                return 0
##                print "False" #test#
        return 1
    
def bracket_check(string):
        if "(" not in string or ")" not in string:
            return 0
        count = 0
        for i in string:
            if i == "(":
                count += 1
            elif i == ")":
                if count == 0:
                    return 0
                count -= 1
        return count==0
    
def extractNumbers(string):
    new_vertex = []
    
    string = string.replace(" ", "")
    string = string.replace(")(", ",")
    string = string.replace(")", "")
    string = string.replace("(", "")
    string = string.replace(",", " ")
    string = string.split(' ')
    
    x = string[0::2]
    y = string[1::2]

    for i in range(len(x)):
        x[i] = float(x[i])
        y[i] = float(y[i])
    return zip(x,y)

def line_intersection(line1, line2):
    xdiff = (line1[0][0] - line1[1][0], line2[0][0] - line2[1][0])
    ydiff = (line1[0][1] - line1[1][1], line2[0][1] - line2[1][1])

    def det(a, b):
        return a[0] * b[1] - a[1] * b[0]

    div = det(xdiff, ydiff)
    if div == 0:
       return None

    d = (det(*line1), det(*line2))
    x = det(d, xdiff) / div
    y = det(d, ydiff) / div
    
    return x, y

def poly_intersection(poly1, poly2):
        
    length = len(poly1) + len(poly2)
    for k in range(length):
        for i, p1_first_point in enumerate(poly1[:-1]):
            p1_second_point = poly1[i + 1]
            x1,y1 = p1_first_point
            x2,y2 = p1_second_point
            
            for j, p2_first_point in enumerate(poly2[:-1]):
                p2_second_point = poly2[j + 1]
                x3,y3 = p2_first_point
                x4,y4 = p2_second_point
                
                xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
                yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
                xden_new = 0
                xden_new_2 = 0

                if xden == 0 and yden == 0:
                        xden_new = (x3-x1)*(y2-y4) - (x2-x4)*(y3-y1)
                        xden_new_2 = (x4-x1)*(y3-y2) - (x3-x2)*(y4-y1) 
        
                if xden_new == 0 and xden_new_2 == 0:
                        parallel = 1
                else:
                        parallel = 0
                
                if line_intersection((p1_first_point, p1_second_point), (p2_first_point, p2_second_point)):
                            x,y = line_intersection((p1_first_point, p1_second_point), (p2_first_point, p2_second_point))
                            if onSegment(x1,x2,x,y1,y2,y) and onSegment(x3,x4,x,y3,y4,y):
                                    intersect.append([p1_first_point])
                                    intersect.append([line_intersection((p1_first_point, p1_second_point), (p2_first_point, p2_second_point))])
                                    intersect.append([p1_second_point])

                                    intersect.append([p2_first_point])
                                    intersect.append([line_intersection((p1_first_point, p1_second_point), (p2_first_point, p2_second_point))])
                                    intersect.append([p2_second_point])

                                    global_edges.append([p1_first_point, line_intersection((p1_first_point, p1_second_point), (p2_first_point, p2_second_point)), p2_first_point])
                                    global_edges.append([p1_second_point, line_intersection((p1_first_point, p1_second_point), (p2_first_point, p2_second_point)), p2_second_point])
                elif parallel == 1:
                    intersect.append([p1_first_point])
                    intersect.append([p1_second_point])
                    intersect.append([p2_first_point])
                    intersect.append([p2_second_point])
                    global_edges.append([p1_first_point,p2_first_point,p1_second_point,p2_second_point])
                        
##        print ("result: ", (intersect)) #test#
        #result = set(result)
##        print "global edges: ",global_edges
        return (intersect)               
                

    return None

def onSegment(x1, x2, xcoor, y1, y2, ycoor):
    if min(x1, x2) <= xcoor and xcoor <= max(x1, x2) and min(y1, y2) <= ycoor and ycoor <= max(y1, y2):
        return True
    else:
        return False  

def plotVertexandEdge():
        temp_dic = street_coord_dic
        values_coords = []
        result = []
        flag = 0
        for i in temp_dic:
                values_coords.append(temp_dic[i])
##        print (values_coords)
        #print len(values_coords)
        if(len(values_coords) == 1):
           #return None
                flag = 1
        elif(len(values_coords) > 2):
                flag = 0
                for i in range(len(values_coords)):
                        for j in range (i+1, len(values_coords)):
                                result = poly_intersection(values_coords[j-1], values_coords[i-1])
        if(len(values_coords)== 2):
                flag = 0
                for i in range(len(values_coords)-1):
                        result = poly_intersection(values_coords[i-1], values_coords[i])
        #for i in range(len(result)):
        if result not in common_vertex:
                common_vertex.extend(result)
##        print ("result: ", (result)) #test#
##        print ("common vertex: ", (common_vertex)) #test#
        
        new_intersection_line = []
        tmp = []
    
        for j in range(len(result) - 1):
                for i in range(j+1, len(result)):
                    
                        if result[j][0] == result[i][0] and result[j][-1] == result[i][-1]:
                                for p in result[j]:
                                    tmp.append(p)
                                for q in result[i]:
                                    if q not in tmp:
                                        tmp.append(q)
                                    else:
                                        continue
                    
                                del result[i]
                                del result[j]
                
                                new_intersection_line.append(sorted(tmp))
                
                                tmp = []
                                break
                
                if j == len(result) - 2:
                    break
        
        for i in new_intersection_line:
                result.append(i)
    
        pos = []
        for i in result:
                for j in i:
                    if j not in pos:
                        pos.append(j)
                        
        output_vertices = {}
        del result[:]
        for i in pos:
                output_vertices[pos.index(i)] = i

##        print ("output: ", output_vertices) #test#

        if flag==1:
                #global_edges.clear()
                print "V = {"
                
                print "}"
##                print "E = {"
##                print "}"
        else:
                print "V = {"
                for key, value in output_vertices.items():
                        print ' ' + str(key) + ': '+'('+ str("{0:.2f}".format(value[0])) + ',' + str("{0:.2f}".format(value[1])) + ')'
                print "}"
                
        final_edges = []

##        for x in range(len(result)):
##                print ("test outer loop")
##                for y in range(len(result)-1):
##                        print ("test inner loop")
        
        for i in range(len(global_edges)):
##              print ("IN edges outer for loop")
                for j in range(len(global_edges[i])-1):
##                      print ("IN edges inner for loop")
                        first = list(output_vertices.keys())[list(output_vertices.values()).index(global_edges[i][j])]
##                      print ("first: ", first)
                        second = list(output_vertices.keys())[list(output_vertices.values()).index(global_edges[i][j+1])]
##                      print ("second: ", second)
            
                        if first != second:
                                res = (first, second) if first < second else (second, first)
                                final_edges.append(res)
   
        final_edges = list(set(final_edges))

        print "E = {"
        for i in range(len(final_edges)): 
                if i == len(final_edges)-1:
                        print ' <' + str(final_edges[i][0]) + ',' + str(final_edges[i][1]) + '>'
                else:
                        print ' <' + str(final_edges[i][0]) + ',' + str(final_edges[i][1]) + '>,'
            
        print '}'
                
sts_coord = []
common_vertex = []
street_coord_dic = {}
global_edges = []
intersect = []

def main():
    ### YOUR MAIN CODE GOES HERE

    ### sample code to read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment

        while True:
            command = raw_input("Enter input: ")

            if 'g' == command.strip(''):
                plotVertexandEdge()
        
            elif '"' not in command:
                print "Error: Invalid command. Try again"
    
            elif '"' in command and len(command.split('"'))==3:
                split_command = command.split('"')

##        print split_command
        
        ##street name check##    
                if all(x.isalpha() or x.isspace() for x in split_command[1]) and split_command[1] != "":
                    valid_name = 1
                else:
                    valid_name = 0
        ##########################
                
                if bracket_check(split_command[2]) and numbers_check(split_command[2]) and " " in split_command[2] and valid_name==1:
                        sts_coord = extractNumbers(split_command[2])
                        for i in sts_coord:
                                if i not in common_vertex:
                                        common_vertex.append(i)
                            
                        if "a " in split_command[0] and len(split_command)==3:
                                if split_command[1].lower() not in street_coord_dic:
                                        street_coord_dic[split_command[1].lower()] = sts_coord
                                else:
                                        print "Error: Street name already exists"
                        elif "c " in split_command[0] and len(split_command)==3:
                                if split_command[1].lower() in street_coord_dic:
                                        street_coord_dic[split_command[1].lower()] = sts_coord
                                else:
                                        print "Error: Street name does not exist"
                        else:
                                print "Error: Command is not valid"
            
            #Test#
##            print "valid input"
##            print sts_coord
##            print street_coord_dic
            ######
                elif 'r' in split_command[0].strip('') and valid_name==1 and len(split_command[0]) == 2:
                        del global_edges[:]
                        if split_command[1].lower() in street_coord_dic:
                                del street_coord_dic[split_command[1].lower()]
                        else:
                                print "Error: Street does not exist"
                else:
                        print "Error: Invalid command"
            else:
                    print "Error: Invalid command format"

if __name__ == '__main__':
    main()
