import random
import math

# 1. royal flush
# 2. straight flush
# 3. four of a kind
# 4. full house
# 5. flush
# 6. straight
# 7. three of a kind
# 8. two pair
# 9. one pair
# 10. high card

# possible sets:
# four of a kind, full house, three of a kind, two pair, one pair
# 3, 4, 7, 8, 9

# black spades (♠), red hearts (♥), blue diamonds (♦) and green clubs (♣)
colors = ['♠','♥','♦','♣']
figures = ['A','K','Q','J']
number = ['2','3','4','5','6','7','8','9','10']

def drawHandFigures():
    res = []
    while len(res)!=5:
        tmp = random.choice(figures) + random.choice(colors)
        if tmp not in res:
            res.append(tmp)
    return sorted(res)

def whatSet(hand):
    figuresCount = [0,0,0,0]
    for card in hand:
        figuresCount[figures.index(card[0])]+=1
    if 4 in figuresCount:
        # print('four of a kind')
        return 3
    if 3 in figuresCount and 2 in figuresCount:
        # print('full house')
        return 4
    if 3 in figuresCount:
        # print('three of a kind')
        return 7
    pairCount = sum(x==2 for x in figuresCount)
    if pairCount == 2:
        # print('two pair')
        return 8
    if pairCount == 1:
        # print('one pair')
        return 9
    

customDeck = [  '2♠','3♠','4♠','5♠','6♠','7♠','8♠','9♠','10♠',
                '2♥','3♥','4♥','5♥','6♥','7♥','8♥','9♥','10♥',
                '2♦','3♦','4♦','5♦','6♦','7♦','8♦','9♦','10♦',
                '2♣','3♣','4♣','5♣','6♣','7♣','8♣','9♣','10♣']

def calculateChance(hand):
    colorsCount = [0,0,0,0]
    numberCount = [0,0,0,0,0,0,0,0,0]
    colorSorted = [[],[],[],[]]

    for card in customDeck:
        for c in colors:
            if c in card:
                colorsCount[colors.index(c)]+=1
                colorSorted[colors.index(c)].append(int(card[0:-1]))
        for n in number:
            if n in card:
                numberCount[number.index(n)]+=1
    
    # print('color sorted cards:')
    for c in colorSorted:
        c.sort()
        # print(c) 
    # print('colorsCount =', colorsCount, 'numberCount =', numberCount)
    

    # a < b : math.comb(a,b) = 0
    allPossibleHands = math.comb(len(customDeck),5)
    chances = [0,0,0,0,0,0,0,0,0,0]
    chances[0] = 0 # royal flush
    
    for c in colorSorted:
        if len(c) >= 5:
            for i in range(len(c)-5+1):
                if all([c[i+j]==(c[i+j+1]-1) for j in range(4)]):
                    chances[1]+=1 # straight flush

    for n in numberCount:
        if n >= 4:
            chances[2] += math.comb(len(customDeck)-4,1) # four of a kind

    for n in range(len(numberCount)):
        if numberCount[n] >= 3:
            for i in range(len(numberCount)):
                if i!=n and numberCount[i] >= 2:
                    chances[3] += math.comb(numberCount[n],3) + math.comb(numberCount[i],2) # full house

    chances[4] = math.comb(colorsCount[0],5) + math.comb(colorsCount[1],5) + math.comb(colorsCount[2],5) + math.comb(colorsCount[3],5) # flush

    for i in range(len(numberCount)-5+1): #math.prod - iloczyn wszystkich elementow listy
        chances[5] += math.prod([numberCount[i+j] for j in range(5)]) # straight

    for n in range(len(numberCount)):
        if numberCount[n] >= 3:
            chances[6] += math.comb(numberCount[n],3) * math.comb(sum([numberCount[i]>=1 and i!=n for i in range(len(numberCount))]),2) # three of a kind

    # chances[7] = # two pair
    for n in range(len(numberCount)):
        if numberCount[n] >= 2:
            for i in range(len(numberCount)):
                if i!=n and numberCount[i] >=2:
                    for j in range(len(numberCount)):
                        if i!=j and j!=n and numberCount[j] >= 1:
                            chances[7] += math.comb(numberCount[n],2) + math.comb(numberCount[i],2) + math.comb(numberCount[j],1) # two pair

    for n in range(len(numberCount)):
        if numberCount[n] >= 2:
            chances[8] += math.comb(numberCount[n],2) * math.comb(sum([numberCount[i]>=1 and i!=n for i in range(len(numberCount))]),3) # one pair

    chances[9] = 0 # high card

    return min(1,(sum(chances[i] for i in range(whatSet(hand)))/allPossibleHands)) # suma szans lepszych ustawien niz przeciwnika / wszystkie combinacje


for i in range(10):
    hand = drawHandFigures()
    print(hand, '\t',format(calculateChance(hand)*100,'.2f'), '%')




# czy straight, flush i straight flush nie maja tych samych ukladow w sobie????