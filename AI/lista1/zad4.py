
def nonogram(row, n):
    oneCount = sum([r=='1' for r in row])
    res = len(row)
    for i in range(len(row)-n+1):
        changes = 0
        oneCopy = oneCount
        for j in range(n):
            if row[i+j] == '0':
                changes+=1
            else:
                oneCopy-=1
        changes+=oneCopy
        res = min(res,changes)
    return res

# with open('zad4_output.txt','a') as out:
with open('zad4_input.txt','r') as file:
    for line in file:
        [row,n] = line.split()
        n = int(n)
        # out.write(nonogram(row,n))
        print(nonogram(row,n))