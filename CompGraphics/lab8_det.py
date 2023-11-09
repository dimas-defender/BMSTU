def get_k(x1,y1,x2,y2):
    if x1 == x2:
        return 10000
    return (y2-y1)/(x2-x1)

def line_intersection(line1, line2):
    xdiff = (line1[0] - line1[2], line2[0] - line2[2])
    ydiff = (line1[1] - line1[3], line2[1] - line2[3])

    def det(a, b):
        return a[0] * b[1] - a[1] * b[0]

    div = det(xdiff, ydiff)
    if div == 0:
       return False, False

    d = (det((line1[0],line1[1]),(line1[2],line1[3])), det((line2[0],line2[1]),(line2[2],line2[3])))
    x = det(d, xdiff) / div
    y = det(d, ydiff) / div
    return x, y

def edges_intersect(polygon):
    for i in range(len(polygon) - 1):
        edge1 = polygon[i]
        for j in range(i + 2, len(polygon)):
            if i == 0 and j == len(polygon) - 1:
                continue
            edge2 = polygon[j]
            x,y = line_intersection(edge1, edge2)
            if x == False:
                continue
            y = 800 - y
            x1 = edge1[0]
            y1 = 800 - edge1[1]
            x2 = edge1[2]
            y2 = 800 - edge1[3]
            x3 = edge2[0]
            y3 = 800 - edge2[1]
            x4 = edge2[2]
            y4 = 800 - edge2[3]
            k1 = get_k(x1,y1,x2,y2)
            k2 = get_k(x3,y3,x4,y4)
            if k1 == 10000:
                t1 = (y-y1)/(y2-y1)
                t2 = (x-x3)/(x4-x3)
            elif k2 == 10000:
                t2 = (y-y3)/(y4-y3)
                t1 = (x-x1)/(x2-x1)
            else:
                t1 = (x-x1)/(x2-x1)
                t2 = (x-x3)/(x4-x3)
                
            if t1 >= 0 and t1 <= 1 and t2 >= 0 and t2 <= 1:
                return True
    return False
